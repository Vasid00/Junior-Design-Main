#include <Adafruit_VL6180X.h>
#include <TFT_HX8357.h>
#include <string.h>
#include <Servo.h>

Adafruit_VL6180X zTof = Adafruit_VL6180X();
Servo gripper;
TFT_HX8357 lcd = TFT_HX8357(); //Call class

//Colors
#define WHITE     0xFFFF
#define BLACK     0x0000
#define AO_BLUE   0x2B15
#define MAROON    0x7800
#define DARKGREY  0x7BEF
#define DARKCYAN  0x03EF
#define PURPLE    0x780F
#define OLIVE     0x7BE0
#define LIGHTGREY 0xC618
#define GOLD      0xFC0F

//Pin values
#define SEL_PIN   2
#define RET_PIN   3
#define YMOT_DIR  4
#define YMOT_STEP 5
#define ZMOT_DIR  6
#define ZMOT_STEP 7
#define POT_PIN   A0 
#define SERVO_SIG 10

//Controls constants
#define BUTTON_NOT_PRESSED 0
#define SELECT_PRESSED     1
#define RETURN_PRESSED     2
#define TIME_BETWEEN_CASES

//Servo constants
#define SERVO_CLOSED 0
#define SERVO_OPEN   7

//Motor constants
#define Z_DOWN LOW
#define Z_UP   HIGH
#define YDIR_TO_CASE
#define YDIR_TO_PALLET
#define DIST_PALLET_TO_CASE
#define DIST_TO_PALLET_OPTION_1 //far two rows
#define DIST_TO_PALLET_OPTION_2 //close two rows
#define Z_DIST_TO_PALLET 
#define DIST_TO_CASE_1
#define DIST_TO_CASE_2

//ScreenStates
#define SHUTDOWN 0
#define MENU     1
#define RUN      2
#define SETTINGS 3
#define STATS    4

//Motor Speeds
#define SPEED_FIVE  750
#define SPEED_FOUR  1000
#define SPEED_THREE 1250
#define SPEED_TWO   1500
#define SPEED_ONE   1750

//Indecies
#define NULL_IDX       0
#define Z_SPEED_IDX    1
#define Y_SPEED_IDX    2
#define CALIBRATE_IDX  5

//Controls variables
bool buttonReady = true;
int potentState = 0;
int potentValue = 0;
int buttonState = 0;

//Screen state variables
bool drawn = false;
int count = 0;
int screenState = SHUTDOWN;
int selLeftX = 0, selLeftY = 0;
int selRightX = 0, selRightY = 0;

//Stats value
int casesLoaded;
int palletsLoaded;
int clock, clock2;
double casesPerHour;

//Settings variables
//ArrayVal
int motorSpeedZ;
int motorSpeedY;
int caseLocationY;
int caseLocationZ;
int palletLocationY;
int palletLocationZ;
int *settingsCommandOut;

bool bootup;

String tempString;

//Main loop
int stateIdxCur;

//LCD////////////////////////////
void runLCD(int buttonValIn, int potentValIn, int *command, int *settingsCommand)
{
  lcd.setTextDatum(MC_DATUM);
  lcd.setTextColor(WHITE, BLACK);

  //Shutdown statement
  if(screenState == SHUTDOWN)
  {
    lcd.drawString("Press", 235, 80, 4);
    lcd.drawString("Select", 235, 120, 4);
    lcd.drawString("To", 235, 160, 4);
    lcd.drawString("Start", 235, 200, 4);

    if(buttonValIn == SELECT_PRESSED)
    {
      lcd.fillScreen(BLACK);
      screenState = MENU;
      drawn = false;
    }
  }

  //Menu statement
  else if(screenState == MENU)
  {
    selRightY = 292;
    selLeftY = 292;
    //Main Menu Art:
    if(!drawn)
    {
      //Main Menu Art:
      //OutlineBox
      lcd.drawRect(30, 20, 420, 230, DARKCYAN);
      //

      //Header Box
      lcd.fillRect(30, 20, 420, 40, DARKCYAN);

      //Team Name
      lcd.setTextColor(WHITE, DARKCYAN);
      lcd.drawString("Team X Packaging", 240, 40, 4);
      lcd.setTextColor(WHITE, BLACK);
      //Outline
      lcd.drawRect(130, 22, 220, 36, WHITE);

      //Canvas bounds
      //X: [30 : 450]
      //Y: [20 + 40(header) : 250]

      //Conveyer Boxes
      //Right
      lcd.fillRect(330, 170, 60, 80, DARKCYAN); //X: [330 : 390], Y: [170 : 250]
      //Left
      lcd.fillRect(70, 170, 60, 80, DARKCYAN); // X: [70 : 130], Y: [170 : 250]

      //Pallet
      lcd.fillRect(331, 162, 58, 6, DARKGREY);
      lcd.drawRect(330, 161, 60, 8, WHITE);

      //Case
      lcd.fillRect(91, 150, 18, 18, DARKGREY);
      lcd.drawRect(90, 149, 20, 20, WHITE);
      
      //Z Motor
      lcd.fillRect(120, 70, 15, 15, DARKGREY);

      //Lead Screw
      lcd.fillRect(125, 85, 5, 60, LIGHTGREY);

      //Guide rods
      //Left
      lcd.fillRect(60, 70, 5, 75, DARKGREY);
      //Right
      lcd.fillRect(145, 70, 5, 75, DARKGREY);

      //Y rail
      lcd.fillRect(31, 72, 420, 8, LIGHTGREY);
      lcd.drawRect(31, 72, 420, 8, DARKGREY);

      //Gripper Holster
      //Thin box
      lcd.fillRect(55, 100, 100, 10, WHITE);

      //Servo
      //Motor
      lcd.fillRect(82, 110, 38, 10, PURPLE);
      //Gear
      lcd.fillCircle(115, 115, 5, GOLD);
      lcd.drawCircle(115, 115, 5, PURPLE);
      
      //Claw
      //Left
      lcd.fillRect(84, 120, 5, 40, PURPLE);
      //Right
      lcd.fillRect(115, 120, 5, 40, PURPLE);
      //Claw lip
      lcd.fillRect(112, 155, 3, 5, PURPLE);
      
      drawn = true;
    //
    }
    
    //
    lcd.setTextDatum(MR_DATUM);
      
    //Run Button
    lcd.drawRect(45, 273, 119, 40, WHITE);
        
    //Spell Run
    lcd.drawString("Run", 118, 290, 2);
    //

    //Settings Button
    lcd.drawRect(182, 273, 119, 40, WHITE);
        
    //Spell Settings
    lcd.drawString("Settings", 265, 290, 2);
    //

    //ShutDown Button
    lcd.drawRect(319, 273, 119, 40, WHITE);
        
    //Spell Shutdown
    lcd.drawString("Shutdown", 402, 290, 2);
    //

    //Place selection indicator
    
    //Clean up old triangles
    lcd.fillTriangle(selLeftX, (selLeftY+15), selLeftX, (selLeftY-15), (selLeftX+15), selLeftY, BLACK);
    lcd.fillTriangle(selRightX, (selRightY+15), selRightX, (selRightY-15), (selRightX-15), selRightY, BLACK);
    //Determine new position
    //Right side
    if(potentValIn < 330)
    {
      selLeftX = 321;
      selRightX = 436;
      if(buttonValIn == SELECT_PRESSED)
      {
        lcd.fillScreen(BLACK);
        screenState = SHUTDOWN;
        drawn = false;
      }
    }
    //Middle
    else if(potentValIn < 660)
    {
      selLeftX = 184;
      selRightX = 299;
      if(buttonValIn == SELECT_PRESSED)
      {
        lcd.fillScreen(BLACK);
        screenState = SETTINGS;
        drawn = false;
      }
    }
    //Left side
    else
    {
      selLeftX = 47;
      selRightX = 162;
      if(buttonValIn == SELECT_PRESSED)
      {
        lcd.fillScreen(BLACK);
        screenState = RUN;
        drawn = false;
        casesLoaded = 0;
        casesPerHour = 0;
        clock = 0;
        stateIdxCur = 0;
      }
    }
    //Draw new triangles
    //Only draw if no button was pressed
    if(screenState == MENU){
      lcd.fillTriangle(selLeftX, (selLeftY+15), selLeftX, (selLeftY-15), (selLeftX+15), selLeftY, AO_BLUE);
      lcd.fillTriangle(selRightX, (selRightY+15), selRightX, (selRightY-15), (selRightX-15), selRightY, AO_BLUE);
    }

    if(buttonValIn == RETURN_PRESSED){
      lcd.fillScreen(BLACK);
      screenState = SHUTDOWN;
      drawn = false;
    }
  }

  //Run statement
  else if(screenState == RUN)
  {
    lcd.drawString("Run Test", 235, 60, 4);
    selLeftX = 184;
    selRightX = 299;


    //For run testing

    //Move the Z motor up and down
    //Move the Y motor back and forth
    //Open and close the servo
    //Always display the TOFS on the side

    //Clock
    lcd.drawRect(182, 120, 119, 40, WHITE);
    lcd.drawString("Time passed: ", 242, 140, 2);

    tempString = String(clock);

    lcd.drawString(tempString.c_str(), 303, 140, 2); //x = 303
    //

    //Cases Loaded
    lcd.drawRect(182, 170, 119, 40, WHITE);
    lcd.drawString("Cases loaded: ", 242, 190, 2);

    tempString = String(casesLoaded);

    lcd.drawString(tempString.c_str(), 303, 190, 2);
    //

    //Cases per hour
    lcd.drawRect(182, 220, 119, 40, WHITE);
    lcd.drawString("Cases/Hour: ", 242, 240, 2);
    
    tempString = String(casesPerHour);

    lcd.drawString(tempString.c_str(), 303, 240, 2);
    //

    if((clock >= 3600)&&(buttonValIn == SELECT_PRESSED))
    {
      lcd.fillScreen(BLACK);
      drawn = false;
      screenState = MENU;
    }
    if(stateIdxCur != -1)
    {
      *command = 1;
    }
  }

  //Settings statement
  else if(screenState == SETTINGS)
  {
    int temp;
    //Constants
    selLeftX = 22;
    selRightX = 138;

    //Draw settings screen

    //Order of drawing
    //Draw background
    //Draw initial buttons
    //Draw conditional buttons
    //Input settings values
    //Draw cursor

    // /* Drawing concept */
    //
    //  ____________
    //  | Option 1 |
    //  ‾‾‾‾‾‾‾‾‾‾‾‾
    //  ____________
    //  | Option 2 |
    //  ‾‾‾‾‾‾‾‾‾‾‾‾
    //  ____________
    //  | Option 3 |
    //  ‾‾‾‾‾‾‾‾‾‾‾‾
    //  ____________
    //  | Option 4 |
    //  ‾‾‾‾‾‾‾‾‾‾‾
    //  ____________
    //  | Option 5 |
    //  ‾‾‾‾‾‾‾‾‾‾‾‾
    lcd.drawString("Settings", 235, 20, 4);

    //First setting: Z motor speed//
    lcd.drawRect(20, 60, 120, 40, WHITE);
    lcd.drawString("Z Motor Speed", 80, 80, 2);
    //Current Value
    temp = 7 - ((motorSpeedZ/250));
    for(int i=0; i<=temp; i++)
    {
      lcd.fillTriangle(150 + (i*20), (95), 150 + (i*20), (65), (165) + (i*20), 80, DARKGREY);
    }



    //Second setting: Y motor speed//
    lcd.drawRect(20, 110, 120, 40, WHITE);
    lcd.drawString("Y Motor Speed", 80, 130, 2);
    //Current Value
    temp = 7 - ((motorSpeedY/250));
    for(int i=0; i<=temp; i++)
    {
      lcd.fillTriangle(150 + (i*20), (145), 150 + (i*20), (115), (165) + (i*20), (130), DARKGREY);
    }

    //Fifth setting: Calibrate positions//
    lcd.drawRect(20, 160, 120, 40, WHITE);
    lcd.drawString("Calibrate", 80, 280,2);


    //Draw cursor//
    //Cleanup
    lcd.fillTriangle(selLeftX, (selLeftY+15), selLeftX, (selLeftY-15), (selLeftX+15), selLeftY, BLACK);
    lcd.fillTriangle(selRightX, (selRightY+15), selRightX, (selRightY-15), (selRightX-15), selRightY, BLACK);

    //Calibrate
    if(potentValIn < 330)
    {
      selLeftY = 180;
      selRightY = 180;
      if(buttonValIn == SELECT_PRESSED)
      {
        //1 == true, must be reset after calibration finishes
        *settingsCommand = CALIBRATE_IDX;
      }
    }
    //Motor Y Speed
    else if(potentValIn < 660)
    {
      selLeftY = 130;
      selRightY = 130;
      if(buttonValIn == SELECT_PRESSED)
      {
        *settingsCommand = Y_SPEED_IDX;
      }
    }
    //Motor Z Speed
    else if(potentValIn < 1000)
    {
      selLeftY = 80;
      selRightY = 80;
      if(buttonValIn == SELECT_PRESSED)
      {
        *settingsCommand = Z_SPEED_IDX;
      }
    }

    if(buttonValIn == BUTTON_NOT_PRESSED)
    {
      *settingsCommand = NULL_IDX;
      lcd.fillTriangle(selLeftX, (selLeftY+15), selLeftX, (selLeftY-15), (selLeftX+15), selLeftY, AO_BLUE);
      lcd.fillTriangle(selRightX, (selRightY+15), selRightX, (selRightY-15), (selRightX-15), selRightY, AO_BLUE);
    }

    if(buttonValIn == RETURN_PRESSED)
    {
      lcd.fillScreen(BLACK);
      screenState = MENU;
      drawn = false;
    }
  }

  //Statistics statement
  else if(screenState == STATS)
  {

    //Display the stats

    if(buttonValIn != BUTTON_NOT_PRESSED)
    {
      lcd.fillScreen(BLACK);
      screenState = MENU;
      drawn = false;
    }
  }
  //Error statement
  else
  {
    lcd.fillScreen(BLACK);
    screenState = SHUTDOWN;
    drawn = false;
  }
}

//SETTINGS////////////////////////////
void runSettingsSelection(int potentValueIn, int *settingsCommand)
{
  int tempVal;
  int tempPtX;
  int tempPtY;
  lcd.setTextDatum(MC_DATUM);
  lcd.setTextColor(WHITE, BLACK);

  if(*settingsCommand == Z_SPEED_IDX)
  {
    
    tempPtY = 80;
    tempPtX = 150;
    //Cleanup
    for(int i=0; i<=4; i++)
    {
      lcd.fillTriangle(tempPtX + (i*20), (tempPtY+15), tempPtX + (i*20), (tempPtY-15), (tempPtX+15) + (i*20), tempPtY, BLACK);
    }


    tempVal = floor(potentValueIn/200);

    if(tempVal == 0)
    {
      motorSpeedZ = SPEED_ONE;
    }
    else if(tempVal == 1)
    {
      motorSpeedZ = SPEED_TWO;
    }
    else if(tempVal == 2)
    {
      motorSpeedZ = SPEED_THREE;
    }
    else if(tempVal == 3)
    {
      motorSpeedZ = SPEED_FOUR;
    }
    else if(tempVal == 4)
    {
      motorSpeedZ = SPEED_FIVE;
    }

    //New triangles
    for(int i=0; i<=tempVal; i++)
    {
      lcd.fillTriangle(tempPtX + (i*20), (tempPtY+15), tempPtX + (i*20), (tempPtY-15), (tempPtX+15) + (i*20), tempPtY, MAROON);
    }

  }
  else if(*settingsCommand == Y_SPEED_IDX)
  {
    tempPtY = 130;
    tempPtX = 150;
    //Cleanup
    for(int i=0; i<=4; i++)
    {
      lcd.fillTriangle(tempPtX + (i*20), (tempPtY+15), tempPtX + (i*20), (tempPtY-15), (tempPtX+15) + (i*20), tempPtY, BLACK);
    }


    tempVal = floor(potentValueIn/200);

    if(tempVal == 0)
    {
      motorSpeedY = SPEED_ONE;
    }
    else if(tempVal == 1)
    {
      motorSpeedY = SPEED_TWO;
    }
    else if(tempVal == 2)
    {
      motorSpeedY = SPEED_THREE;
    }
    else if(tempVal == 3)
    {
      motorSpeedY = SPEED_FOUR;
    }
    else if(tempVal == 4)
    {
      motorSpeedY = SPEED_FIVE;
    }

    //New triangles
    for(int i=0; i<=tempVal; i++)
    {
      lcd.fillTriangle(tempPtX + (i*20), (tempPtY+15), tempPtX + (i*20), (tempPtY-15), (tempPtX+15) + (i*20), tempPtY, MAROON);
    }
  }
  else if(*settingsCommand == CALIBRATE_IDX)
  {
    //Run calibration
    //Idea:
    //    Sends motor to both sides
    //    Scans Z distance to conveyer on both sides
    //    Updates internal parameters
    //    Scans Y bounds of conveyers on both sides
    //    Updates internal parameters
    //    Determines if 2 cases is possible
    Serial.println("FIXME: Implement calibration");
  }
  else
  {
    *settingsCommand = NULL_IDX;
  }

}

//MOTORS//////////////////////////////
void moveZmotor(int steps, int direction)
{
  digitalWrite(ZMOT_DIR, direction);
  for(int i=0; i<steps; i++)
  {
    digitalWrite(ZMOT_STEP,LOW);
    delayMicroseconds(motorSpeedZ);
    digitalWrite(ZMOT_STEP,HIGH);
    delayMicroseconds(motorSpeedZ);
  }
}

void moveYmotor(int steps, int direction)
{
  digitalWrite(YMOT_DIR, direction);
  for(int i=0; i<steps; i++)
  {
    digitalWrite(YMOT_STEP,LOW);
    delayMicroseconds(motorSpeedY);
    digitalWrite(YMOT_STEP,HIGH);
    delayMicroseconds(motorSpeedY);
  }
}

//SETUP////////////////////////
void setup() {
  // put your setup code here, to run once:

  //Initial Values
  motorSpeedZ = SPEED_ONE;
  motorSpeedY = SPEED_ONE;
  caseLocationZ = 0;
  caseLocationY = 0;
  palletLocationZ = 0;
  palletLocationY = 0;

  palletsLoaded = 0;
  casesLoaded = 0;
  casesPerHour = 0;
  clock = 0;

  settingsCommandOut = new int;
  *settingsCommandOut = NULL_IDX;

  stateIdxCur = 0;

  bootup = false;  

  gripper.attach(SERVO_SIG);
  Serial.begin(9600);

  //Controls setup
  pinMode(SEL_PIN, INPUT);
  pinMode(RET_PIN, INPUT);
  pinMode(POT_PIN, INPUT);

  //LCD setup
  lcd.init();
  lcd.begin();
  lcd.fillScreen(BLACK);
  lcd.setRotation(1);
}

//LOOP/////////////////////////
void loop() {
  //Memory allocation for memory heavy commands
  int *commandOut = new int;
  //Read potentiometer value
  potentValue = analogRead(POT_PIN);
  //Serial Debugging
  Serial.print("Potentiometer Value: ");
  Serial.print(potentValue);

  //Serial debugging
  Serial.print(", Button State: ");

  
  //Value passing and button reading
  if(buttonReady){
    
    //Determine which button was pressed
    if(digitalRead(SEL_PIN) == HIGH)
    {
      buttonState = SELECT_PRESSED;
    }
    else if(digitalRead(RET_PIN) == HIGH)
    {
      buttonState = RETURN_PRESSED;
    }
    else 
    {
      buttonState = BUTTON_NOT_PRESSED;
    }

    if(buttonState != BUTTON_NOT_PRESSED){
      buttonReady = false;
    }
  }
  else{
    buttonState = BUTTON_NOT_PRESSED;
    buttonReady = true;
  }


  //Serial Debugging
  if(buttonState == SELECT_PRESSED){
    Serial.println("Select");
  }
  else if(buttonState == RETURN_PRESSED){
    Serial.println("Return");
  }
  else{
    Serial.println("No Button Pressed");
  }


    //read settings output and draw a selection to the screen
  
  if(*settingsCommandOut == NULL_IDX)
  {
    runLCD(buttonState, potentValue, commandOut, settingsCommandOut);
  }

  if((*settingsCommandOut != NULL_IDX))
  {
    runSettingsSelection(analogRead(POT_PIN), settingsCommandOut);

    if(digitalRead(RET_PIN) == HIGH)
    {
      lcd.fillScreen(BLACK);
      *settingsCommandOut = NULL_IDX;
    }
  }

  //Add calibration state

  //Add main function state


  //Y starts at case

  //After 8 cases                             17
  //Detect new pallet                         18
  
  //Move Y motor above case side              0 X
  //Detect if case is there                   1 X
  //Open gripper                              2 X
  //Lower Z motor to first case               3 
  //Close gripper                             4
  //Raise Z motor                             5
  //Wait for next case                        6
  //Lower Z motor to above second case        7
  //Open gripper                              8
  //Lower Z motor to second case              9
  //Closer gripper                            10
  //Raise Z motor fully                       11
  //Move Y motor to pallet side               12
  //Lower to pallet                           13
  //Open gripper                              14
  //Raise Z motor fully                       15
  //Increment casesLoaded by 2                16
  //if casesLoaded == 8 go to special case    17
  //else Repeat                               17

  //Main loop
  if(*commandOut == 1)
  {
    //Cycle 0 - Move above case
    if(stateIdxCur == 0)
    {
      if(!bootup)
      {
        bootup = true;
      }
      else
      {
        moveYmotor(DIST_PALLET_TO_CASE, YDIR_TO_CASE);
      }
    }
    
    //Cycle 1 - Check if case is under gripper
    else if(stateIdxCur == 1)
    {
      bool casePresent = false;
      while(!casePresent)
      {
        if(zTof.readRange() <= caseLocationZ-1)
        {
          casePresent = true;
        }
      }
      
    }
    
    //Cycle 2 - Open gripper
    else if(stateIdxCur == 2)
    {
      gripper.write(SERVO_OPEN);
    }
    
    //Cycle 3 - Lower to case 1
    else if(stateIdxCur == 3)
    {
      moveZmotor(DIST_TO_CASE_1, DOWN);
    }
    
    //Cycle 4 - Close gripper
    else if(stateIdxCur == 4)
    {
      gripper.write(SERVO_CLOSED);
    }
    
    //Cycle 5 - Lift Z
    else if(stateIdxCur == 5)
    {
      moveZmotor(DIST_TO_CASE_1, UP);
    }
    
    //Cycle 6 - wait for next case
    else if(stateIdxCur == 6)
    {
      delay(TIME_BETWEEN_CASES);
    }
    
    //Cycle 7 - Lower Z to second case
    else if(stateIdxCur == 7)
    {
      moveZmotor(DIST_TO_CASE_2, DOWN);
    }
    
    //Cycle 8 - open gripper
    else if(stateIdxCur == 8)
    {
      gripper.write(SERVO_OPEN);
    }
    
    //Cycle 9 - lower Z to first case location
    else if(stateIdxCur == 9)
    {
      moveZmotor(DIST_TO_CASE_1 - DIST_TO_CASE_2, DOWN);
    }
    
    //Cycle 10 - close gripper
    else if(stateIdxCur == 10)
    {
      gripper.write(SERVO_CLOSED);
    }
    
    //Cycle 11 - raise Z
    else if(stateIdxCur == 11)
    {
      moveZmotor(DIST_TO_CASE_1, UP);
    }
    
    //Cycle 12 - Move y motor
    else if(stateIdxCur == 12)
    {
      if((casesLoaded == 0)||
         (casesLoaded == 4)
      {
        moveYmotor(DIST_TO_PALLET_OPTION_1, DIR_TO_PALLET); //Far two rows
      }
      else if((casesLoaded == 2)||
              (casesLoaded == 6)
      {
        moveYmotor(DIST_TO_PALLET_OPTION_2, DIR_TO_PALLET); //Close two rows
      }
      
    }
    
    //Cycle 13 - Lower Z
    else if(stateIdxCur == 13)
    {
      moveZmotor(Z_DIST_TO_PALLET, DOWN);
    }
    
    //Cycle 14 - open gripper
    else if(stateIdxCur == 14)
    {
      gripper.write(SERVO_OPEN);
    }
    
    //Cycle 15 - raise Z
    else if(stateIdxCur == 15)
    {
      moveZmotor(Z_DIST_TO_PALLET, UP);
    }
    
    //Cycle 16 - increment cases
    else if(stateIdxCur == 16)
    {
      casesLoaded += 2;
    }
    
    //Cycle 17 - check case amount
    else if(stateIdxCur == 17)
    {
      if(casesLoaded >= 8)
      {
        casesLoaded = 0;
        palletsLoaded++;
      }
      else
      {
        stateIdxCur = 0;
      }
    }
    
    //Cycle 18 - check for pallet
    else if(stateIdxCur == 18)
    {
      bool palletPresent = false;
      while(!palletPresent)
      {
        if(zTof.readRange() <= palletLocationZ - 1)
        {
          palletPresent = true;
        }
      }
    }
    
    //Cycle 19 - 
    else if(stateIdxCur == 19)
    {
      stateIdxCur = 0;
    }


    //Null Cycle
    else
    {
      stateIdxCur = -99;
      *commandOut == 0;
    }
    if(stateIdxCur != -99)
    {
      stateIdxCur++;
    }
  }

  //Lazy debounce solution -> delay it
  delay(100);
  clock2++;
  if(clock2 >= 10)
  {
    clock2 = 0;
    clock++;
  }
  if(clock >= 3600)
  {
    clock2 = 0;
    clock = 0;
  }
  //Memory freeing
  delete commandOut;
}
