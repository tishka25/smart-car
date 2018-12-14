#include "BLE.hpp"

using namespace std;

BLE::BLE() {}
BLE::BLE(std::string deviceName)
{
    this->deviceName = deviceName;
}

void BLE::begin()
{
    //start the ble device with name "BLE"
    BLEDevice::init(deviceName);
    //TOO Change power level for RELEASE
    BLEDevice::setPower(ESP_PWR_LVL_P7);

    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new ServerCallbacks(this));
    pCallback = new CharacteristicCallback(this);
    //Create the Car service
    carService = pServer->createService(CAR_SERVICE_UUID);
    //TEST max char value
    pCharacteristic = carService->createCharacteristic(CHARACRERISTIC_UUID,
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY);
    pCharacteristic->addDescriptor(new BLE2902());
    pCharacteristic->setCallbacks(pCallback);


    BLE::setDefault();

    carService->start();
    // //

    //Start advertising so you can search for it
    pAdvertising = pServer->getAdvertising();
    pAdvertising->start();
}



string BLE::getIgnitionState(){
    return pCharacteristic->getValue().substr(0,1);
}
string BLE::getWindowStates(){
    return pCharacteristic->getValue().substr(1,2);
}
string BLE::getCentralLockState(){
    return pCharacteristic->getValue().substr(3,1);
}
string BLE::getPinCode()
{
    return pCharacteristic->getValue().substr(4,pin.MAX_PASSWORD_LENGTH);
}

void BLE::clearPinCode()
{
    string buff = pCharacteristic->getValue().replace(4,pin.MAX_PASSWORD_LENGTH , "0");

}
void BLE::setDefault()
{
    uint8_t data[] = {
        STANDARD , STANDARD , STANDARD , STANDARD , '0'
    };
    pCharacteristic->setValue(data , sizeof(data));
}
void BLE::notifyAll()
{
    if (isConnected)
    {
        pCharacteristic->notify();
    }
}


void BLE::block()
{
    pServer->disconnectClient();
    // xTaskCreate(blockedTimeout, "Blocked timeout", 1024, (void *)1, tskIDLE_PRIORITY, NULL);
}





void CharacteristicCallback::onRead(BLECharacteristic *pCharacteristic)
{
}
void CharacteristicCallback::onWrite(BLECharacteristic *pCharacteristic)
{
    string pass = c->getPinCode();
    if(pass != c->pin.PIN_CODE){
        c->setDefault();
        c->pin.failedEntries++;
        Serial.print("Failed entries: ");
        Serial.println(c->pin.failedEntries);
        c->block();
    }else if (pass == c->pin.PIN_CODE){
        Serial.println("Password Correct!");
        //Clear the failed attempts
        c->pin.failedEntries = 0;
        //
    }

    if (c->pin.failedEntries >= c->pin.MAX_FAILED_ENTRIES)
        c->block();
        

    Serial.println(pCharacteristic->getValue().data());
}

void ClockCallback::onRead(BLECharacteristic *pCharacteristic)
{
    
}
void ClockCallback::onWrite(BLECharacteristic *pCharacteristic){
    string bleTime = pCharacteristic->getValue();
    if(bleTime!="0"){
        struct tm tm;
        tm.tm_year = atoi(bleTime.substr(0,3).c_str()) - 1900;
        tm.tm_mon = atoi(bleTime.substr(5,6).c_str()) - 1;
        tm.tm_mday = atoi(bleTime.substr(8,9).c_str());
        tm.tm_hour = atoi(bleTime.substr(11,12).c_str());
        tm.tm_min = atoi(bleTime.substr(14,15).c_str());
        t = mktime(&tm);
        struct timeval now = { .tv_sec = t };
        settimeofday(&now , NULL);
    }
}
