//Author: Gideon Davis


#include "DefinitionsAndConstants.h"
#include <Adafruit_VL53L0X.h>      // Y sensor library
#include <Adafruit_VL6180X.h>      // Z sensor library
#include <Wire.h>
#include <Servo.h>
#include <TFT_HX8357.h>

TFT_HX8357 lcd = TFT_HX8357(); //Call class

//Colors
#define WHITE     0xFFFF
#define BLACK     0x0000
#define AO_BLUE   0x2B15
#define MAROON    0x7800
#define DARKGREY  0x7BEF

//Controls constants
#define SEL_PIN 2
#define RET_PIN 3
#define POT_PIN A0 
#define BUTTON_NOT_PRESSED 0
#define SELECT_PRESSED 1
#define RETURN_PRESSED 2

//ScreenStates
#define SHUTDOWN 0
#define MENU 1
#define RUN 2
#define SETTINGS 3
#define STATS 4

//Motor Speeds
#define SPEED_FIVE 750
#define SPEED_FOUR 1000
#define SPEED_THREE 1250
#define SPEED_TWO 1500
#define SPEED_ONE 1750

//Indecies
#define NULL_IDX 0
#define Z_SPEED_IDX 1
#define Y_SPEED_IDX 2
#define CASE_LOC_IDX 3
#define PALLET_LOC_IDX 4
#define CALIBRATE_IDX 5

//Controls variables
bool buttonReady = true;
int potentState = 0;
int potentValue = 0;
int buttonState = 0;

//Screen state variables
int count = 0;
int screenState = SHUTDOWN;
int selLeftX = 0, selLeftY = 0;
int selRightX = 0, selRightY = 0;


//LCD_menu variables
char str_run[4] = "Run";
char str_settings[9] = "Settings";
char str_shutdown[9] = "Shutdown";
char str_teamX[8] = "Team X";
char str_robotics[10] = "Packaging";

//Settings variables
//ArrayVal
int motorSpeedZ;
int motorSpeedY;
int caseLocationY;
int caseLocationZ;
int palletLocationY;
int palletLocationZ;
int *settingsCommandOut;

//LCD////////////////////////////
void runLCD(int buttonValIn, int potentValIn, int *command, int *settingsCommand)
{
  lcd.setTextDatum(MC_DATUM);
  lcd.setTextColor(WHITE, BLACK);
  
  //Shutdown statement
  if(screenState == SHUTDOWN)
  {
    lcd.drawString("Press", 235, 60, 4);
    lcd.drawString("Select", 235, 100, 4);
    lcd.drawString("To", 235, 140, 4);
    lcd.drawString("Start", 235, 180, 4);

    if(buttonValIn == SELECT_PRESSED)
    {
      lcd.fillScreen(BLACK);
      screenState = MENU;
    }
  }

  //Menu statement
  else if(screenState == MENU)
  {
    selRightY = 292;
    selLeftY = 292;
    //FIXME: Draw art
    lcd.drawRect(49, 21, 382, 232, WHITE);
    lcd.drawString(str_teamX, 235, 100, 4);
    lcd.drawString(str_robotics, 235, 140, 4);
    //
    lcd.setTextDatum(MR_DATUM);
      
    //Run Button
    lcd.drawRect(45, 273, 119, 40, WHITE);
        
    //Spell Run
    lcd.drawString(str_run, 118, 290, 2);
    //

    //Settings Button
    lcd.drawRect(182, 273, 119, 40, WHITE);
        
    //Spell Settings
    lcd.drawString(str_settings, 265, 290, 2);
    //

    //ShutDown Button
    lcd.drawRect(319, 273, 119, 40, WHITE);
        
    //Spell Shutdown
    lcd.drawString(str_shutdown, 402, 290, 2);
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
    }
  }

  //Run statement
  else if(screenState == RUN)
  {
    lcd.drawString("Debug Testing", 235, 60, 4);
    selLeftX = 184;
    selRightX = 299;


    //For run testing

    //Move the Z motor up and down
    //Move the Y motor back and forth
    //Open and close the servo
    //Always display the TOFS on the side

    //Moving the Z motor button
    lcd.drawRect(182, 120, 119, 40, WHITE);
    lcd.drawString("Move Z Motor", 242, 140, 2);
    //

    //Moving the Y motor button
    lcd.drawRect(182, 170, 119, 40, WHITE);
    lcd.drawString("Move Y Motor", 242, 190, 2);
    //

    //Using the servo button
    lcd.drawRect(182, 220, 119, 40, WHITE);
    lcd.drawString("Open/Close Servo", 242, 240, 2);
    //
    //Cursor drawing
    //Cleanup
    lcd.fillTriangle(selLeftX, (selLeftY+15), selLeftX, (selLeftY-15), (selLeftX+15), selLeftY, BLACK);
    lcd.fillTriangle(selRightX, (selRightY+15), selRightX, (selRightY-15), (selRightX-15), selRightY, BLACK);

    //Bottom
    if(potentValIn < 330)
    {
      selLeftY = 240;
      selRightY = 240;
      if(buttonValIn == SELECT_PRESSED)
      {
        //Move servo
        *command = 3;
      }
    }
    //Middle
    else if(potentValIn < 660)
    {
      selLeftY = 190;
      selRightY = 190;
      if(buttonValIn == SELECT_PRESSED)
      {
        *command = 2;
      }
    }
    //Top
    else if(potentValIn < 1000)
    {
      selLeftY = 140;
      selRightY = 140;
      if(buttonValIn == SELECT_PRESSED)
      {
        *command = 1;
      }
    }

    if(buttonValIn == BUTTON_NOT_PRESSED)
    {
      *command = 0;
      lcd.fillTriangle(selLeftX, (selLeftY+15), selLeftX, (selLeftY-15), (selLeftX+15), selLeftY, AO_BLUE);
      lcd.fillTriangle(selRightX, (selRightY+15), selRightX, (selRightY-15), (selRightX-15), selRightY, AO_BLUE);
    }

    //Return button
    if(buttonValIn == RETURN_PRESSED)
    {
      lcd.fillScreen(BLACK);
      screenState = MENU;
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


    //Third setting: Case location//
    lcd.drawRect(20, 160, 120, 40, WHITE);
    lcd.drawString("Case Location", 80, 180, 2);

    //Fourth setting: Pallet location//
    lcd.drawRect(20, 210, 120, 40, WHITE);
    lcd.drawString("Pallet Location", 80, 230, 2);

    //Fifth setting: Calibrate positions//
    lcd.drawRect(20, 260, 120, 40, WHITE);
    lcd.drawString("Calibrate", 80, 280,2);


    //Draw cursor//
    //Cleanup
    lcd.fillTriangle(selLeftX, (selLeftY+15), selLeftX, (selLeftY-15), (selLeftX+15), selLeftY, BLACK);
    lcd.fillTriangle(selRightX, (selRightY+15), selRightX, (selRightY-15), (selRightX-15), selRightY, BLACK);

    //Calibrate
    if(potentValIn < 200)
    {
      selLeftY = 280;
      selRightY = 280;
      if(buttonValIn == SELECT_PRESSED)
      {
        //1 == true, must be reset after calibration finishes
        *settingsCommand = CALIBRATE_IDX;
      }
    }
    //Pallet Location
    else if(potentValIn < 400)
    {
      selLeftY = 230;
      selRightY = 230;
      if(buttonValIn == SELECT_PRESSED)
      {
        //Enter selection
        *settingsCommand = PALLET_LOC_IDX;
      }
    }
    //Case Location
    else if(potentValIn < 600)
    {
      selLeftY = 180;
      selRightY = 180;
      if(buttonValIn == SELECT_PRESSED)
      {
        *settingsCommand = CASE_LOC_IDX;
      }
    }
    //Motor Y Speed
    else if(potentValIn < 800)
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
    }
  }
  //Error statement
  else
  {
    lcd.fillScreen(BLACK);
    screenState = SHUTDOWN;
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
  else if(*settingsCommand == CASE_LOC_IDX)
  {
    //Adjustable by mm
    
  }
  else if(*settingsCommand == PALLET_LOC_IDX)
  {
    //Adjustable by mm
    
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

  settingsCommandOut = new int;
  *settingsCommandOut = NULL_IDX;

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

  //Lazy debounce solution -> delay it
  delay(100);

  //Memory freeing
  delete commandOut;
}
