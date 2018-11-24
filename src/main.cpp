#include <Arduino.h>
#include <SmartCar.hpp>

SmartCar car;

void setup(){
  Serial.begin(115200);
  car.begin();
}

void loop(){
  Serial.println(car.getValues().data());
  delay(500);
}