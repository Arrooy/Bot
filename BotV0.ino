#include <Wire.h>
#include "data.h"

#define VBATPIN A9
#define ControllBattery 0

unsigned long CLOCK  = 4000;   //4000us = 250Hz

unsigned long currentTime = 0;

float Battery = 0.0;

Bot bot;

void setup() {

  Serial.begin(57600);

  Wire.begin();                                                                  //Start I2C as master
  pinMode(INPUT,A0);
  //configPID();

  Bot_init(50);

  setup_mpu_6050_registers();                                                 //Setup the registers of the MPU-6050 (500dfs / +/-8g) and start the gyro
  currentTime = micros();                                                      //Reset the loop timer
}


void loop() {

  if(ControllBattery)
    Battery = MeasureBat();

  CalcGyro(0);

  Input = angle_pitch_output;
  Lateral.Compute();

  reAdjustTimer();
}

void reAdjustTimer() {

  CLOCK = micros() - currentTime;
  calculateConstantsGyro();

  currentTime = micros();
}


float MeasureBat(){
  float Bat = analogRead(VBATPIN);
  Bat *= 6.6;  // Multiply by 3.3V, our reference voltage and we divided by 2, so multiply back
  Bat /= 1024; // convert to voltage
  return Bat;
}
