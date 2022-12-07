

//include liquidCrystal_I2C.h 
//Tools -> Manage Libraries and type liquidCrystal_I2C

#include <LiquidCrystal_I2C.h>

//define I2C address......
LiquidCrystal_I2C lcd(0x27,16,2);


void setup() {
  lcd.init();
  lcd.clear();
  lcd.backlight();

  lcd.setCursor(2,0);
  lcd.print("Mobile");

  lcd.setCursor(2,1);
  lcd.print("Tracker");

}

void loop() {


}
