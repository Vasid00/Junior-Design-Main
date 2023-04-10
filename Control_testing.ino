//Author: Gideon Davis, Kaleb Sarlls

//Controls with a potentiometer instead of buttons

//Idea:
//     Potentionmeter has different "states" connected to resistance values. If the potentiometer is set to that state it changes the control signal to that state.
//     This allows easier wiring and more accurate coding without having to deal with debounce interrupts.

//     There are two buttons used for select and return.
//     Currently the output signal is only serial, but it can be modified to go elsewhere.


int potentState = 0;
int potentValue = 0;

int buttonStateSel = 0;
int buttonStateRet = 0;
bool buttonReady = true;
int count = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(2, INPUT);
  pinMode(A0, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  Serial.print("Potentiometer State: ");
  potentValue = analogRead(A0);

  if(potentValue < 330){
    potentState = 1;
    Serial.print("Left");
  }
  else if(potentValue < 660){
    potentState = 2;
    Serial.print("Middle");
  }
  else{
    potentState = 3;
    Serial.print("Right");
  }





  Serial.print(", Button State: ");

  if(buttonReady){
    buttonStateSel = digitalRead(2);
    buttonStateRet = digitalRead(3);
    if((digitalRead(2) == HIGH) || (digitalRead(3) == HIGH)){
      buttonReady = false;
    }
  }
  else{
    if((buttonStateSel == 1) && (buttonStateRet == 1)) buttonStateSel = 0;
    count++;
    if(count == 1){
      count = 0;
      buttonStateSel = 0;
      buttonStateRet = 0;
      buttonReady = true;
    }
    
  }

  if(buttonStateSel == 1){
    Serial.println("Select");
  }
  else if(buttonStateRet == 1){
    Serial.println("Return");
  }
  else{
    Serial.println("N/A");
  }


  delay(100);
}
