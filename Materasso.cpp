#include "Arduino.h"
#include "Pistone.h"
#include "Materasso.h"



Materasso::Materasso(Pistone front_sx, Pistone front_dx,Pistone rear_sx, Pistone rear_dx)
{
  _front_sx = front_sx;
  _front_dx = front_dx;
  _rear_sx = rear_sx;
  _rear_dx = rear_dx;
  
    
   
}

void Materasso::moveto(int mm)
{

  if (_front_sx.pos <= mm) {_front_sx.up(mm);}
  if (_front_sx.pos >= mm) {_front_sx.down(mm);}
  
  if (_front_dx.pos <= mm) {_front_dx.up(mm);}
  if (_front_dx.pos >= mm) {_front_dx.down(mm);}

  
  if (_rear_sx.pos <= mm) {_rear_sx.up(mm);}
  if (_rear_sx.pos >= mm) {_rear_sx.down(mm);}

  
  if (_rear_dx.pos <= mm) {_rear_dx.up(mm);}
  if (_rear_dx.pos >= mm) {_rear_dx.down(mm);}
 
}

void Materasso::sx()
{
	
}

void Materasso::dx()
{
  
}

int  Materasso::getPos_front_sx()
{
 return _front_sx.pos;
}

int  Materasso::getPos_front_dx()
{
 return _front_dx.pos;
}

int  Materasso::getPos_rear_sx()
{
 return _rear_sx.pos;
}

int  Materasso::getPos_rear_dx()
{
 return _rear_dx.pos;
}


int Materasso::getStatus_front_sx(){
  return _front_sx.getStatus();
}

int Materasso::getStatus_front_dx(){
  return _front_dx.getStatus();
}

int Materasso::getStatus_rear_sx(){
  return _rear_sx.getStatus();
}

int Materasso::getStatus_rear_dx(){
  return _rear_dx.getStatus();
}


