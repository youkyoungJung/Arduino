#include<SoftwareSerial.h>

SoftwareSerial bt(2,3);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  bt.begin(9600);
  Serial.println("20181013 ready");
}

void loop() {
  // put your main code here, to run repeatedly:
  if(bt.available()){
      Serial.write("YOUKYOUNG PHONE: ");
      Serial.write(bt.read());
      Serial.println();
     
    }
  if(Serial.available()){
      bt.write("PC: ");
      bt.write(Serial.read());
      bt.println(); 
    }
   
}
