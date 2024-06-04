
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver abc = Adafruit_PWMServoDriver();

#define servoMIN 150
#define servoMAX 600

void setup(){
  Serial.begin(9600);
  abc.begin();
  abc.setPWMFreq(50);
  Wire.setClock(400000);
}

void loop(){
  
  
}
