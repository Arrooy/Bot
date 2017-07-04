typedef struct{
  int absMax;
  int absMin;
  int center;
  int posX;
  int posY;
  int pin;
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
