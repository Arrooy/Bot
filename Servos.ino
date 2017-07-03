#include <Adafruit_PWMServoDriver.h>


#define SERVOMIN  167
#define SERVOMAX  599
#define SERVOOFF 4096




Adafruit_PWMServoDriver m = Adafruit_PWMServoDriver(); //Obrim wire 0x40

  unsigned long lastApagat = 0;
  unsigned long lastMove = 0;
  int w8 = 50;

 void Bot(int freq) {
    m.begin();
    m.setPWMFreq(freq);
    /*for(int i =  0;i<6;i++){
      Bot_move(i,IniciPos[i]);
      delay(w8);
    }*/
    Bot_apagarServo(1000,-1);
  }

  void Bot_update(int m0,int m1,int m2,int m3,int m4,int m5){

     Bot_move(0,map(m0,0,180,165,15));//165-15 graus
     delay(w8);
     Bot_move(1,m1);//180-0 graus
     delay(w8);
     Bot_move(2,map(m2,0,180,20,160));//160-20 graus
     delay(w8);
     Bot_move(3,map(m3,0,180,180,0));
     delay(w8);
     Bot_move(4,map(m4,0,180,130,0));//130-0 inversa que 1
     delay(w8);
     Bot_move(5,map(m5,0,180,0,125));//125-0
     delay(w8);
  }
  void Bot_move(int a,int b){
    m.setPWM(a, 0, b);
  }

  void Bot_still(int timer){
    for(int i = 50;i<130;i++){
      Bot_move(5,map(i,0,180,0,130));
      Bot_move(0,map(i,0,180,25,140));
      delay(timer);
    }
    for(int i = 130;i>50;i--){
      Bot_move(5,map(i,0,180,0,130));
      Bot_move(0,map(i,0,180,25,140));
      delay(timer);
    }
  }

  int Bot_d(int a){
    return int(map(a,0,180,SERVOMIN,SERVOMAX));
  }

  void Bot_apagarServo(int time,int servo){

    if(millis() - lastApagat >= time){
      if(servo == -1){
        for(int i = 0; i <= 5;i++){
          m.setPWM(i, 0, 4096);
        }
      }else{
        m.setPWM(servo,0,4096);
      }
      lastApagat = millis();
    }else if(DEBUG == 1){
      Serial.print("Faltan ");Serial.print(time - (millis() - lastApagat));Serial.println(" ms");
    }
  }
