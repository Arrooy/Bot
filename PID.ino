#include <PID_v1.h>


double Setpoint, Input, Output;
double kp = 4,ki = 0.4,kd =0.3;


PID Lateral(&Input, &Output, &Setpoint,kp,ki,kd, DIRECT);

void configPID(){
  Setpoint = bot.left.foot.center;
  Lateral.SetMode(AUTOMATIC);
  Lateral.SetOutputLimits(bot.left.foot.AbsMin,bot.left.foot.AbsMax);
}
