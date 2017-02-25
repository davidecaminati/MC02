#include "Pistone.h"
#include "Materasso.h"


int rele1 = 2;
int rele2 = 3;
int rele3 = 4;
int rele4 = 5;
int rele5 = 6;
int rele6 = 7;
int rele7 = 8;
int rele8 = 9;

int potenziometro = A0;

Pistone pist_blu =   Pistone(rele7,rele8);
Pistone pist_bianco =  Pistone(rele1,rele2);
Pistone pist_rosso =  Pistone(rele3,rele4);
Pistone pist_nero =  Pistone(rele5,rele6);

Materasso mat = Materasso(pist_blu,pist_bianco,pist_rosso,pist_nero);


void setup() {
   Serial.begin(9600);
   pinMode(potenziometro,INPUT_PULLUP);
   
}

void loop() {
  int altezza = analogRead(potenziometro)/10;
  
  //print_pos();
  print_status();
  
  mat.moveto(altezza);
  //print_pos();

}


void print_pos(){
    
  Serial.print("pist_front_sx = ");
  Serial.print(mat.getPos_front_sx());
  Serial.print("   ");
  Serial.print("pist_front_sx = ");
  Serial.print(mat.getPos_front_sx());
  Serial.print("   ");
  Serial.print("pist_rear_sx= ");
  Serial.print(mat.getPos_rear_sx());
  Serial.print("   ");
  Serial.print("pist_rear_dx = ");
  Serial.print(mat.getPos_rear_dx());
  Serial.print("   ");
  Serial.println("");
}
void print_status(){
    
  Serial.print("pist_front_sx_status = ");
  Serial.print(mat.getStatus_front_sx());
  Serial.print("   ");
  Serial.print("pist_front_dx_status = ");
  Serial.print(mat.getStatus_front_dx());
  Serial.print("   ");
  Serial.print("pist_rear_sx_status = ");
  Serial.print(mat.getStatus_rear_sx());
  Serial.print("   ");
  Serial.print("pist_rear_dx_status = ");
  Serial.print(mat.getStatus_rear_dx());
  Serial.print("   ");
  Serial.println("");
}

