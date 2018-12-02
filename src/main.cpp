#include <Arduino.h>
#include <BLE.hpp>

BLE car;

void setup(){
  Serial.begin(115200);
  car.begin();
}

void loop(){
  for(auto v : car.getValues()){
    Serial.println(v.data());
  }

  delay(500);
}