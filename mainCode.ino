//Author: Gideon Davis
//File: FunctionStorage.ino
//Description: This is the main control file for the JR DESIGN system
//             This system uses 2 steppers, 1 servo, 2 tof sensors, 5 push buttons, and 1 3.5" TFT display
//             The purpose of this system is secondary packaging
//Date Created: 02/04/2023

#include <TFT_HX8357.h> //Custom Lib by Bodmer for LCD
#include <VL6180X.h> //for TOF Z
#include <VL53L0X.h> //for TOF Y
#include <AccelStepper.h> //for Steppers
#include <MultiStepper.h> //for Steppers
#include <Servo.h> //for Servo

#define BLACK 0x0000
#define WHITE 0xFFFF

#define BOUNDS_H 480
#define BOUNDS_V 320

//FIXME: add bounds
#define CASE_H_BOUNDS_POS
#define CASE_H_BOUNDS_NEG

//FIXME: add bounds
#define CASE_V_BOUNDS_POS
#define CASE_V_BOUNDS_NEG

//FIXME: add bounds
#define PALLET_H_BOUNDS_POS
#define PALLET_H_BOUNDS_NEG

//FIXME: add bounds
#define PALLET_V_BOUNDS_POS
#define PALLET_V_BOUNDS_NEG

#define POWER_ON       10
#define SCREEN_AMOUNT  10
#define SHUTDOWN_STATE 0
#define BOOTUP_STATE   1
#define MENU_STATE     2
#define SETTINGS_STATE 3
#define INIT_STATE     4
#define RUN_STATE      5
#define STATS_STATE    6

#define SERVO_OPEN   30
#define SERVO_CLOSED 60

const float DEG_TO_RAD_CONSTANT = 57.2957795131; //conversion standard from deg to rad
const float PI = 3.141592653; //Pi to 10 digits

//GLOBAL VARIABLES
  //Bottom left of case
  double caseStart_H = 0;
  double caseStart_V = 0;

  //Bottom left of pallet
  double palletStart_H = 0;
  double palletStart_V = 0;

  //Booleans for settings
  bool caseStartSel = false;
  bool palletStartSel = false;
  bool twoCubeMode = true;
  bool showStats = true;
  bool autoParameters = true;

/************************************************************ DPAD ************************************************************/
  const int button5 = 2;
  const int button4 = 3;
  const int button3 = 4;
  const int button2 = 5;
  const int button1 = 6;

  int buttonState1, buttonState2, buttonState3, buttonState4, buttonState5;

/************************************************************ SERVO ************************************************************/

  Servo gripper;

  const int servoSignal = 7;
  int servoAngle = 0;

  bool moveServo, servoState; //servoState true = open, false = closed


/************************************************************ STEPPER ************************************************************/

  const int zDriver_En = 8;
  const int zDriver_Dir = 9;
  const int zDriver_Step = 10;
  const int yDriver_En = 11;
  const int yDriver_Dir = 12;
  const int yDriver_Step = 13;

  int zMotorSteps, yMotorSteps;



/************************************************************ NO CONNECTION ************************************************************/ 

  int NC_1 = null;
  int NC_14 = null;
  int NC_15 = null;
  int NC_16 = null;
  int NC_17 = null;


/************************************************************ TOF ************************************************************/
  const int zTof_SCL = 18;
  const int zTof_SDA = 19;
  const int yTof_SCL = 20;
  const int yTof_SDA = 21;




/************************************************************ LCD ************************************************************/
  TFT_HX8357 lcd = TFT_HX8357(); //Call class

  //structs
  struct Point2d
  {
    int x=-90;
    int y=-90;
  }
  struct Line2d
  {
    Point2d pt1;
    Point2d pt2;
  }
  struct Rect2d
  {
    Point2d topRight;
    int h=1;
    int w=1;
  }
  struct Circle2d
  {
    Point2d center;
    int radius=1;
  }

  //array variables
  short pixelArray[480][320]; // x = 480 , y = 320
  int curOriginX[SCREEN_AMOUNT];
  int curOriginY[SCREEN_AMOUNT];

  //single variables
  Point2d dispCursor;

  Line2d underline;

  int16_t height;
  int16_t width;

  int buttonCurr = 0;
  int frame = 0;
  int state = 0;
  int increment = -2;
  int Xan, Yan;
  int Xoff, Yoff, Zoff;
  int newRenderLines;
  int oldRenderLines;

  char str_run[3] = "Run";
  char str_settings[8] = "Settings";
  char str_shutdown[8] = "Shutdown";


  //functions


  /* FIXME: Add button parameters  */
  void dispSelect(int *xCur, int *yCur, int *state)
  {
    //State selection
    //State -> BOOTUP
    if(&state == BOOTUP_STATE)
    {
      //Select Skip
      &state = MENU_STATE;
    }

    //State -> MAIN MENU
    else if(&state == MENU_STATE)
    {
      //Select Run
      if(() && () &&
         () && ())
      {
        &state = RUN_STATE
      }
      //Select Settings
      else if(() && () &&
              () && ())
      {
        &state = SETTINGS_STATE
      }
      //Select ShutDown
      else if(() && () &&
              () && ())
      {
        &state = SHUTDOWN_STATE
      }
    }

    //State -> SETTINGS
    else if(&state == SETTINGS_STATE)
    {
      //Select Back
      if(() && () &&
         () && ())
      {
        &state = MENU_STATE;
      }

      //Select Bool_2CubeMode
      else if(() && () &&
              () && ())
      {
        twoCubeMode = !twoCubeMode;
      }

      //Select Bool_ShowStats
      else if(() && () &&
              () && ())
      {
        showStats = !showStats;
      }

      //Select Bool_AutoParameters
      else if(() && () &&
              () && ())
      {
        autoParameters = !autoParameters;
      }

      //Select Adjust_CaseStart
      else if(() && () &&
              () && ())
      {
        caseStartSel = !caseStartSel;
      }

      //Select Adjust_PalletStart
      else if(() && () &&
              () && ())
      {
        palletStartSel = !palletStartSel;
      }
    }

    //State -> RUN
    else if(&state == RUN_STATE)
    {
      //Select Stop
      if(() && () &&
         () && ())
      {
        &state = STATS_STATE;
      }
      //Select Show_CurrStats
      else if(() && () &&
              () && ())
      {
        showStats = !showStats;
      }
    }

    //State -> ANALYTICS
    else if(&state == STATS_STATE)
    {
      //Select Return
      if(() && () &&
         () && ())
      {
        &state = MENU_STATE;
      }
    }
  };

  //3 columns
  void dispMoveRight(int *xCur)
  {
    //return xCur+1 if within bounds, otherwise nothing
    if(&xCur < 2)
    {
      if((!caseStartSel)&&(!palletStartSel))
      {
        &xCur++;
      }
    }
    //Settings Adjust
    if(&state == SETTINGS_STATE)
    {
      //Select Adjust_CaseStart
      if(caseStartSel)
      {
        if(caseStart_H < CASE_H_BOUNDS_POS)
        {
          caseStart_H = caseStart_H + 0.5;
        }
      }

      //Select Adjust_PalletStart
      else if(palletStartSel)
      {
        if(palletStart_H < PALLET_H_BOUNDS_POS)
        {
          palletStart_H = palletStart_H + 0.5;
        }
      }
    }
    
  };
  void dispMoveLeft(int *xCur)
  {
    //return xCur-1 if within bounds, otherwise nothing
    if(&xCur > 0)
    {
      if((!caseStartSel)&&(!palletStartSel))
      {
        &xCur--;
      }
    }
    //Settings Adjust
    if(&state == SETTINGS_STATE)
    {
      //Select Adjust_CaseStart
      if(caseStartSel)
      {
        if(caseStart_H > CASE_H_BOUNDS_NEG)
        {
          caseStart_H = caseStart_H - 0.5;
        }
      }

      //Select Adjust_PalletStart
      else if(palletStartSel)
      {
        if(palletStart_H > PALLET_H_BOUNDS_NEG)
        {
          palletStart_H = palletStart_H - 0.5;
        }
      }
    }
  };

  //Only used in settings state
  void dispMoveUp(int *yCur)
  {
    //Settings Adjust
    if(&state == SETTINGS_STATE)
    {
      //Select Adjust_CaseStart
      if(caseStartSel)
      {
        if(caseStart_V < CASE_V_BOUNDS_POS)
        {
          caseStart_V = caseStart_V + 0.5;
        }
      }

      //Select Adjust_PalletStart
      else if(palletStartSel)
      {
        if(palletStart_V < PALLET_V_BOUNDS_POS)
        {
          palletStart_V = palletStart_V + 0.5;
        }
      }
      else
      {
        &yCur == 0;
      }
    }
  };
  //Only used in settings state
  void dispMoveDown(int *yCur)
  {
    //Settings Adjust
    if(&state == SETTINGS_STATE)
    {
      //Select Adjust_CaseStart
      if(caseStartSel)
      {
        if(caseStart_V > CASE_V_BOUNDS_NEG)
        {
          caseStart_V = caseStart_V - 0.5;
        }
      }

      //Select Adjust_PalletStart
      else if(palletStartSel)
      {
        if(palletStart_V > PALLET_V_BOUNDS_NEG)
        {
          palletStart_V = palletStart_V - 0.5;
        }
      }
      else
      {
        &yCur == 0;
      }
    }
  };

  void dispHome(int *xCur, int *yCur, int *state)
  {
    //move to main menu

    &state = MENU_STATE;
    
    &xCur = curOriginX[MENU_STATE];
    &yCur = curOriginY[MENU_STATE];
    
  };

  void dispClearScreen()
  {
    //Clear screen
    lcd.fillScreen(TFT_BLACK);
  };

  //DRAWING FUNCTIONS:
    //drawPixel(uint16_t x, uint16_t y, uint16_t color),
    //drawChar(int16_t x, int16_t y, uint8_t c, uint16_t color, uint16_t bg, uint8_t font),
    //drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color),
    //fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color),
    //fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color),
    //fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color),
    //setRotation(uint8_t r),
  //

  //FIXME: Add frames for bootup
  //       Add Drawing for each screen
  void dispDrawScreen(int *stateIn, int *button)
  {
    //Set standards
    lcd.setTextDatum(MC_DATUM);
    lcd.setTextColor(WHITE, BLACK);
    
    //Just a blank screen
    if(&stateIn == SHUTDOWN_STATE)
    {
      lcd.fillScreen(TFT_BLACK);
      if(&button = POWER_ON)
      {
        &stateIn = BOOTUP_STATE;
      }
    }

    //FIXME: Needs Screen Drawn
    //FIXME: Needs animation
    //30 frame animation that moves case from a to b
    //Main Box:
    //  Top Left:     x=49, y=21
    //  Bottom Right: x=431, y=253
    else if(&stateIn == BOOTUP_STATE)
    {
      if(frame == 0)
      {

      }
      else
      {
        frame = 0;
        &state = MENU_STATE;
      }
    }

    //FIXME: Draw Menu Art
    //Main Box:
    //  Top Left:     x=49, y=21
    //  Bottom Right: x=431, y=253
    //Run Button
    //  Top Left:     x=45, y=273
    //  Bottom Right: x=164, y=313
    //Settings Button
    //  Top Left:     x=182, y=273
    //  Bottom Right: x=301, y=313
    //ShutDown Button
    //  Top Left:     x=319, y=273
    //  Bottom Right: x=438, y=313
    else if(&stateIn == MENU_STATE)
    {
      //Draw Menu screen
      //Insert art here

      //fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color),
      //Main Box
      //FIXME: Draw art
        lcd.drawRect(49, 21, 382, 232, TFT_WHITE);
      //

      //Run Button
        lcd.drawRect(45, 273, 119, 40, TFT_WHITE);
        //Spell Run
        lcd.drawString(str_run, 118, 290, 2);
      //

      //Settings Button
        lcd.drawRect(182, 273, 119, 40, TFT_WHITE);
        //Spell Settings
        lcd.drawString(str_settings, 265, 290, 2);
      //

      //ShutDown Button
        lcd.drawRect(319, 273, 119, 40, TFT_WHITE);
        //Spell Shutdown
        lcd.drawString(str_shutdown, 402, 290, 2);
      //
      //Cursor Selection
      //Run button
      if(&button = 0)
      {
        //Underline 
        underline.pt1.x = 45;
        underline.pt1.y = 313;
        underline.pt2.x = 164;
        underline.pt2.y = 313;
      }
      //Settings button
      else if(&button = 1)
      {
        //Underline 
        underline.pt1.x = 182;
        underline.pt1.y = 313;
        underline.pt2.x = 301;
        underline.pt2.y = 313;
      }
      //Shutdown button
      else
      {
        //Underline 
        underline.pt1.x = 319;
        underline.pt1.y = 313;
        underline.pt2.x = 438;
        underline.pt2.y = 313;
      }
      lcd.drawLine(underline.pt1.x, underline.pt1.y
                   underline.pt2.x, underline.pt2.y,
                   TFT_WHITE);
    }

    //FIXME: Draw Menu & Values
    else if(&stateIn == SETTINGS_STATE)
    {
      //TwoCubeToggle
      if(&button = 0)
      {
        //FIXME: Add underline locations
        //Underline 
        underline.pt1.x = ;
        underline.pt1.y = ;
        underline.pt2.x = ;
        underline.pt2.y = ;

        //FIXME: add check draw
      }
      //ShowStatsToggle
      else if(&button = 1)
      {
        //FIXME: Add underline locations
        //Underline 
        underline.pt1.x = ;
        underline.pt1.y = ;
        underline.pt2.x = ;
        underline.pt2.y = ;

        //FIXME: add check draw
      }
      //AutoParametersToggle
      else if(&button = 2)
      {
        //FIXME: Add underline locations
        //Underline 
        underline.pt1.x = ;
        underline.pt1.y = ;
        underline.pt2.x = ;
        underline.pt2.y = ;

        //FIXME: add check draw
      }
      //AdjustCaseStart
      else if(&button = 3)
      {
        //FIXME: Add underline locations
        //Underline 
        underline.pt1.x = ;
        underline.pt1.y = ;
        underline.pt2.x = ;
        underline.pt2.y = ;

        //FIXME: add value draw
      }
      //AdjustPalletStart
      else if(&button = 4)
      {
        //FIXME: Add underline locations
        //Underline 
        underline.pt1.x = ;
        underline.pt1.y = ;
        underline.pt2.x = ;
        underline.pt2.y = ;

        //FIXME: add value draw
      }
      lcd.drawLine(underline.pt1.x, underline.pt1.y
                   underline.pt2.x, underline.pt2.y,
                   TFT_WHITE);
    }

    //FIXME: Needs Screen Drawn
    else if(&stateIn == RUN_STATE)
    {
      //EmergencyStop
      if(&button = 0)
      {
        //FIXME: Add underline locations
        //Underline 
        underline.pt1.x = ;
        underline.pt1.y = ;
        underline.pt2.x = ;
        underline.pt2.y = ;
      }
      //ShowStats
      else if(&button = 1)
      {
        //FIXME: Add underline locations
        //Underline 
        underline.pt1.x = ;
        underline.pt1.y = ;
        underline.pt2.x = ;
        underline.pt2.y = ;
      }
      lcd.drawLine(underline.pt1.x, underline.pt1.y
                   underline.pt2.x, underline.pt2.y,
                   TFT_WHITE);
      //Draw The Screen
    }

    //FIXME: Needs Screen Drawn
    else if(&stateIn == STATS_STATE)
    {
      //FIXME: Add underline locations
      //Return
      underline.pt1.x = ;
      underline.pt1.y = ;
      underline.pt2.x = ;
      underline.pt2.y = ;
      lcd.drawLine(underline.pt1.x, underline.pt1.y
                   underline.pt2.x, underline.pt2.y,
                   TFT_WHITE);

      //FIXME: Draw Stats Screen
    }
  };

  //Pin values
  const int LCD_DB8 = 22;
  const int LCD_DB9 = 23;
  const int LCD_DB10 = 24;
  const int LCD_DB11 = 25;
  const int LCD_DB12 = 26;
  const int LCD_DB13 = 27;
  const int LCD_DB14 = 28;
  const int LCD_DB15 = 29;
  const int LCD_DB7 = 30;
  const int LCD_DB6 = 31;
  const int LCD_DB5 = 32;
  const int LCD_DB4 = 33;
  const int LCD_DB3 = 34;
  const int LCD_DB2 = 35;
  const int LCD_DB1 = 36;
  const int LCD_DB0 = 37;
  const int LCD_LCDRS = 38;
  const int LCD_LCDWR = 39;
  const int LCD_LCDCS = 40;
  const int LCD_LCDRST = 41;
  const int LCD_NC0 = 42;
  const int LCD_NC6 = 43;
  const int LCD_FLASHCS = 44;
  const int LCD_NC4 = 45;
  const int LCD_NC1 = 46;
  const int LCD_NC3 = 47;
  const int LCD_NC2 = 48;
  const int LCD_SPI_MISO = 49;
  const int LCD_SPI_MOSI = 50;
  const int LCD_SPI_CLK = 51;
  const int LCD_SC_CS = 52;

//
void setup() {
  // put your setup code here, to run once:
  // DPAD ************************************************************
    pinMode(button1, INPUT);
    pinMode(button2,INPUT);
    pinMode(button3,INPUT);
    pinMode(button4,INPUT);
    pinMode(button5,INPUT);
  //------------------------------------------------------------------

  // SERVO ************************************************************
    gripper.attach(servoSignal); //connect servo
    servoState = true;
  //------------------------------------------------------------------

  // STEPPERS ************************************************************
    //FIXME: Add stepper setup
  //------------------------------------------------------------------

  // TOF ************************************************************
    //FIXME: Add tof setup
  //------------------------------------------------------------------

  // LCD ************************************************************
    lcd.init();
    lcd.begin();
    lcd.fillScreen(BLACK);
    lcd.setRotation(1);
  
  //------------------------------------------------------------------


}


void loop() {
  
  //Reset Values
    zMotorSteps = 0;
    yMotorSteps = 0;
    moveServo = false;
  
  //Read the sensors

    //FIXME: ADD CODE FOR READING TOF

  //Read the buttons
    buttonState1 = digitalRead(button1);
    buttonState2 = digitalRead(button2);
    buttonState3 = digitalRead(button3);
    buttonState4 = digitalRead(button4);
    buttonState5 = digitalRead(button5);

  //Update the screen
    dispDrawScreen(*state, *buttonCurr);
  //Update the motors

    //Servo
    if(moveServo)
    {
      //FIXME: ADD CODE FOR MOVING SERVO
      //close servo
      if(servoState) 
      {
        gripper.write(SERVO_CLOSED);
        servoState = false;
      }
      //open servo
      else
      {
        gripper.write(SERVO_OPEN);
        servoState = true;
      }
    }

    //Z motor
    if(zMotorSteps != 0)
    {
      //FIXME: ADD CODE FOR MOVING STEPPER
    }

    //Y motor
    if(yMotorSteps != 0)
    {
      //FIXME: ADD CODE FOR MOVING STEPPER
    }

  delay(10); //delay 10ms for troubleshooting

}
