//Author: Gideon Davis, Kaleb Sarlls

#include <Adafruit_VL53L0X.h>      // Y sensor library
#include <Adafruit_VL6180X.h>      // Z sensor library
#include <Wire.h>

Adafruit_VL53L0X tofY = Adafruit_VL53L0X();
Adafruit_VL6180X tofZ = Adafruit_VL6180X();

void setup() {
  Serial.begin(9600);              // number for ardunio setup
  
  //FIXME: add code to change I2C address thing o both sensors work at same time
  
  while (!Serial)                  // makes sure TOFs connected
  {
  delay(1);
  }
  
  Serial.println("TOF Sensors testing.");   // test if sensors are connected
  if (!tofY.begin())                        // kills code if both are not detected
  {
    Serial.println(F("Failed to find VL53L0X (YMV)"));
    while(1);
  }
  if (!tofZ.begin())
  {
    Serial.println(F("Failed to find VL6180X (ZMV)"));
    while(1);
  }
  
  Serial.println(F("TOF Testing Start:\n\n"));  // testing start
  tofY.startRangeContinuous();                  // start continuous ranging for YMV
}


void loop() {
  if (tofY.isRangeComplete()) {                 // detect and give Y distance
    Serial.print("Distance in Y-: ");
    Serial.println(tofY.readRange());
  }
  
  float lux = tofZ.readLux(VL6180X_ALS_GAIN_5);   // Z setup code
  Serial.print("Lux: ");                          // FIXME/TESTME: need to see difference in readlux and readRange functions
  Serial.println(lux);
  uint8_t range = tofZ.readRange();
  uint8_t status = tofZ.readRangeStatus();

  if (status == VL6180X_ERROR_NONE) {          // detect and give Z distance
    Serial.print(""Distance in Z: ");
    Serial.println(range);
  }
}