#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <Adafruit_PWMServoDriver.h>

#define LINTERRUPT 0
#define RINTERRUPT 1
#define LINPUT 2
#define RINPUT 3

#define LMOTOR 1
#define RMOTOR 4

int lPin = 2;
int rPin = 3;

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_DCMotor * lMotor = AFMS.getMotor(LMOTOR);
Adafruit_DCMotor * rMotor = AFMS.getMotor(RMOTOR);

int deadspace = 20;
volatile unsigned int lTimer = 0;
volatile unsigned int rTimer = 0;
volatile int lInitPos = 1460;
volatile int rInitPos = 1460;
volatile int lPos = 0;
volatile int rPos = 0;
volatile bool lReady = false;
volatile bool rReady = false;

int pos = 0;
int mappedPos = 0;

void setup() {
  AFMS.begin();
  lMotor->setSpeed(0);
  rMotor->setSpeed(0);

  attachInterrupt(LINTERRUPT, left_interrupt, CHANGE);
  attachInterrupt(RINTERRUPT, right_interrupt, CHANGE);

}

void loop() {
  if(lReady){
      setSpeed(lPos, lInitPos, lMotor);
      lReady = false;
  }

  if(rReady){
    setSpeed(rPos, rInitPos, rMotor);
    rReady = false;
  }
}

void left_interrupt(){

  if(digitalRead(LINPUT) == HIGH){
    lTimer = micros();
  }else{
    lPos = (int)(micros()-lTimer);
    lReady = true;
  }
}

void right_interrupt(){

  if(digitalRead(RINPUT) == HIGH){
    rTimer = micros();
  }else{
    rPos = (int)(micros()-rTimer);
    rReady = true;
  }
}

void setSpeed(int pos, int initPos, Adafruit_DCMotor * motor){

  if (pos < initPos + deadspace && pos > initPos - deadspace){
    motor->setSpeed(0);
  }
  else{
    pos = pos - initPos;
    mappedPos = map(abs(pos), 0, 330, 0, 255);

    if(mappedPos > 255){
      motor->setSpeed(255);
    }else{
      motor->setSpeed(mappedPos);
    }

    if (pos > 0){
      motor->run(FORWARD);
    }
    else{
      motor->run(BACKWARD);
    }
    
  }
}

