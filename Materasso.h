/*
  Materasso.h - Libreria per gestione il sincronismo dei pistoni del progetto MC02
  Created by Davide Caminati, Febbraio 2017.
  Released into the public domain.
*/
#ifndef Materasso_h
#define Materasso_h

#include "Arduino.h"
#include "Pistone.h"

class Materasso
{
  public:
    Materasso(Pistone front_sx, Pistone front_dx, Pistone rear_sx, Pistone rear_dx);
    
    void moveto(int pos);
    int getPos_front_sx();
    int getPos_front_dx();
    int getPos_rear_sx();
    int getPos_rear_dx();
    int getStatus_front_sx();
    int getStatus_front_dx();
    int getStatus_rear_sx();
    int getStatus_rear_dx();
    void sx();
    void dx();
    

  private:
    int _height;
    Pistone _front_sx;
    Pistone _front_dx;
    Pistone _rear_sx;
    Pistone _rear_dx;
    
    
    
};

#endif
