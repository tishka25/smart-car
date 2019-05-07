#include <Arduino.h>
#include <BLE.hpp>

//Global reference of the BLE class
BLE car;

void setup(){
  //Initialise the Serial monitor at the specified baud rate
  Serial.begin(115200);
  // pinMode(4 , OUTPUT);
  // pinMode(5 , OUTPUT);

  //Initialise the main BLE class
  car.begin();
}

void loop(){
  //Notify the current values if there are clients connected
  car.notifyAll();
  delay(1000);
}