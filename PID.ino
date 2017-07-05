#include <PID_v1.h>

double Setpoint = 0, Input, Output;

double kp = 4,ki = 0.4,kd =0.3;
/*
double SetpointF = 0, InputF, OutputF;

double kpF = 4,kiF = 0.4,kdF =0.3;
*/
long bigData = 0;

PID Lateral(&Input, &Output, &Setpoint,kp,ki,kd, DIRECT);

//PID Frontal(&InputF, &OutputF, &SetpointF,kpF,kiF,kdF, DIRECT);

void configPID(){

  Lateral.SetMode(AUTOMATIC);
  Lateral.SetOutputLimits(-90,90);
  /*
  Frontal.SetMode(AUTOMATIC);
  Frontal.SetOutputLimits(-90,90);
  */

}
int PID_compute(){
  int OnlyOneReturn = 0;
  if(angle_pitch_output > 3 || angle_pitch_output < -3){
    Input = angle_pitch_output;
    Lateral.Compute();
    Serial.println(Output);
    PID_Corrections(int(Output));
    OnlyOneReturn = 0;
  }else{
    OnlyOneReturn = 1;
  }
}/*

void PID_adjustSetpoint(char data){
  bigData *= 10;
  if(data == m){
    Setpoint = data
  }else if(data == z){
    Setpoint = 0;
    bigData = 0;
  }


}*/
