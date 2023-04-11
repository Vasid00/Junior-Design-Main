//Author: Kaleb Sarlls
//Updated: 4/11/23
//By: John Westbrook

#include <AccelStepper.h>       //Allows acceleration in bipolar steppers
#include <MultiStepper.h>       //Allows the use of multiple steppers
// Tried including Adafruit library
#include <Adafruit_VL53L0X.h>
// Extras? Are they needed ///////////////////
#include <vl53l0x_api.h>
#include <vl53l0x_api_calibration.h>
#include <vl53l0x_api_core.h>
#include <vl53l0x_api_ranging.h>
#include <vl53l0x_api_strings.h>
#include <vl53l0x_def.h>
#include <vl53l0x_device.h>
#include <vl53l0x_i2c_platform.h>
#include <vl53l0x_interrupt_threshold_settings.h>
#include <vl53l0x_platform.h>
#include <vl53l0x_platform_log.h>
#include <vl53l0x_tuning.h>
#include <vl53l0x_types.h>
///////////////////////////////////////////////

//Needs RPM of 187.5 ~~ 188

//Needs testing to determine how many steps per mm

//Pins:
//     Step = 5
//     Direction = 4

#define stepPinY 5
#define dirPinY  4
#define stopPin 50
// Copied from ZMV_Testing /////////////
bool notStopped = true;
bool readyToGrabCase = true;
int buttonState = 0;
int clk = 0;
uint8_t range; //tofZ setup code
uint8_t status;
const int STEPS_PER_MM = 27;    // constant for steps per milimeter
const int MOTOR_SPEED = 1000;   //If torque is an issue, raise speed by increments of 250
double tempVar = 0;
double distToCase = range - tempVar;    //(constant for how much space we need to leave);


AccelStepper yMotor(1, stepPinY, dirPinY);
Adafruit_VL53L0X tofY = Adafruit_VL53L0X();
//////////////////////////////////////////
void setup(){
  Serial.begin(9600);
  yMotor.setMaxSpeed(500);
  
  //Emergency stop button
  pinMode(stopPin, INPUT);
  
  pinMode(stepPinY, OUTPUT);
  pinMode(dirPinY, OUTPUT);

  while (!Serial)               //makes sure TOF is connected
  {
  delay(1);
  }
  if (!tofY.begin())
  {
    Serial.println(F("Failed to find VL6180X (ZMV)"));
    while(1);
  }
  yMotor.setCurrentPosition(0);
}

void loop(){
  range = tofY.readRange();
  status = tofY.readRangeStatus();

  if (status == VL53L0X_ERROR_NONE){          //detect and give Z distance
    Serial.print("Distance in Y: ");
    Serial.println(range);
  }

  buttonState = digitalRead(stopPin);

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
        digitalWrite(dirPinY, LOW);
        for(int i=0; i<distToCase * STEPS_PER_MM; i++){
          digitalWrite(stepPinY, HIGH);
          delayMicroseconds(MOTOR_SPEED);
          digitalWrite(stepPinY, LOW);
          delayMicroseconds(MOTOR_SPEED);
        }
        delay(10);
        
        //Activate servo motor here
        //NEEDS SERVO CODE
        delay(1000);
        readyToGrabCase = false;
        
        //raise case
        digitalWrite(dirPinY, HIGH);
        for(int i=0; i<distToCase * STEPS_PER_MM; i++){
          digitalWrite(stepPinY, HIGH);
          delayMicroseconds(MOTOR_SPEED);
          digitalWrite(stepPinY, LOW);
          delayMicroseconds(MOTOR_SPEED);
        }
      }
  }
}
