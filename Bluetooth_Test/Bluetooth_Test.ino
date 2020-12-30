#include<SoftwareSerial.h>
#include<String.h>

SoftwareSerial BTSerial(2,3);
byte buffer[1024];
byte temp[16];
byte value;
int count;

int tempPosition;
int bufferPosition;

void setup() {
  BTSerial.begin(9600);
  Serial.begin(9600);

  tempPosition = 0;
  bufferPosition=0;
  count = 0;
  Serial.println("20181013 you");
}

void loop() {
  /*  if(BTSerial.available()){
      byte data = Serial.read();
      BTSerial.write(data);
      buffer[bufferPosition++]=data;
    
    if(data=='\n'){
        buffer[bufferPosition]='\0';
       //시리얼로 문자열 전송
        Serial.write(buffer, bufferPosition);
        bufferPosition=0;
    }
  }*/
   if(Serial.available()){
    value = Serial.read();
    Serial.write(value);
    Serial.print(",");
    Serial.println(tempPosition);
    temp[tempPosition++]=value;
    count = 16-tempPosition;
 
    if(value=='\n'){    
      for(int i = 0; i < count; i++){
        temp[(1+i+count)]='x';
        tempPosition++;
      } 
       temp[tempPosition+1] = '\0';         
    } 
  }  
    if(tempPosition == 16){
      BTSerial.println("Start\n");
      BTSerial.write(temp, tempPosition);
      Serial.write(temp, tempPosition);
      tempPosition=0;
  }
}
