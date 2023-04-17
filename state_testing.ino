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

int buttonState = 0;
int clk = 0;
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

/* STATES TO TEST:
y max and min
z max and min
case grabbed
case dropped
# of cases loaded
speed of y and z and grabbing
*/
