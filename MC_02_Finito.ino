/*
NOTE to avoid Timer2 overlap between IRremote and HCMotor, 
you need to change 
// Arduino Mega
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  //#define IR_USE_TIMER1   // tx = pin 11
  //#define IR_USE_TIMER2     // tx = pin 9 <----- comment this
  #define IR_USE_TIMER3   // tx = pin 5    <------ uncomment this
  //#define IR_USE_TIMER4   // tx = pin 6
  //#define IR_USE_TIMER5   // tx = pin 46

from "boarddefs.h"  file 

*/

// TODO  bloccare i movimenti iniziali dei pistoni durante l'upload di nuovi sketch

enum direzione {
  Su,
  Giu  
};

const int pinRele1 = 32;
const int pinRele2 = 34;
const int delay_for_rele = 400;

 

int RECV_PIN = 5;
#include <IRremote.h>
IRrecv irrecv(RECV_PIN);
decode_results results;
const int positive_pin_IR = 3;
const int negative_pin_IR = 4;

unsigned long right_1 = 2148500513;
unsigned long right_2 = 2148467745;
unsigned long left_1 = 2148500512;
unsigned long left_2 = 2148467744;
unsigned long up_1 = 2148500510;
unsigned long up_2 = 2148467742;
unsigned long down_1 = 2148500511;
unsigned long down_2 = 2148467743;
unsigned long tilt_right_1 = 2148467727;
unsigned long tilt_right_2 = 2148500495;
unsigned long tilt_left_1 = 2148467747;
unsigned long tilt_left_2 = 2148500515;


 

/* Include the library */
#include <HCMotor.h>

/* Pins used to drive the motors */
#define DIR_PIN 22 //Connect to drive modules 'direction' input.
#define CLK_PIN 24 //Connect to drive modules 'step' or 'CLK' input.
const int en_pin_pos = 30;
const int en_pin_neg = 28;

/* Create an instance of the library */
HCMotor HCMotor;

// corona 120 denti
// pignone 6 denti
// stepper 1,8 gradi per step (200 steps)

const int my_steps = 1600;
//const int my_steps = 16000;
const int Speed = 12;


#include <SPI.h>
#include <Wire.h>

#define MPU_1 0x68  // I2C address of the MPU-6050

int16_t AcX_1,AcY_1,AcZ_1;
int16_t Pitch_1, Roll_1;


int pwmpinM1 = 10;
int pwmpinM2 = 8;

int dirpinM1 = 11;
int dirpinM2 = 9;

int btn_up = 40;
int btn_down = 41;

int btn_inclina_M1 = 42;
int btn_inclina_M2 = 43;

int delta = 5;                                 // delta inclinazione
int inclinazione_iniziale = -5 ;               // compensazione per posizionamento giroscopio   
int inclinazione_attuale ; //  variabile dell'inclinazione reale
                                                  
int velocita = 700;
int frequenza_lettura = 1000;
int lettura = 0;

int cm_movimento_pistoni = 1;  // in cm


// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;        // will store last time LED was updated

// constants won't change:
const long interval = 1000;           // interval at which to blink (milliseconds)

bool need_stop = false;

void setup()
{
  pinMode(pwmpinM1, OUTPUT);
  pinMode(pwmpinM2, OUTPUT);
  pinMode(dirpinM1, OUTPUT);
  pinMode(dirpinM2, OUTPUT);
  digitalWrite(pwmpinM1,LOW);
  digitalWrite(dirpinM2,LOW);
  digitalWrite(pwmpinM2,LOW);
  digitalWrite(dirpinM1,LOW);
  pinMode(btn_up, INPUT_PULLUP);
  pinMode(btn_down, INPUT_PULLUP);
  pinMode(btn_inclina_M1, INPUT_PULLUP);
  pinMode(btn_inclina_M2, INPUT_PULLUP);
  Serial.begin(9600);
  
  Serial.println("Enabling IRin");
  irrecv.enableIRIn(); // Start the receiver
  Serial.println("Enabled IRin");
 
  HCMotor.Init();
  HCMotor.attach(0, STEPPER, CLK_PIN, DIR_PIN);

  pinMode(en_pin_pos,OUTPUT);
  pinMode(en_pin_neg,OUTPUT);
  digitalWrite(en_pin_pos,HIGH);
  digitalWrite(en_pin_neg,HIGH);

  // RELE
  pinMode(pinRele1,OUTPUT);
  pinMode(pinRele2,OUTPUT);
  digitalWrite(pinRele1,HIGH);
  digitalWrite(pinRele2,HIGH);

  // IR
  pinMode (positive_pin_IR,OUTPUT);
  pinMode (negative_pin_IR,OUTPUT);
  digitalWrite(positive_pin_IR,HIGH); // don't change
  digitalWrite(negative_pin_IR,LOW);  // don't change
  
  HCMotor.DutyCycle(0, Speed);
  
  init_MPU_1(); // Inizializzazione MPU6050 1
}

void loop()
{
  
  if (irrecv.decode(&results)) {
      Serial.println(results.value);
      unsigned long codice_infrarosso = results.value;
      Serial.println(codice_infrarosso);
      

      if ((codice_infrarosso == right_1) || (codice_infrarosso == right_2)) {    
        digitalWrite(pinRele1,LOW);
        digitalWrite(pinRele2,LOW);
        delay(delay_for_rele); 
        HCMotor.Direction(0, REVERSE);
        HCMotor.Steps(0,my_steps);  
        codice_infrarosso = 0;
        need_stop = true;
      }
      
      if ((codice_infrarosso == left_1) || (codice_infrarosso == left_2)) {       
        digitalWrite(pinRele1,LOW);
        digitalWrite(pinRele2,LOW);
        delay(delay_for_rele); 
        HCMotor.Direction(0, FORWARD);
        HCMotor.Steps(0,my_steps);  
        codice_infrarosso = 0;
        need_stop = true;
      }

      if ((codice_infrarosso == up_1) || (codice_infrarosso == up_2)) {   
        Move_Pistoni(Su,cm_movimento_pistoni);
      }
      if ((codice_infrarosso == down_1) || (codice_infrarosso == down_2)) {   
        Move_Pistoni(Giu,cm_movimento_pistoni);
      }
      if ((codice_infrarosso == tilt_right_1) || (codice_infrarosso == tilt_right_2)) {   
        // TODO control for pistons tilt
      }
      if ((codice_infrarosso == tilt_left_1) || (codice_infrarosso == tilt_left_2)) {   
        // TODO control for pistons tilt
      }

      irrecv.resume(); // Receive the next value
  }
    
    //Serial.println(HCMotor.passi(0));
     
  if ((HCMotor.passi(0) == 0) && (need_stop == true))
    {
      Stop_Rotation();
    }

  if (digitalRead(btn_up) == LOW)
    {
      Move_Pistoni(Su,cm_movimento_pistoni);
    }
    
  if (digitalRead(btn_down) == LOW)
    {
      Move_Pistoni(Giu,cm_movimento_pistoni);
    }
    
  if ((digitalRead(btn_inclina_M2) == LOW) && (inclinazione_attuale < (inclinazione_iniziale+delta)))
    {  
      Inclina_M2();
    }
    
  if ((digitalRead(btn_inclina_M1) == LOW) && (inclinazione_attuale > (inclinazione_iniziale-delta)))
    {
      Inclina_M1();
    }
    
  inclinazione_attuale = Aggiorna_inclinazione_giroscopio();
  //Serial.println(inclinazione_attuale);

}

  int Cicli_to_cm(int cm){
    return cm * 480; // 480 ricavato empiricamente senza carico sul lettino
  }

int Aggiorna_inclinazione_giroscopio(){

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis; 
      FunctionsMPU_1(); // Acquisisco assi AcX, AcY, AcZ.
      int val = FunctionsPitchRoll(AcX_1, AcY_1, AcZ_1);
      Serial.println(val);
      return  val;
    }
    else{
      return inclinazione_attuale;  //  return previous value
    }
}

void Inclina_M1(){
      lettura +=  1;
      if (lettura == frequenza_lettura){
        Serial.println(lettura);
        FunctionsMPU_1(); // Acquisisco assi AcX, AcY, AcZ.
        inclinazione_attuale = FunctionsPitchRoll(AcX_1, AcY_1, AcZ_1);
        Serial.print("inclinazione_attuale M1: "); 
        Serial.println(inclinazione_attuale);
        lettura = 0;
      }
      digitalWrite(pwmpinM2, HIGH);
      delayMicroseconds(velocita); 
      digitalWrite(pwmpinM2, LOW);
      delayMicroseconds(1000 - velocita);
      digitalWrite(dirpinM2,HIGH);
}

void Inclina_M2(){
      lettura +=  1;
      if (lettura == frequenza_lettura){
        Serial.println(lettura);
        FunctionsMPU_1(); // Acquisisco assi AcX, AcY, AcZ.
        inclinazione_attuale = FunctionsPitchRoll(AcX_1, AcY_1, AcZ_1);
        Serial.print("inclinazione_attuale M2: "); 
        Serial.println(inclinazione_attuale);
        lettura = 0;
      }
      digitalWrite(pwmpinM1, HIGH);
      delayMicroseconds(velocita); 
      digitalWrite(pwmpinM1, LOW);
      delayMicroseconds(1000 - velocita);
      digitalWrite(dirpinM1,HIGH);
}

void Stop_Rotation(){
      delay(delay_for_rele);
      digitalWrite(pinRele1,HIGH);
      digitalWrite(pinRele2,HIGH);
      need_stop = false;
}



void Move_Pistoni(int dir, int cm){
  int cicli = Cicli_to_cm(cm);
  for (int x = 0; x < cicli; x++){
      digitalWrite(pwmpinM1, HIGH);
      digitalWrite(pwmpinM2, HIGH);
      delayMicroseconds(velocita); 
      digitalWrite(pwmpinM1, LOW);
      digitalWrite(pwmpinM2, LOW);
      delayMicroseconds(1000 - velocita);
      if (dir == Su){
        digitalWrite(dirpinM1,LOW);
        digitalWrite(dirpinM2,LOW);
      }
      else
      {
        digitalWrite(dirpinM1,HIGH);
        digitalWrite(dirpinM2,HIGH);
      }
  }
}

// inizializzazione giroscopio
void init_MPU_1(){
  Wire.begin();
  Wire.beginTransmission(MPU_1);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  delay(100);
}


//Funzione per il calcolo degli angoli Pitch e Roll
double FunctionsPitchRoll(double A, double B, double C){
  double DatoA, DatoB, Value;
  DatoA = A;
  DatoB = (B*B) + (C*C);
  DatoB = sqrt(DatoB);
  Value = atan2(DatoA, DatoB);
  Value = Value * 180/3.14;
  return (int)Value;
}

//Funzione per l'acquisizione degli assi X,Y,Z del MPU6050
void FunctionsMPU_1(){
  Wire.beginTransmission(MPU_1);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_1,6,true);  // request a total of 6 registers
  AcX_1=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)     
  AcY_1=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ_1=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  // next registers (0x41-0x42) are for temp
  //delay(333);
}
