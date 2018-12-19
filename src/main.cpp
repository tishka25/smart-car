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

  car.notifyAll();
  // time_t currTime = time(nullptr);
  // Serial.print("Current time is: ");
  // Serial.println(ctime(&currTime));

  delay(1000);
}