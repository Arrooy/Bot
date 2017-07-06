#include <Wire.h>
#include "data.h"



#define VBATPIN A9
#define ControllBattery 0

unsigned long CLOCK  = 4000;   //4000us = 250Hz

unsigned long currentTime = 0;

float Battery = 0.0;

Bot bot;

unsigned long temporizadorDreta = 0,temporizadorRecuperasio= 0;

int oneTime = 1;
void setup() {

  Serial.begin(57600);

  Wire.begin();                                                                  //Start I2C as master

  configPID();

  Bot_init(50);

  configPID();

  setup_mpu_6050_registers();                                                 //Setup the registers of the MPU-6050 (500dfs / +/-8g) and start the gyro
  currentTime = micros();                                                      //Reset the loop timer
  temporizadorDreta = millis();
  temporizadorRecuperasio = millis();
}

void loop() {
  if(ControllBattery)
    Battery = MeasureBat();

  CalcGyro(1);

  if(millis() - temporizadorDreta >= 5000){

    InclinarseDreta(oneTime);
    oneTime = 0;
    if(millis() - temporizadorRecuperasio >= 200){
      Move_Servo(bot.left.foot.pin,bot.right.foot.center);
    }
    //-31 en pitch
  }else{
    temporizadorRecuperasio = millis();
    if(PID_compute()){
      initialPositionFoot();
    }
  }

//PID_adjustSetpoint();

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
