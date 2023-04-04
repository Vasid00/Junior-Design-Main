//Author: Gideon Davis, Kaleb Sarlls

#include <Servo.h> //Allows easier control of servo motors

//Constants
//Change these to the actual values
#define SERVO_OPEN   30
#define SERVO_CLOSED 60

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
      //ADD CODE FOR MOVING SERVO
      //close servo
      if(servoState) 
      {
        gripper.write(SERVO_CLOSED);
        servoState = false;
      }
      //open servo
      else
      {
        gripper.write(SERVO_OPEN);
        servoState = true;
      }
    }
};
