#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define MIN 75 //0
#define MAX 560 //180
#define RIGHT 315 //90

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pwm.begin();
  pwm.setPWMFreq(50);
}

void loop() {
  // put your main code here, to run repeatedly:
  pwm.setPWM(4, 0, RIGHT);
  delay(1000);


}
