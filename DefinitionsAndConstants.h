//Author: Gideon Davis, Kaleb Sarlls

//This file is a header for constants and definitions

//TFT Colors
#ifndef DefinitionsAndConstants_h
#define DefinitionsAndConstants_h

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

//Servo states
#define SERVO_OPEN   6
#define SERVO_CLOSED 0

//Math constants
const float DEG_TO_RAD_CONSTANT = 57.2957795131; //conversion standard from deg to rad
const float PI = 3.141592653; //Pi to 10 digits

//Controls button pin values
const int BUTTON_5 = 2;
const int BUTTON_4 = 3;
const int BUTTON_3 = 4;
const int BUTTON_2 = 5;
const int BUTTON_1 = 6;

//Servo pin value
const int servoSignal = 7;

//Stepper pin values
const int zDriver_En = 8;    //Z
const int zDriver_Dir = 9;   //Z
const int zDriver_Step = 10; //Z
const int yDriver_En = 11;   //Y
const int yDriver_Dir = 12;  //Y
const int yDriver_Step = 13; //Y

//TOF pin values
const int zTof_SCL = 18; //Z
const int zTof_SDA = 19; //Z
const int yTof_SCL = 20; //Y
const int yTof_SDA = 21; //Y

//LCD pin values
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

#endif
