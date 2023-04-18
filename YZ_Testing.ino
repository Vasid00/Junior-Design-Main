//Author: Kaleb Sarlls
//Updated: 4/17/23
//VERY SHITTY CODE TO TEST Z AND Y MOTORS AT THE SAME TIME

#include <AccelStepper.h>       //Allows acceleration in bipolar steppers
#include <MultiStepper.h>       //Allows the use of multiple steppers
// Tried including Adafruit library
#include <Adafruit_VL53L0X.h>
////////

//Needs RPM of 187.5 ~~ 188

//Needs testing to determine how many steps per mm

//Pins:
//     Step = 5
//     Direction = 4

#define stepPinY 5
#define dirPinY  4
#define stepPinZ 7
#define dirPinZ  6
#define stopPin 50
// Copied from ZMV_Testing /////////////
bool notStopped = true;
bool readyToGrabCase = true;
int buttonState = 0;
int clk = 0;
//uint8_t range; //tofZ setup code
//uint8_t status;
const int STEPS_PER_MM = 27;    // constant for steps per milimeter
const int MOTOR_SPEED = 1200;   //If torque is an issue, raise speed by increments of 250
double tempVar = 0;
double distToCase = 20 - tempVar;    //(constant for how much space we need to leave);

// Min range 9-13mm (pick-up side)
// Max range 417-435mm (drop-off side)

AccelStepper zMotor(1, stepPinZ, dirPinZ);
AccelStepper yMotor(1, stepPinY, dirPinY);
//Adafruit_VL53L0X tofY = Adafruit_VL53L0X();
//////////////////////////////////////////
void setup(){
  Serial.begin(9600);
  yMotor.setMaxSpeed(500);  
  zMotor.setMaxSpeed(500);
  
  //Emergency stop button
  pinMode(stopPin, INPUT);
  
  pinMode(stepPinY, OUTPUT);
  pinMode(dirPinY, OUTPUT);
  pinMode(stepPinZ, OUTPUT);
  pinMode(dirPinZ, OUTPUT);

  while (!Serial)               //makes sure TOF is connected
  {
  delay(1);
  }
}
 /* if (!tofY.begin())
  {
    Serial.println(F("Failed to find VL6180X (ZMV)"));
    while(1);
  }
  yMotor.setCurrentPosition(0);
}*/

void loop(){
  /*range = tofY.readRange();
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
    */
      // This would need a boolean for if the mechanism is ready to grab the case.
      // This boolean would be supplied from control monitoring of the entire system
     // if(readyToGrabCase){ 
        //Hardcoded example for stepper control:
        
        //Lower to grab the case
        digitalWrite(dirPinY, HIGH);     //TO Y MAX SIDE
        for(int i=0; i<distToCase * STEPS_PER_MM; i++){
          digitalWrite(stepPinY, HIGH);
          delayMicroseconds(MOTOR_SPEED);
          digitalWrite(stepPinY, LOW);
          delayMicroseconds(MOTOR_SPEED);
        }
        delay(100);
        
        digitalWrite(dirPinZ, LOW);     //TO Y MAX SIDE
        for(int i=0; i<500; i++){
          digitalWrite(stepPinZ, HIGH);
          delayMicroseconds(MOTOR_SPEED);
          digitalWrite(stepPinZ, LOW);
          delayMicroseconds(MOTOR_SPEED);
        }
        delay(500);
                
        //Activate servo motor here
        //NEEDS SERVO CODE
        //delay(1000);
       // readyToGrabCase = false;

        digitalWrite(dirPinZ, HIGH);     
        for(int i=0; i<50; i++){
          digitalWrite(stepPinZ, HIGH);
          delayMicroseconds(MOTOR_SPEED);
          digitalWrite(stepPinZ, LOW);
          delayMicroseconds(MOTOR_SPEED);
        }
        
        //raise case
        digitalWrite(dirPinY, LOW);   //TO Y MIN SIDE
        for(int i=0; i<distToCase * STEPS_PER_MM; i++){
          digitalWrite(stepPinY, HIGH);
          delayMicroseconds(MOTOR_SPEED);
          digitalWrite(stepPinY, LOW);
          delayMicroseconds(MOTOR_SPEED);
        }
      }
