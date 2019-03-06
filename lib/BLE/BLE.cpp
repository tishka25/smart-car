#include "BLE.hpp"

using namespace std;

BLE::BLE() {}
BLE::BLE(std::string deviceName)
{
    this->deviceName = deviceName;
}

/**
 * @brief Used to initialize the BLE with all the services and characteristic
 */
void BLE::begin()
{
    //Start the ble device with name "BLE"
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
    uint8_t commands[] = {
        STANDARD , STANDARD , STANDARD , STANDARD
    };
    string dateBuff(commands , commands + sizeof(commands));
    dateBuff+="00000000000"; //11 elements
    pCharacteristic->setValue(dateBuff);
    //
    
    //
    carService->start();
    //

    //Start advertising so you can search for it
    pAdvertising = pServer->getAdvertising();
    pAdvertising->start();
}


/**
 * @brief Returns the current Ignition state
 * @return string
 */
string BLE::getIgnitionState(){
    return pCharacteristic->getValue().substr(0,1);
}
/**
 * @brief Returns the current Window states
 * @return string
 */
string BLE::getWindowsStates(){
    return pCharacteristic->getValue().substr(1,2);
    
}
/**
 * @brief Returns the current Left Window state
 * @return string
 */
string BLE::getWindowLeftState(){
    return pCharacteristic->getValue().substr(1,1);
}
/**
 * @brief Returns the current Right Window state
 * @return string
 */
string BLE::getWindowRightState(){
    return pCharacteristic->getValue().substr(2,1);
}
/**
 * @brief Returns the current Lock State
 * @return string
 */
string BLE::getCentralLockState(){
    return pCharacteristic->getValue().substr(3,1);
}
/**
 * @brief Returns the current password entry from the BLE characteristic
 * @return string
 */
string BLE::getPinCode()
{
    return pPassword->getValue();
}

/**
 * @brief Returns a string representation of the seconds since Epoch
 * @return char[1-10] seconds
 */
string BLE::getDate(){
    return pCharacteristic->getValue().substr(4,10);
}
/**
 * @brief Returns the command for the Date/Time
 * @return uint8_t command
 */
uint8_t BLE::getDateCommand(){
    return atoi(pCharacteristic->getValue().substr(14,1).c_str());
}
/**
 * @brief Used to clear the current password entry
 */
void BLE::clearPinCode()
{
    pPassword->setValue("0");
}
/**
 * @brief Set the default states 
 */
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
void BLE::setDateCommand(string c){
    string buff = pCharacteristic->getValue().replace(10,1,c);
    pCharacteristic->setValue(buff);
}
/**
 * @brief Overloaded function to notify the BLE client for all the states
 */
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