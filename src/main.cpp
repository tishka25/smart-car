#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// #define CAR_SERVICE "aefe8d5e-3afd-4698-8bdb-cc8211963b98"
// #define CHARACTERISTIC_UUID "dc07e333-1e98-4054-a408-eb9b667c85e9"

#define MANUFACTURER_SERVICE "0000180a-0000-1000-8000-00805F9B34FB"
#define MANUFACTURER_NAME_UUID "00002a29-0000-1000-8000-00805F9B34FB"


#define CAR_SERVICE "0000160a-0000-1000-8000-00805F9B34FB"
#define CHARACTERISTIC_UUID "00002a28-0000-1000-8000-00805F9B34FB"

BLECharacteristic *buttonStateChar;

void setup(){
  Serial.begin(115200);
  //start the ble device with name "SmartCar"
  BLEDevice::init("SmartCar");
  BLEServer *pServer = BLEDevice::createServer();

  BLEService *manufacturerService = pServer->createService(MANUFACTURER_SERVICE);
  BLECharacteristic *manufacturerName = manufacturerService->createCharacteristic(MANUFACTURER_NAME_UUID , BLECharacteristic::PROPERTY_READ);


  //Create a service
  BLEService *carService = pServer->createService(CAR_SERVICE);
  //Create a chararteristic for that service 
  buttonStateChar = carService->createCharacteristic(
    CHARACTERISTIC_UUID , BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);

  //Set the default value
  manufacturerName->setValue("ELSYS");
  manufacturerService->start();

  //set default value for the button state and start the service
  uint8_t data= 0x56;
  buttonStateChar->setValue(&data ,sizeof(&data));
  carService->start();
  //Start advertising so you can search for it
  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();
}

void loop(){
  Serial.println(buttonStateChar->getValue().data());

  delay(500);
}