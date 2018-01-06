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


int RECV_PIN = 5;
#include <IRremote.h>
IRrecv irrecv(RECV_PIN);
decode_results results;

/* Include the library */
#include <HCMotor.h>

/* Pins used to drive the motors */
#define DIR_PIN 22 //Connect to drive modules 'direction' input.
#define CLK_PIN 24 //Connect to drive modules 'step' or 'CLK' input.

/* Set the analogue pin the potentiometer will be connected to. */
#define POT_PIN A0

/* Set a dead area at the centre of the pot where it crosses from forward to reverse */
#define DEADZONE 20

/* The analogue pin will return values between 0 and 1024 so divide this up between
   forward and reverse */
#define POT_REV_MIN 0
#define POT_REV_MAX (512 - DEADZONE)
#define POT_FWD_MIN (512 + DEADZONE)
#define POT_FWD_MAX 1024

int en_pin_pos = 30;
int en_pin_neg = 28;

/* Create an instance of the library */
HCMotor HCMotor;



void setup()
{
    Serial.begin(9600);
  // In case the interrupt driver crashes on setup, give a clue
  // to the user what's going on.
  Serial.println("Enabling IRin");
  irrecv.enableIRIn(); // Start the receiver
  Serial.println("Enabled IRin");
  
  /* Initialise the library */
  HCMotor.Init();

  /* Attach motor 0 to digital pins 8 & 9. The first parameter specifies the
     motor number, the second is the motor type, and the third and forth are the
     digital pins that will control the motor */
  HCMotor.attach(0, STEPPER, CLK_PIN, DIR_PIN);

  /* Set the number of steps to continuous so the the motor is always turning whilst
     not int he dead zone*/
  HCMotor.Steps(0,CONTINUOUS);
  pinMode(en_pin_pos,OUTPUT);
  pinMode(en_pin_neg,OUTPUT);
  
}


void loop()
{
  int tempo = 0;
  int Speed = 2;
  digitalWrite(en_pin_pos,HIGH);
  digitalWrite(en_pin_neg,HIGH);
  
  if (irrecv.decode(&results)) {;
    Serial.println(results.value);
    irrecv.resume(); // Receive the next value
  }


  if (results.value == 16722135)
  {
    HCMotor.Direction(0, REVERSE);

  }
  if (results.value == 16713975)
  {
    HCMotor.Direction(0, FORWARD);
  }


  HCMotor.DutyCycle(0, Speed);

}
  
    
