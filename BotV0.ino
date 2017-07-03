#include <Wire.h>

#define VBATPIN A9

unsigned long CLOCK  = 9000;   //4000us = 250Hz

float Battery = 0.0;

unsigned long Zone = 0;
unsigned long helper = 0;
int gameConstant = 0;

void setup() {

  Serial.begin(57600);

  Wire.begin();                                                                  //Start I2C as master

  configPID();

  Bot(50);

  setup_mpu_6050_registers();                                                 //Setup the registers of the MPU-6050 (500dfs / +/-8g) and start the gyro
  currentTime = micros();                                                      //Reset the loop timer
}

void loop() {

  if(ControllBattery)
    Battery = MeasureBat();

  CalcGyro();
  reAdjustTimer();
}

void reAdjustTimer() {

  Zone = micros() - currentTime;
  helper = currentTime;

  //Si tenim gran differencia, actualitzar el clock i despres calculateConstantsGyro();

  currentTime = helper;
  while (micros() - currentTime < CLOCK);
  currentTime = micros();
}


void MeasureBat(){
  int Bat = analogRead(VBATPIN);
  Bat *= 6.6;  // Multiply by 3.3V, our reference voltage and we divided by 2, so multiply back
  Bat /= 1024; // convert to voltage
  return Bat;
}
