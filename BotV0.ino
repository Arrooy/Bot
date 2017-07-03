#include <Wire.h>
#include <PID_v1.h>

#define VBATPIN A9



float calcRaw;
float useRaw;

float ToRadians;

//0 : Gyro
//1 :  Serial
//2 : PID
unsigned long CLOCK  = 9000;   //4000us = 250Hz
#define PI 3.14159265358979323846264338327950288
#define DEBUG 0
#define MODE 0
#define ControllBattery 0
#define RC 0
  #define WHAITFORPULSE 32000


double Setpoint, Input, Output;
double kp = 4,ki = 0.4,kd =0.3;
double kp2 = 0.5,ki2 = 0.4,kd2 =0.2;
PID Lateral(&Input, &Output, &Setpoint,kp,ki,kd, DIRECT);
double Setpoint2, Input2, Output2;
PID Frontal(&Input2, &Output2, &Setpoint2,kp2,ki2,kd2, DIRECT);



//Declaring some global variables
int gyro_x, gyro_y, gyro_z;
long acc_x, acc_y, acc_z, acc_total_vector;
int temperature;
long gyro_x_cal, gyro_y_cal, gyro_z_cal;
unsigned long loop_timer = 0;
float angle_pitch, angle_roll;
boolean set_gyro_angles;
float angle_roll_acc, angle_pitch_acc;
float angle_pitch_output, angle_roll_output;




String val0;
String val1;
String val2;
String val3;
String val4;
String val5;

int m0 = 0;
int m1 = 0;
int m2 = 0;
int m3 = 0;
int m4 = 0;
int m5 = 0;


int IniciPos[6];

int ch[7];

float Batteria = 0.0;

int timeCalibrationSetup = 0;
int numberOftimesCalibrated = 2000;

int revisor = 500;
unsigned long Zone = 0;

unsigned long helper = 0;

int gameConstant = 0;

unsigned long servoLast[10];

void setup() {
  for(int i = 0 ;i <= 5;i++)IniciPos[i] = 90;
                                                                                //[0][Mes gran dreta]
                                                                                //[1][Mes gran enrere]
                                                                                //[2][Mes gran enrere]
                                                                                //[3][Mes gran devant]
                                                                                //[4][Mes gran endevant]
                                                                                //[5][Mes gran dreta]

  //Rc inputs
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(10, INPUT);
  pinMode(11, INPUT);
  pinMode(12, INPUT);
  pinMode(13, INPUT);

  Serial.begin(57600);
  Wire.begin();                                                                  //Start I2C as master

  Bot(50);
  setup_mpu_6050_registers();                                                 //Setup the registers of the MPU-6050 (500dfs / +/-8g) and start the gyro
  loop_timer = micros();                                                      //Reset the loop timer

  Setpoint = 0;
  Lateral.SetMode(AUTOMATIC);
  Lateral.SetOutputLimits(-90,90);
  Setpoint2 = 10;
  Frontal.SetMode(AUTOMATIC);
  Frontal.SetOutputLimits(-100,80);
                                                                                //Lateral.SetSampleTime();//milliseconds


  for(int i = 1 ;i < 7;i++)ch[i] = 0;
}

int i = 50;
unsigned long medision = 0;unsigned long Donete = 0;

void loop() {

  if(ControllBattery){
    Batteria = analogRead(VBATPIN);
    Batteria *= 6.6;  // Multiply by 3.3V, our reference voltage and we divided by 2, so multiply back
    Batteria /= 1024; // convert to voltage
    if(DEBUG){Serial.print("VBat: " ); Serial.println(Batteria);}
  }

  if(RC){

    ch[1] = pulseIn(5 , HIGH, WHAITFORPULSE);
    ch[2] = pulseIn(6 , HIGH, WHAITFORPULSE);
    ch[3] = pulseIn(10, HIGH, WHAITFORPULSE);
    ch[4] = pulseIn(11, HIGH, WHAITFORPULSE);
    ch[5] = pulseIn(12, HIGH, WHAITFORPULSE);
    ch[6] = pulseIn(13, HIGH, WHAITFORPULSE);

  }

                                                                                /*if(MODE == 1){
                                                                                  if (Serial.available()){
                                                                                    Serialel();
                                                                                  }
                                                                                }else */
  if(MODE == 0){

    CalcGyro();

    Input = angle_pitch_output;
    Lateral.Compute();
    Input2 = angle_roll_output;
    Frontal.Compute();


    if(angle_pitch_output < 35 && angle_pitch_output > -35 && angle_roll_output < 35 && angle_roll_output > -35){



      Serial.print("  I1 ");
      Serial.print(Input);
      Serial.print("  O1 ");
      Serial.print(Output);
      Serial.print("  Signal1 ");
      Serial.println(90 + Output);
      Serial.print("  I2 ");
      Serial.print(Input2);
      Serial.print("  O2 ");
      Serial.print(Output2);
      Serial.print("  Signal2 ");
      Serial.println(90 - Output2);

      Bot_move(0,90 + Output);
      Bot_move(5,90 + Output);



      Bot_move(2,90 + Output2);

      Bot_move(3,90 - Output2);
      

    }else{
       Bot_apagarServo(5000,-1);
    }

  }else if(MODE == 2){ //Dance
    Bot_still(rand()%20);

  }else if(MODE == 3){//RC Controller
    if(ch[1] == 0){
      Bot_still(rand()%20);
    }else{

    }
  }

  Zone = micros() - loop_timer;
  helper = loop_timer;
  if(gameConstant < 10 && RC == 1){//Limite para no petar el programa en rc
    if(dist((int)CLOCK,(int)Zone,1500)){                                                     //Si tenim una diferencia de 3000us. El giroscopi no sera tant precis.
      CLOCK = Zone;                                                               //Al detectar la diferencia, corregim rapidament les constants de les equacions.
      calculateConstantsGyro();
      gameConstant ++;
    }
  }
  loop_timer = helper;
  while (micros() - loop_timer < CLOCK);                                         //Wait until the loop_timer reaches 4000us (250Hz) before starting the next loop
  loop_timer = micros();                                                        //Reset the loop timer
}

void setup_mpu_6050_registers(){
  //Activate the MPU-6050
  Wire.beginTransmission(0x68);                                                 //Start communicating with the MPU-6050
  Wire.write(0x6B);                                                             //Send the requested starting register
  Wire.write(0x00);                                                             //Set the requested starting register
  Wire.endTransmission();                                                       //End the transmission
  //Configure the accelerometer (+/-8g)
  Wire.beginTransmission(0x68);                                                 //Start communicating with the MPU-6050
  Wire.write(0x1C);                                                             //Send the requested starting register
  Wire.write(0x10);                                                              //Set the requested starting register
  Wire.endTransmission();                                                        //End the transmission
  //Configure the gyro (500dps full scale)
  Wire.beginTransmission(0x68);                                                  //Start communicating with the MPU-6050
  Wire.write(0x1B);                                                              //Send the requested starting register
  Wire.write(0x08);                                                              //Set the requested starting register
  Wire.endTransmission();                                                        //End the transmission

  //Calibrating gyro
  timeCalibrationSetup = CLOCK/1000;

  for (int cal_int = 0; cal_int < numberOftimesCalibrated ; cal_int ++){                  //Run this code 2000 times
    read_mpu_6050_data();                                                       //Read the raw acc and gyro data from the MPU-6050
    gyro_x_cal += gyro_x;                                                       //Add the gyro x-axis offset to the gyro_x_cal variable
    gyro_y_cal += gyro_y;                                                       //Add the gyro y-axis offset to the gyro_y_cal variable
    gyro_z_cal += gyro_z;                                                        //Add the gyro z-axis offset to the gyro_z_cal variable
    delay(timeCalibrationSetup);
  }
  gyro_x_cal /= numberOftimesCalibrated;                                                  //Divide the gyro_x_cal variable by 2000 to get the avarage offset
  gyro_y_cal /= numberOftimesCalibrated;                                                  //Divide the gyro_y_cal variable by 2000 to get the avarage offset
  gyro_z_cal /= numberOftimesCalibrated;                                                  //Divide the gyro_z_cal variable by 2000 to get the avarage offset

  calculateConstantsGyro();
}


void CalcGyro(){
  read_mpu_6050_data();                                                //Read the raw acc and gyro data from the MPU-6050
  gyro_x -= gyro_x_cal;                                                //Subtract the offset calibration value from the raw gyro_x value
  gyro_y -= gyro_y_cal;                                                //Subtract the offset calibration value from the raw gyro_y value
  gyro_z -= gyro_z_cal;                                                //Subtract the offset calibration value from the raw gyro_z value

  //Gyro angle calculations
  //0.0000611 = 1 / (250Hz * 65.5)

  angle_pitch += (float)gyro_x * calcRaw;                                   //Calculate the traveled pitch angle and add this to the angle_pitch variable
  angle_roll += (float)gyro_y * calcRaw;                                    //Calculate the traveled roll angle and add this to the angle_roll variable

  //0.1066 = 0.0000611 * (3.142(PI) / 180degr)   The Arduino sin function is in radians
  angle_pitch += angle_roll * sin(gyro_z * useRaw);               //If the IMU has yawed transfer the roll angle to the pitch angel
  angle_roll -= angle_pitch * sin(gyro_z * useRaw);               //If the IMU has yawed transfer the pitch angle to the roll angel

  //Accelerometer angle calculations
  acc_total_vector = sqrt((acc_x*acc_x)+(acc_y*acc_y)+(acc_z*acc_z));  //Calculate the total accelerometer vector
  //57.296 = 1 / (3.142 / 180) The Arduino asin function is in radians
  angle_pitch_acc = asin((float)acc_y/acc_total_vector)*  ToRadians;       //Calculate the pitch angle
  angle_roll_acc = asin((float)acc_x/acc_total_vector)* -ToRadians;       //Calculate the roll angle

  //Place the MPU-6050 spirit level and note the values in the following two lines for calibration
  angle_pitch_acc -= 0.0;                                              //Accelerometer calibration value for pitch
  angle_roll_acc -= 0.0;                                               //Accelerometer calibration value for roll

  if(set_gyro_angles){                                                 //If the IMU is already started
    angle_pitch = angle_pitch * 0.9996 + angle_pitch_acc * 0.0004;     //Correct the drift of the gyro pitch angle with the accelerometer pitch angle
    angle_roll = angle_roll * 0.9996 + angle_roll_acc * 0.0004;        //Correct the drift of the gyro roll angle with the accelerometer roll angle
  }
  else{                                                                //At first start
    angle_pitch = angle_pitch_acc;                                     //Set the gyro pitch angle equal to the accelerometer pitch angle
    angle_roll = angle_roll_acc;                                       //Set the gyro roll angle equal to the accelerometer roll angle
    set_gyro_angles = true;                                            //Set the IMU started flag
  }

  //To dampen the pitch and roll angles a complementary filter is used,
  angle_pitch_output = angle_pitch_output * 0.9 + angle_pitch * 0.1;   //Take 90% of the output pitch value and add 10% of the raw pitch value
  angle_roll_output = angle_roll_output * 0.9 + angle_roll * 0.1;      //Take 90% of the output roll value and add 10% of the raw roll value
  }

void read_mpu_6050_data(){                                             //Subroutine for reading the raw gyro and accelerometer data
  Wire.beginTransmission(0x68);                                        //Start communicating with the MPU-6050
  Wire.write(0x3B);                                                    //Send the requested starting register
  Wire.endTransmission();                                              //End the transmission
  Wire.requestFrom(0x68,14);                                           //Request 14 bytes from the MPU-6050
  while(Wire.available() < 14);                                        //Wait until all the bytes are received
  acc_x = Wire.read()<<8|Wire.read();                                  //Add the low and high byte to the acc_x variable
  acc_y = Wire.read()<<8|Wire.read();                                  //Add the low and high byte to the acc_y variable
  acc_z = Wire.read()<<8|Wire.read();                                  //Add the low and high byte to the acc_z variable
  temperature = Wire.read()<<8|Wire.read();                            //Add the low and high byte to the temperature variable
  gyro_x = Wire.read()<<8|Wire.read();                                 //Add the low and high byte to the gyro_x variable
  gyro_y = Wire.read()<<8|Wire.read();                                 //Add the low and high byte to the gyro_y variable
  gyro_z = Wire.read()<<8|Wire.read();                                 //Add the low and high byte to the gyro_z variable

  }

void Serialel(){

  // If data is available to read,

  /*val0 = Serial.readStringUntil('a');
  val1 = Serial.readStringUntil('b');
  val2 = Serial.readStringUntil('c');
  val3 = Serial.readStringUntil('d');
  val4 = Serial.readStringUntil('e');
  val5 = Serial.readStringUntil('f');


  m0 = val0.toInt();
  m1 = val1.toInt();
  m2 = val2.toInt();
  m3 = val3.toInt();
  m4 = val4.toInt();
  m5 = val5.toInt();*/
  }

void calculateConstantsGyro(){

  Donete = micros();
  ToRadians = 1.0/(PI/180);

  calcRaw = 1.0 / ((1.0/(CLOCK / 1000000.0)) * 65.5);

  useRaw = calcRaw * (1.0/ToRadians);
  /*Serial.print("Time Calculating : ");
  Serial.println(micros() - Donete);*/

}



int dist(int num1,int num2,int comparator){
  int res = abs(num1 - num2);
/*  Serial.print("Num1 ");
  Serial.print(num1);
  Serial.print(" Num 2 ");
  Serial.print(num2);
  Serial.print(" Res " );
  Serial.print(res);
  Serial.print(" Compara " );
  Serial.println(comparator);*/
  if(res > comparator && num2>0 )
    return 1;
  else
    return 0;
}
