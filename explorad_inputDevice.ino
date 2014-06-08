#include <Wire.h>

#include "inputType.h"

#define SerialOutputDevice Serial1// SerialDevice

#define PIN_LEFT 0
#define PIN_UP 1
#define PIN_RIGHT 2
#define PIN_DOWN 3
#define PIN_ENTER 4

struct Button
{
  Button(const uint8_t pinMask, const EInputType button) :
    pinMask_(pinMask),
    button_(button),
    state_(true)
  {
  }
  void update(uint8_t buttons)
  {
    bool newState = (buttons & pinMask_);
    if (newState != state_)
    {
      state_ = newState;
      if (state_ == false)
      {
        // button was pressed
        SerialOutputDevice.printf("b %d 0\n", button_);
        state_ = newState;
        timer_ = 0;
      }
    }
    else if ((state_ == false) && (timer_ >= repeatInterval_))
    {
      // repeat button press
      timer_ = 0;
      SerialOutputDevice.printf("b %d 1\n", button_);
    }
  }
  const uint8_t pinMask_;
  const EInputType button_;
  bool state_;
  elapsedMillis timer_;
  static const uint32_t repeatInterval_;
};

const uint32_t Button::repeatInterval_ = 500;

template<uint8_t bufSize = 10>
struct SerialInputParser
{
  SerialInputParser(Stream& device) :
    stream_(device)
  {
    
  }
  void update()
  {
    while(stream_.available())
    {
      parseChar(stream_.read());
    }
  }
  void parseChar(char c)
  {
    if (c == '\n')
    {
      buf_[index_] = 0;
      index_++;
      parseLine();
    }
    else
    {
      if (index_ > (bufSize-2))
      {
        index_ = 0;
        return;
      }
      buf_[index_] = c;
      index_++;
    }
  }
  void parseLine()
  {
    Serial.printf("received line \"%s\"\n", buf_);
    index_ = 0; 
  }
  char buf_[bufSize];
  uint8_t index_;
  Stream& stream_;
};

void setup()
{
  Serial1.begin(9600);//, SERIAL_8N1);
  Wire.begin();
}

void loop()
{
  static elapsedMillis buttonTimer;
  static const uint16_t buttonReadInterval = 50;
  static Button left((1<<eLeft), eLeft);
  static Button up((1<<eUp), eUp);
  static Button right((1<<eRight), eRight);
  static Button down((1<<eDown), eDown);
  static Button enter((1<<eEnter), eEnter);
  
  static SerialInputParser<10> parser(Serial1);

  if (buttonTimer >= buttonReadInterval)
  {
    buttonTimer -= buttonReadInterval;
    Wire.requestFrom(0x38, 1);
    while(Wire.available() == 0);
    uint8_t buttons =  Wire.receive();
    while(Wire.available())
    {
      Wire.receive();
    }
    left.update(buttons);
    up.update(buttons);
    right.update(buttons);
    down.update(buttons);
    enter.update(buttons);
  }
  
  parser.update();
}
