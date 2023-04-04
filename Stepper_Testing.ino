//Author: Gideon Davis, Kaleb Sarlls

#include <AccelStepper.h> //Allows acceleration in bipolar steppers
#include <MultiStepper.h> //Allows the use of multiple steppers

//Desired Y RPM = 187.50
//Desired Z RPM = 266.67

//Y test
//   Pins:
//      - Enable = N/A
//      - Step = 5
//      - Direction = 4
//

//Z test
//   Pins:
//      - Enable = N/A
//      - Step = 3
//      - Direction = 2
//


#define stepPinZ 3
#define dirPinZ  2


#define stepPinY 5
#define dirPinY  4


#define stopPin 50

bool notStoppedY = true, notStoppedZ = true;
int buttonState = 0;
int clk = 0;

AccelStepper z_testMotor(1, stepPinZ, dirPinZ);
AccelStepper y_testMotor(1, stepPinY, dirPinY);

void setup(){
  //Serial monitoring
  Serial.begin(9600);
  
  //Setting max speed for both to 500 for now
  z_testMotor.setMaxSpeed(500);
  y_testMotor.setMaxSpeed(500);
  
  //Emergency stop button
  pinMode(stopPin, INPUT);
  
  //Setting the step/dir pins for both motors
  pinMode(stepPinZ, OUTPUT); //Z
  pinMode(dirPinZ, OUTPUT);  //Z
  pinMode(stepPinY, OUTPUT); //Y
  pinMode(dirPinY, OUTPUT);  //Y
};

void loop(){
  //read the value of the emergency stop pin
  buttonState = digitalRead(stopPin);
  
  //Emergency stop clause
  if(buttonState==HIGH)
  {
    //Emergency stop serial monitor
    if(notStopped){
      //Print to serial monitor
      Serial.println();
      Serial.println("System Stopped.");
    }
    notStopped = false; //change bool flag
  }
  //If emergency stop has not been pressed
  
  //Two types of stepper control, comment out the one you want to use
  if(notStopped)
  {
    /* Library-based stepper control for z-motor
    z_testMotor.setCurrentPosition(0);
    z_testMotor.setSpeed(200);
    z_testMotor.runSpeed();
    if(z_testMotor.currentPosition() >= 400)
    {
      delay(10);
      z_testMotor.setCurrentPosition(0);
    }
    */
    
    /* Library-based stepper control for y-motor
    y_testMotor.setCurrentPosition(0);
    y_testMotor.setSpeed(200);
    y_testMotor.runSpeed();
    if(y_testMotor.currentPosition() >= 400)
    {
      delay(10);
      y_testMotor.setCurrentPosition(0);
    }
    */
    
    /* Hardcoded stepper control for z-motor
    digitalWrite(dirPinZ, HIGH);

    for(int i=0; i<200; i++){
      digitalWrite(stepPinZ, HIGH);
      delayMicroseconds(2000);
      digitalWrite(stepPinZ, LOW);
      delayMicroseconds(2000);
    }
    */
    
    /* Hardcoded stepper control for y-motor
    digitalWrite(dirPinY, HIGH);

    for(int i=0; i<200; i++){
      digitalWrite(stepPinY, HIGH);
      delayMicroseconds(2000);
      digitalWrite(stepPinY, LOW);
      delayMicroseconds(2000);
    }
    */
  }
  delay(1000);
};
