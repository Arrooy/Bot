#include <Adafruit_PWMServoDriver.h>

#define SERVOMIN  167
#define SERVOMAX  599
#define SERVOOFF 4096

typedef struct{
  int absMax;
  int absMin;
  int center;
  int posX:
  int posY;
}Servo;

typedef struct{
  Servo foot;
  Servo knee;
  Servo hip;
}Left;
typedef struct{
  Servo foot;
  Servo knee;
  Servo hip;
}Right;

typedef struct{
  Left left;
  Right right;
}Bot;


Adafruit_PWMServoDriver m = Adafruit_PWMServoDriver(); //Obrim wire 0x40

void Bot_init(int freq) {
  m.begin();
  m.setPWMFreq(freq);
}

void Move_Servo(int a,int b){
  m.setPWM(a, 0, ToServoUnits(b));
}

int ToServoUnits(int a){
  return int(map(a,0,180,SERVOMIN,SERVOMAX));
}

void ShutDown_Servos(){
  for(int i = 0; i <= 5;i++){
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
