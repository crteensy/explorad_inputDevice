#include "inputType.h"
#include <Bounce2.h>

#define SerialOutputDevice Serial1

#define PIN_LEFT 0
#define PIN_UP 1
#define PIN_RIGHT 2
#define PIN_DOWN 3
#define PIN_ENTER 4

struct Button
{
  Button(const uint8_t pin, const EInputType button) :
    debouncer_(),
    button_(button)
  {
    pinMode(pin, INPUT_PULLUP);
    debouncer_.attach(pin);
    debouncer_.interval(bounceInterval_);
  }
  void update()
  {
    if(!debouncer_.update())
    {
      return;
    }
    int value = debouncer_.read();
    if (value == LOW)
    {
      SerialOutputDevice.printf("b %d 0\n", button_);
    }
  }
  Bounce debouncer_;
  const EInputType button_;
  static const uint8_t bounceInterval_;
};

const uint8_t Button::bounceInterval_ = 5;

//template<uint8_t bufSize = 10>
//struct SerialInputParser
//{
//  SerialInputParser(Stream& device) :
//    stream_(device)
//  {
//    
//  }
//  void update()
//  {
//    while(stream_.available())
//    {
//      parseChar(stream_.read());
//    }
//  }
//  void parseChar(char c)
//  {
//    if (c == '\n')
//    {
//      buf_[index_] = 0;
//      index_++;
//      parseLine();
//    }
//    else
//    {
//      if (index_ > (bufSize-2))
//      {
//        index_ = 0;
//        return;
//      }
//      buf_[index_] = c;
//      index_++;
//    }
//  }
//  void parseLine()
//  {
//    Serial.printf("received line \"%s\"\n", buf_);
//    index_ = 0; 
//  }
//  char buf_[bufSize];
//  uint8_t index_;
//  Stream& stream_;
//};
//
void setup()
{
  Serial1.begin(9600);//, SERIAL_8N1);
}

void loop()
{
  static Button left(PIN_LEFT, eLeft);
  static Button up(PIN_UP, eUp);
  static Button right(PIN_RIGHT, eRight);
  static Button down(PIN_DOWN, eDown);
  static Button enter(PIN_ENTER, eEnter);
  
//  static SerialInputParser<10> parser(Serial1);
  
  left.update();
  up.update();
  right.update();
  down.update();
  enter.update();
  
//  parser.update();
}
