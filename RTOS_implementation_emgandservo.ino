#include <Arduino_FreeRTOS.h>
#include <Wire.h>
#include <semphr.h>
#include <Adafruit_PWMServoDriver.h>

#define MIN 75 //0°
#define MAX 560 // 180°

int pin = A0;
int sEMG_val = 0;

int testvar = 1000;

Adafruit_PWMServoDriver servoDriver = Adafruit_PWMServoDriver();

SemaphoreHandle_t xSerialSemaphore;

void Task1(void *pvParameters);
void Task2(void *pvParameters);

void setup(){
  Serial.begin(9600);

  servoDriver.begin();
  servoDriver.setPWMFreq(50);
  
  while(!Serial){
    ;
  }

  if(xSerialSemaphore == NULL){
    xSerialSemaphore = xSemaphoreCreateMutex();
    if((xSerialSemaphore) != NULL){
      xSemaphoreGive((xSerialSemaphore));
    }
  }

  xTaskCreate(Task1, "sEMGSensor", 256, NULL, 1, NULL);
  xTaskCreate(Task2, "servoTask", 128, NULL, 1, NULL); 

  vTaskStartScheduler();
}

void loop(){
}

void Task1(void *pvParameters __attribute__((unused))){
  (void) pvParameters;

  const int duration = 500; //ms
  const int amount =  duration/50; //amount every 50ms

  int sEMG_val_array[amount];

  for(;;){
    sEMG_val = analogRead(pin);
    int sum = 0;

    for(int i = 0; i < amount; i++){
      sEMG_val_array[i] = analogRead(pin);
      sum += sEMG_val_array[i]
      vTaskDelay(pdMS_TO_TICKS(50));
    }

    int sEMG_val = sum / amount;

    if(xSemaphoreTake(xSerialSemaphore, (TickType_t) 5) == pdTRUE){
      Serial.println(sEMG_val);
      xSemaphoreGive(xSerialSemaphore);
    }

    vTaskDelay(pdMS_TO_TICKS(50));
  }
}

void Task2(void *pvParameters __attribute((unused))){
  (void) pvParameters;

  for(;;){
    int sEMG_val_local = sEMG_val;

    if(xSemaphoreTake(xSerialSemaphore, (TickTye_t) 5) == pdTRUE){
      int angle = map(sEMG_val_local, 0, 1023, MIN, MAX);
      servoDriver.setPWM(0, 0, angle);
      servoDriver.setPWM(1, 0, angle);
      servoDriver.setPWM(2, 0, angle);
      servoDriver.setPWM(3, 0, angle);
      servoDriver.setPWM(4, 0, angle);

      vTaskDelay(pdMS_TO_TICKS(500));

      xSemaphoreGive(xSerialSemaphore);
    }
    
    vTaskDelay(pdMS_TO_TICKS(50));
  }
}

