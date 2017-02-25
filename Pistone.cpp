#include "Arduino.h"
#include "Pistone.h"


Pistone::Pistone(int pin_direction, int pin_on)
{
  pinMode(pin_on, OUTPUT);
  digitalWrite(pin_on, HIGH);
  pinMode(pin_direction, OUTPUT);
  digitalWrite(pin_direction, HIGH);
  
  _pin_on = pin_on;
  _pin_direction = pin_direction;
  
}


Pistone::Pistone()
{
  
}

void Pistone::up( int mm)
{
  if (pos < mm){
    digitalWrite(_pin_direction, LOW);
    digitalWrite(_pin_on, LOW);
    pos += random(4, 8);
    _status = 2;
    delay(100);
  }
  else
  {
    _status = 0;
    alt();
  }
}
void Pistone::down(int mm)
{
  if (pos > mm){
    digitalWrite(_pin_direction, HIGH);
    digitalWrite(_pin_on, LOW);
    pos -= random(4, 8);
    _status = 1;
    delay(100);
    
  }
  else
  {
    _status = 0;
    alt();
  }
}
void Pistone::alt()
{
  digitalWrite(_pin_on, HIGH);
  digitalWrite(_pin_direction, HIGH);
  _status = 0;

}

int Pistone::getStatus(){
  return _status;
}


