#include <TFT_HX8357.h>

TFT_HX8357 lcd = TFT_HX8357(); //Call class

#define WHITE     0xFFFF
#define BLACK     0x0000

char str_run[4] = "Run";
char str_settings[9] = "Settings";
char str_shutdown[9] = "Shutdown";
char str_teamX[8] = "Team X";
char str_robotics[10] = "Packaging";

void run()
{
      
      lcd.setTextDatum(MC_DATUM);
      lcd.setTextColor(WHITE, BLACK);
      

      //FIXME: Draw art
        lcd.drawRect(49, 21, 382, 232, TFT_WHITE);
        lcd.drawString(str_teamX, 235, 100, 4);
        lcd.drawString(str_robotics, 235, 140, 4);
      //
      lcd.setTextDatum(MR_DATUM);
      
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
}

void setup() {
  // put your setup code here, to run once:
  lcd.init();
  lcd.begin();
  lcd.fillScreen(BLACK);
  lcd.setRotation(1);
}

void loop() {
  // put your main code here, to run repeatedly:
  run();
}
