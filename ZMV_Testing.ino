//Author: Kaleb Sarlls

#include <AccelStepper.h>       //Allows acceleration in bipolar steppers
#include <MultiStepper.h>       //Allows the use of multiple steppers
#include <Adafruit_VL6180X.h>   // Z sensor library
#include <Wire.h>
#define stepPinZ 3
#define dirPinZ  2
#define stopPin 50

//Desired Z RPM = 266.67
//Z test
//   Pins:
//      - Enable = N/A
//      - Step = 3
//      - Direction = 2

bool notStopped = true;
bool readyToGrabCase = true;
int buttonState = 0;
int clk = 0;
uint8_t range; //tofZ setup code
uint8_t status;
const int STEPS_PER_MM = 27;    // constant for steps per milimeter
const int MOTOR_SPEED = 1000;   //If torque is an issue, raise speed by increments of 250
double distToCase = range - (constant for how much space we need to leave);

AccelStepper zMotor(1, stepPinZ, dirPinZ);
Adafruit_VL6180X tofZ = Adafruit_VL6180X();


void setup(){
  Serial.begin(9600);           //Serial monitoring
  zMotor.setMaxSpeed(500);      //Setting max speed for both to 500 for now
  
  pinMode(stopPin, INPUT);      //Emergency stop button
  
  pinMode(stepPinZ, OUTPUT);    //Setting the step/dir pins
  pinMode(dirPinZ, OUTPUT);  
  
  while (!Serial)               //makes sure TOF is connected
  {
  delay(1);
  }
  if (!tofZ.begin())
  {
    Serial.println(F("Failed to find VL6180X (ZMV)"));
    while(1);
  }
  zMotor.setCurrentPosition(0);
};

void loop() {
  range = tofZ.readRange();
  status = tofZ.readRangeStatus();

  if (status == VL6180X_ERROR_NONE){          //detect and give Z distance
    Serial.print(""Distance in Z: ");
    Serial.println(range);
  }

  buttonState = digitalRead(stopPin);          //read the value of the emergency stop pin
  
  if(buttonState==HIGH)                 //Emergency stop clause
  {
    if(notStopped){                     //Emergency stop serial monitor
      Serial.println();                 //Print to serial monitor
      Serial.println("System Stopped.");
    }
    notStopped = false;                 //change bool flag
  }
  
  //FIXME: implement stepper to work with tof
  if(notStopped)
  {
    
      // This would need a boolean for if the mechanism is ready to grab the case.
      // This boolean would be supplied from control monitoring of the entire system
      if(readyToGrabCase){ 
        //Hardcoded example for stepper control:
        
        //Lower to grab the case
        digitalWrite(dirPinZ, LOW);
        for(int i=0; i<distToCase * STEPS_PER_MM; i++){
          digitalWrite(stepPinZ, HIGH);
          delayMicroseconds(MOTOR_SPEED);
          digitalWrite(stepPinZ, LOW);
          delayMicroseconds(MOTOR_SPEED);
        }
        delay(10);
        
        //Activate servo motor here
        //NEEDS SERVO CODE
        delay(1000);
        readyToGrabCase = false;
        
        //raise case
        digitalWrite(dirPinZ, HIGH);
        for(int i=0; i<distToCase * STEPS_PER_MM; i++){
          digitalWrite(stepPinZ, HIGH);
          delayMicroseconds(MOTOR_SPEED);
          digitalWrite(stepPinZ, LOW);
          delayMicroseconds(MOTOR_SPEED);
        }
      }
  }
};
