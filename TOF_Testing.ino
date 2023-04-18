//Author: Gideon Davis, Kaleb Sarlls

#include <Adafruit_VL53L0X.h>      // Y sensor library
#include <Adafruit_VL6180X.h>      // Z sensor library
#include <Wire.h>

#define TOFY_ADDRESS 0x30
#define TOFZ_ADDRESS 0x31

Adafruit_VL53L0X tofY = Adafruit_VL53L0X();
Adafruit_VL6180X tofZ = Adafruit_VL6180X();

void setup() {
  Serial.begin(9600);              // number for ardunio setup
  Wire.begin();
  
  while (!Serial)                  // makes sure TOFs connected
  {
  delay(1);
  }
  
  Serial.println("*** TOF Sensors Testing for Both Y and Z ***");   // test if sensors are connected
  if (!tofY.begin(TOFY_ADDRESS))            // kills code if both are not detected
  {
    Serial.println(F("Failed to find VL53L0X (YMV)"));
    while(1);
  }
  if (!tofZ.begin(TOFZ_ADDRESS))
  {
    Serial.println(F("Failed to find VL6180X (ZMV)"));
    while(1);
  }
  
  Serial.println(F("TOF Testing Start:\n\n"));  // testing start
  tofY.startRangeContinuous();                  // start continuous ranging for YMV
  //uint8_t rangeZ;       //rangeY variable if permissable
  //uint8_t statusZ;
  
}


void loop() {
  if (tofY.isRangeComplete()) {                 // detect and give Y distance
    Serial.print("Y: ");
    Serial.print(tofY.readRange());
  }
  
  uint8_t rangeZ = tofZ.readRange();
  uint8_t statusZ = tofZ.readRangeStatus();
  if (statusZ == VL6180X_ERROR_NONE) {          // detect and give Z distance
    Serial.print("   Z: ");
    Serial.print(rangeZ);
    Serial.println();
  }
  delay(1000);
 }
