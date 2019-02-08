#include <Arduino.h>
#include <WiFi.h>
#include <BLE.hpp>

BLE car;

void setup(){
  Serial.begin(115200);
  pinMode(21 , OUTPUT);

  WiFi.mode(WIFI_OFF);
  car.begin();
}

void loop(){
  car.notifyAll();
  // Serial.println("Left Window state: " + String(car.getWindowLeftState().data()));
  // (car.getWindowLeftState()) ? 
  // 
  delay(200);
  // digitalWrite(21 , LOW);
  // delay(200);
}