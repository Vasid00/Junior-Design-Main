//Author: Kaleb Sarlls

//Needs RPM of 187.5 ~~ 188

//Needs testing to determine how many steps per mm

//Pins:
//     Step = 5
//     Direction = 4

#define stepPinY 5
#define dirPinY  4

#define stopPin 50

bool buttonState = false;

void setup(){
  Serial.begin(9600);
  
  //Emergency stop button
  pinMode(stopPin, INPUT);
  
  pinMode(stepPinY, OUTPUT);
  pinMode(dirPinY, OUTPUT);
}

void loop(){
  buttonState = digitalRead(stopPin);
  
  //FIXME: needs actual code
  
  //Safety delay
  delay(1000);
}
