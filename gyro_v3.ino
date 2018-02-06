// http://www.giuseppecaccavale.it/
// Giuseppe Caccavale  modified by Davide Caminati

#include <SPI.h>
#include <Wire.h>
#define MPU_2 0x68  // I2C address of the MPU-6050
#define MPU_1 0x69  // I2C address of the MPU-6050

double AcX_1,AcY_1,AcZ_1;
double AcX_2,AcY_2,AcZ_2;
int Pitch_1, Roll_1;
int Pitch_2, Roll_2;

void setup(){
  Serial.begin(9600);
  init_MPU_1(); // Inizializzazione MPU6050 1
  init_MPU_2(); // Inizializzazione MPU6050 2 
}
 
void loop()
{
  FunctionsMPU_1(); // Acquisisco assi AcX, AcY, AcZ.
  FunctionsMPU_2(); 
    
  Roll_1 = FunctionsPitchRoll(AcX_1, AcY_1, AcZ_1);   //Calcolo angolo Roll
  
  Roll_2 = FunctionsPitchRoll(AcX_2, AcY_2, AcZ_2);   //Calcolo angolo Roll

  Serial.print("Roll_1: "); Serial.print(Roll_1);
  Serial.print("\t");
  Serial.print("Roll_2: "); Serial.print(Roll_2);
  Serial.print("\n");
}

void init_MPU_1(){
  Wire.begin();
  Wire.beginTransmission(MPU_1);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  delay(1000);
}
void init_MPU_2(){
  Wire.begin();
  Wire.beginTransmission(MPU_2);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  delay(1000);
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
}
//Funzione per l'acquisizione degli assi X,Y,Z del MPU6050
void FunctionsMPU_2(){
  Wire.beginTransmission(MPU_2);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_2,6,true);  // request a total of 6 registers
  AcX_2=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)     
  AcY_2=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ_2=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  // next registers (0x41-0x42) are for temp
}
