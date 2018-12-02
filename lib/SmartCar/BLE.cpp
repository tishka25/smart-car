#include "BLE.hpp"

using namespace std;

BLE::BLE(){}
BLE::BLE(std::string deviceName){
    this->deviceName = deviceName;
}

void BLE::begin(){
    //start the ble device with name "BLE"
    BLEDevice::init(deviceName);
    pServer = BLEDevice::createServer();
    pCallback = new Log();

    manufacturerService = pServer->createService(MANUFACTURER_SERVICE_UUID);
    manufacturerName = manufacturerService->createCharacteristic(MANUFACTURER_NAME_UUID , BLECharacteristic::PROPERTY_READ);

    //Create the service
    carService = pServer->createService(CAR_SERVICE_UUID);
    //Create the chararteristics for that service 
        //WINDOW LEFT
        windows[WINDOW_LEFT] = carService->createCharacteristic(WINDOW_LEFT_CHARACTERISTIC_UUID , 
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
        windows[WINDOW_LEFT]->setCallbacks(pCallback);
        //WINDOW RIGHT
        windows[WINDOW_RIGHT] = carService->createCharacteristic(WINDOW_RIGHT_CHARACTERISTIC_UUID , 
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
        windows[WINDOW_RIGHT]->setCallbacks(pCallback);
        //IGNITION
        ignition = carService->createCharacteristic(IGNITION_CHARACTERISTIC_UUID , 
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
        ignition->setCallbacks(pCallback);
    //

    BLE::logCharacteristic = carService->createCharacteristic(LOG_CHARACTERISTIC_UUID ,
    BLECharacteristic::PROPERTY_READ);
    logCharacteristic->setCallbacks(pCallback);

    //Set the default value
    manufacturerName->setValue(MANUFACTURER_NAME);
    manufacturerService->start();

    //set default value for the windows and start the service
    windows[WINDOW_LEFT]->setValue(&WINDOW_LEFT_DEFAULT , sizeof(&WINDOW_LEFT_DEFAULT));
    windows[WINDOW_RIGHT]->setValue(&WINDOW_RIGHT_DEFAULT , sizeof(&WINDOW_RIGHT_DEFAULT));
    carService->start();
    //

    //Start advertising so you can search for it
    pAdvertising = pServer->getAdvertising();
    pAdvertising->start();
}

void BLE::writeLog(string s){
    _log+=s + "\n";
    Serial.print(_log.data());

    //TODO
    logCharacteristic->setValue(_log);
}
void BLE::readLog(){
    Serial.println("READ LOG");
}

//DEBUG
vector<string> BLE::getValues(){
    vector<string>buff;
    buff.push_back(windows[WINDOW_LEFT]->getValue());
    buff.push_back(windows[WINDOW_RIGHT]->getValue());

    return buff;
}





void Log::onRead(BLECharacteristic *pCharacteristic){
    string log_uuid = LOG_CHARACTERISTIC_UUID;
    std::transform(log_uuid.begin(), log_uuid.end(), log_uuid.begin(), ::tolower);

    if(pCharacteristic->getUUID().toString() == log_uuid){
        car.readLog();
    }
}
void Log::onWrite(BLECharacteristic *pCharacteristic){
    string currValue = pCharacteristic->getValue();
    //Time
    string message("Date: ");
    message += asctime(localtime(&t));
    message.resize(message.size()-1);
    //
    message +=" ; ";
    //Characteristic modified
    message +=" Characteristic: ";
    message+= pCharacteristic->getUUID().toString();
    //
    //Changed to value
    message +=" Changed value: ";
    message+= string_to_hex(currValue);
    car.writeLog(message);
}


