/*
  Pistone.h - Libreria per gestione i pistoni del progetto MC02
  Created by Davide Caminati, Febbraio 2017.
  Released into the public domain.
*/

#ifndef Pistone_h
#define Pistone_h

#include "Arduino.h"

class Pistone
{
  public:
    Pistone(int pin_on,int pin_direction);
    Pistone();
    
    void up(int mm);
    void down(int mm);
    void alt();
    int getStatus();
    int pos;

  private:
    int _pin_on;
    int _pin_direction;
    int _status;
};

#endif
