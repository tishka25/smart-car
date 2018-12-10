#include <Arduino.h>
#include <WiFi.h>
#include <BLE.hpp>

BLE car;

void setup(){
  Serial.begin(115200);
  WiFi.mode(WIFI_OFF);
  car.begin();
}

void loop(){
  // for(auto v : car.getValues()){
  //   Serial.println(v.data());
  // }
  car.notifyAll();

  delay(500);
}