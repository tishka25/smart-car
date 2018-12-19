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

    pPassword = carService->createCharacteristic(PIN_CODE_CHARACRERISTIC_UUID ,
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
    pPassword->addDescriptor(new BLE2902());
    pPassword->setCallbacks(pCallback);



    // BLE::setDefault();

    //Initial setup
    uint8_t data[] = {
        STANDARD , STANDARD , STANDARD , STANDARD
    };
    string buff(data , data + sizeof(data));
    buff+="0000000000";
    pCharacteristic->setValue(buff);
    //
    
    //
    carService->start();
    //

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
    return pPassword->getValue();
}
string BLE::getDate(){
    return pCharacteristic->getValue().substr(4,10);
}


void BLE::clearPinCode()
{
    pPassword->setValue("0");
}
void BLE::setDefault()
{
    uint8_t data[] = {
        STANDARD , STANDARD , STANDARD , STANDARD
    };
    string buff(data , data + sizeof(data));
    buff+=BLE::getDate();
    pCharacteristic->setValue(buff);
    BLE::clearPinCode();
}
void BLE::notifyAll()
{
    if (isConnected)
    {
        pCharacteristic->notify();
    }
}

BLEServer* BLE::getServer(){
    return pServer;
}
BLECharacteristic* BLE::getMainCharacteristic(){
    return pCharacteristic;
}