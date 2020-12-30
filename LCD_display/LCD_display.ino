#include<Wire.h>
#include<LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16,2);

void setup() {
  // put your setup code here, to run once:
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()){
    delay(100);

    lcd.clear();

    String szTemp;
    while(Serial.available()>0){
      char cRead=Serial.read();
      szTemp += cRead;
      }
      if(szTemp){
        lcd.print(szTemp);
      }
    }
}
