/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/

#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

void setup() {
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  Serial.begin(9600);
  Serial.println("Servo Motor Version 1");
}

void loop() {
  
  if(Serial.available() ){
    
    String szTemp;
     while(Serial.available() > 0){
       char cRead = Serial.read();
       szTemp += cRead;
     }
    
    int input = szTemp.toInt();
    myservo.write(input);

    Serial.print("Move to: "); Serial.println(input);
  }
  delay(1000);
}

/*
version2
 
 if(Serial.available() ){
    
    int input = Serial.parseInt();

    if(Serial.read() == '\n'){
        myservo.write(input);
        Serial.print("Move to: ");Serial.println(input);
      }
  } 
  delay(1000);

*/
