#include <Adafruit_PWMServoDriver.h>
/*
#define SERVOMIN  167
#define SERVOMAX  599*/
#define SERVOMIN  93
#define SERVOMAX  409
#define SERVOOFF 4096







Adafruit_PWMServoDriver m = Adafruit_PWMServoDriver(); //Obrim wire 0x40


void Bot_init(int freq) {
  m.begin();
  m.setPWMFreq(freq);
  fillVars();
  InitialPosition();
  ShutDown_Servos();
}

void Move_Servo(int a,int b){
  //m.setPWM(a, 0, ToServoUnits(b));
  m.setPWM(a, 0, b);
}
void Move_Servox(int a,int b){
  //m.setPWM(a, 0, ToServoUnits(b));
  m.setPWM(a, b, 0);
}

int ToServoUnits(int a){
  return int(map(a,0,180,SERVOMIN,SERVOMAX));
}

void ShutDown_Servos(){
  for(int i = 1; i <= 6;i++){
    m.setPWM(i, 0, 4096);
  }
}

void Dance(int timer){
  for(int i = 50;i<130;i++){
    Move_Servo(5,map(i,0,180,0,130));
    Move_Servo(0,map(i,0,180,25,140));
    delay(timer);
  }
  for(int i = 130;i>50;i--){
    Move_Servo(5,map(i,0,180,0,130));
    Move_Servo(0,map(i,0,180,25,140));
    delay(timer);
  }
}
void fillVars(){
  bot.left.foot.center = 250;
  bot.left.foot.absMin = 100;
  bot.left.foot.absMax = 400;
  bot.left.foot.pin = 6;
  bot.left.foot.posY = 0;
  bot.left.foot.posX = 0;

  bot.left.knee.center = 285;
  bot.left.knee.absMin = 115;
  bot.left.knee.absMax = 400;
  bot.left.knee.pin = 5;
  bot.left.knee.posY = 1;
  bot.left.knee.posX = 0;

  bot.left.hip.center = 300;
  bot.left.hip.absMin = 230;
  bot.left.hip.absMax = 480;
  bot.left.hip.pin = 4;
  bot.left.hip.posY = 2;
  bot.left.hip.posX = 0;


  bot.right.foot.center = 250;
  bot.right.foot.absMin = 100;
  bot.right.foot.absMax = 400;
  bot.right.foot.pin = 1;
  bot.right.foot.posY = 0;
  bot.right.foot.posX = 0;

  bot.right.knee.center = 210;
  bot.right.knee.absMin = 90;
  bot.right.knee.absMax = 400;
  bot.right.knee.pin = 2;
  bot.right.knee.posY = 1;
  bot.right.knee.posX = 0;

  bot.right.hip.center = 380;
  bot.right.hip.absMin = 210;
  bot.right.hip.absMax = 460;
  bot.right.hip.pin = 3;
  bot.right.hip.posY = 2;
  bot.right.hip.posX = 0;
}


void InitialPosition(){
  Move_Servo(bot.left.foot.pin,bot.left.foot.center);
  Move_Servo(bot.left.knee.pin,bot.left.knee.center);
  Move_Servo(bot.left.hip.pin,bot.left.hip.center);
  Move_Servo(bot.right.foot.pin,bot.right.foot.center);
  Move_Servo(bot.right.knee.pin,bot.right.knee.center);
  Move_Servo(bot.right.hip.pin,bot.right.hip.center);
}
