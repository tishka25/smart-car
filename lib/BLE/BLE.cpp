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
    
    pSecurity = new Security(pServer , this);
    pCallback = new CharacteristicCallback(this , pSecurity);

    //Create the Car service
    carService = pServer->createService(CAR_SERVICE_UUID);
    //TEST max char value
    pCharacteristic = carService->createCharacteristic(CHARACRERISTIC_UUID,
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY);
    pCharacteristic->addDescriptor(new BLE2902());
    pCharacteristic->setCallbacks(pSecurity);

    pPassword = carService->createCharacteristic(PIN_CODE_CHARACRERISTIC_UUID ,
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
    pPassword->addDescriptor(new BLE2902());
    pPassword->setCallbacks(pSecurity);



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
    return pPassword->getValue();
}

void BLE::clearPinCode()
{
    // string buff = pCharacteristic->getValue().replace(4,pin.MAX_PASSWORD_LENGTH , "0");
    pPassword->setValue("0");
}
void BLE::setDefault()
{
    uint8_t data[] = {
        STANDARD , STANDARD , STANDARD , STANDARD
    };
    string buff(data , data + sizeof(data));
    buff+="0000000000";
    pCharacteristic->setValue(data , sizeof(data));
    BLE::clearPinCode();
}
void BLE::notifyAll()
{
    if (isConnected)
    {
        pCharacteristic->notify();
    }
}

BLECharacteristic* BLE::getCharacteristic(){
    return pCharacteristic;
}





void CharacteristicCallback::onRead(BLECharacteristic *pCharacteristic)
{
}
void CharacteristicCallback::onWrite(BLECharacteristic *pCharacteristic)
{
    // s->passwordHandler();
    Serial.println(pCharacteristic->getValue().data());
}
