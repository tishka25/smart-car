#include "SmartCar.hpp"

using namespace std;

SmartCar::SmartCar(){}
SmartCar::SmartCar(std::string deviceName){
    this->deviceName = deviceName;
}

void SmartCar::begin(){
    //start the ble device with name "SmartCar"
    BLEDevice::init(deviceName);
    pServer = BLEDevice::createServer();
    manufacturerService = pServer->createService(MANUFACTURER_SERVICE_UUID);
    manufacturerName = manufacturerService->createCharacteristic(MANUFACTURER_NAME_UUID , BLECharacteristic::PROPERTY_READ);

    //Create a service
    carService = pServer->createService(CAR_SERVICE_UUID);
    //Create a chararteristic for that service 
    buttonStateChar = carService->createCharacteristic(WINDOW_CHARACTERISTIC_UUID , 
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
    // pCallback->setLogHandler(logChar);
    pCallback = new BLECallback();
    buttonStateChar->setCallbacks(pCallback);
    

    SmartCar::logChar = carService->createCharacteristic(LOG_CHARACTERISTIC_UUID ,
    BLECharacteristic::PROPERTY_READ
    | BLECharacteristic::PROPERTY_WRITE);

    //Set the default value
    manufacturerName->setValue(MANUFACTURER_NAME);
    manufacturerService->start();

    //set default value for the button state and start the service
    buttonStateChar->setValue(&WINDOW_LEFT_DEFAULT , sizeof(&WINDOW_LEFT_DEFAULT));

    carService->start();

    //Start advertising so you can search for it
    pAdvertising = pServer->getAdvertising();
    pAdvertising->start();
}

string SmartCar::getValues(){
    string buff;
    buff.append(buttonStateChar->getValue() + " : " + logChar->getValue());
    //
    return buff;
}

void SmartCar::setValue(string s){
    logChar->setValue(s);
}



void BLECallback::onRead(BLECharacteristic *pCharacteristic){
    Serial.println("I Have Read");
    //TODO
    car.setValue("ASD");
}
void BLECallback::onWrite(BLECharacteristic *pCharacteristic){
    Serial.println("I have written");
}


