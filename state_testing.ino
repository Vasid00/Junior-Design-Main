#include <AccelStepper.h>       //Allows acceleration in bipolar steppers
#include <MultiStepper.h>       //Allows the use of multiple steppers
#include <Adafruit_VL53L0X.h>   // Y sensor library
#include <Adafruit_VL6180X.h>   // Z sensor library
#include <Wire.h>

#define dirPinZ  2
#define stepPinZ 3
#define dirPinY  4
#define stepPinY 5
#define stopPin 50
#define TOFY_ADDRESS 0x30
#define TOFZ_ADDRESS 0x31

int buttonState = 0;
int clk = 0;
//int caseHeight = ;    //case height in mm
bool grab2Cases = false;
bool caseGrabbed = false;
bool evenCount = false;
int ySpeed, zSpeed;
int caseCount, palletCount;
uint8_t distY; //tofZ setup code
uint8_t distZ;
const int STEPS_PER_MM = 27;    // constant for steps per milimeter
const int MOTOR_SPEED = 1000;   //If torque is an issue, raise speed by increments of 250

AccelStepper yMotor(1, stepPinY, dirPinY);
Adafruit_VL53L0X tofY = Adafruit_VL53L0X();
AccelStepper zMotor(1, stepPinZ, dirPinZ);
Adafruit_VL6180X tofZ = Adafruit_VL6180X();

// Min y range 9-13mm (pick-up side)
// Max y range 417-435mm (drop-off side)

void setup() {
  //FIXME: add setup from other codes
}

/* STATES TO TEST:
y max and min
z max and min
case grabbed
case dropped
# of cases loaded
speed of y and z and grabbing
*/

//FIXME: implement peusdocode below; will be what main loop consists of

void loop(){
  /* 
  if (!grab2Cases){
    for {
      ensure y min is reached
      lower z min
      ensure z max is reached
      grab bool
      rise to z min
      go to y max
      if (evenCount){
        lower to zMax                  //caseCount is even (0,2,4,6)
      if (!evenCount){
        lower to zMin - caseHeight      //caseCount is odd (1,3,5,7)
      drop case
      grabbed !bool
      increase case count for current pallet
      go to zMin
    }
    when caseCount == 8
      delay (lets other team do thing)
      reset caseCount
      palletCount++
      start main loop again 
/*
