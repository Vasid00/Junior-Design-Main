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

bool notStoppedZ = true;
int buttonState = 0;
int clk = 0;
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
  float lux = tofZ.readLux(VL6180X_ALS_GAIN_5);   //Z setup code
  Serial.print("Lux: ");                          //FIXME/TESTME: see what Lux is (also in TOF testing)
  Serial.println(lux);
  uint8_t range = tofZ.readRange();
  uint8_t status = tofZ.readRangeStatus();

  if (status == VL6180X_ERROR_NONE) {          //detect and give Z distance
    Serial.print(""Distance in Z: ");
    Serial.println(range);
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
    zMotor.setSpeed(200);
    zMotor.runSpeed();
    if(range <= 75)
    {
      delay(10);
      zMotor.setCurrentPosition(0);
    }
  }
};
