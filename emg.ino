#include <Arduino_FreeRTOS.h>
#include <Wire.h>
#include <semphr.h>

int pin = A0;
int sEMG_val = 0;

SemaphoreHandle_t xSerialSemaphore;

void Task1(void *pvParameters);

void setup(){
  Serial.begin(9600);
  
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
}

void loop(){
}

void Task1(void *pvParameters __attribute__((unused))){

  (void) pvParameters;
  for(;;){
    sEMG_val = analogRead(pin);

    if(xSemaphoreTake(xSerialSemaphore, (TickType_t) 5) == pdTRUE){
      Serial.println(sEMG_val);
      xSemaphoreGive(xSerialSemaphore);
    }

    vTaskDelay(pdMS_TO_TICKS(50));
  }
}

