//Author: Gideon Davis, Kaleb Sarlls

#include <Servo.h> //Allows easier control of servo motors

//Constants
//Change these to the actual values
#define SERVO_OPEN   0     //ask chris
#define SERVO_CLOSED 6

//Global setup
Servo gripper; //Class declaration

int servoSignal = 7; //Pin connection for servo
int servoAngle = 0; //Current angle of servo

bool moveServo, servoState; //servoState true = open, false = closed

void  setup(){
  gripper.attach(servoSignal); //connect servo
  servoState = true;
};

void loop(){
  //Servo
    if(moveServo)
    {
      if(servoState) 
      {
        gripper.write(SERVO_CLOSED);
        servoState = false;
      }
      else
      {
        gripper.write(SERVO_OPEN);
        servoState = true;
      }
      moveServo = false;
      delay(15);
    }
};
