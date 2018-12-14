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
    //Change power level for RELEASE
    BLEDevice::setPower(ESP_PWR_LVL_P7);
    // BLEDevice::setMTU(500);
    //
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new ServerCallbacks(this));
    pCallback = new CharacteristicCallback(this);


    // manufacturerService = pServer->createService(MANUFACTURER_SERVICE_UUID);
    // manufacturerName = manufacturerService->createCharacteristic(MANUFACTURER_NAME_UUID, BLECharacteristic::PROPERTY_READ);
    //Create the Car service
    carService = pServer->createService(CAR_SERVICE_UUID);
    //TEST max char value
    pCharacteristic = carService->createCharacteristic(CHARACRERISTIC_UUID,
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY);
    pCharacteristic->addDescriptor(new BLE2902());
    pCharacteristic->setCallbacks(pCallback);


    pCharacteristic->setValue("1532");

    carService->start();
    // //

    //Start advertising so you can search for it
    pAdvertising = pServer->getAdvertising();
    pAdvertising->start();
}

string BLE::getPinCode()
{
    return pin.characteristic->getValue();
}
void BLE::clearPinCode()
{
    pin.characteristic->setValue("0");
}

void BLE::setDefaultAll()
{
    
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
    // string buffUUID = pCharacteristic->getUUID().toString();
    // // if (buffUUID == PIN_CODE_CHARACRERISTIC_UUID)
    // // {
    //     if (c->getPinCode() != c->pin.PIN_CODE)
    //     {
    //         c->setDefaultAll();
    //         c->pin.failedEntries++;
    //         Serial.print("Failed entries: ");
    //         Serial.println(c->pin.failedEntries);
    //         c->block();

    //     }
    //     else if (c->getPinCode() == c->pin.PIN_CODE)
    //     {
    //         Serial.println("Password Correct!");
    //         //Clear the failed attempts
    //         c->pin.failedEntries = 0;
    //         //
    //     }
    // // }
    // if (c->pin.failedEntries >= c->pin.MAX_FAILED_ENTRIES)
    //     c->block();

    Serial.print((pCharacteristic->getValue()).data());
    Serial.println();
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
