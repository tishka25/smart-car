#include <Arduino.h>
#include <BLE.hpp>

BLE car;

void setup(){
  Serial.begin(115200);
  pinMode(21 , OUTPUT);
  car.begin();
}

void loop(){
  car.notifyAll();
  
  delay(1000);
}