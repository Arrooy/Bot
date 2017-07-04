/*
#include <PID_v1.h>


double Setpoint, Input, Output;
double kp = 4,ki = 0.4,kd =0.3;

double Setpoint2, Input2, Output2;
double kp2 = 0.5,ki2 = 0.4,kd2 =0.2;


PID Lateral(&Input, &Output, &Setpoint,kp,ki,kd, DIRECT);

PID Frontal(&Input2, &Output2, &Setpoint2,kp2,ki2,kd2, DIRECT);





void configPID(){
  Setpoint = 0;
  Lateral.SetMode(AUTOMATIC);
  Lateral.SetOutputLimits(-90,90);
}

Input = angle_pitch_output;
Lateral.Compute();
Input2 = angle_roll_output;
Frontal.Compute();

*/
