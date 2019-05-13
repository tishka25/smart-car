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
    BLE::initializePins();
    //Start the ble device with name "BLE"
    BLEDevice::init(deviceName);
    //TODO Change power level for RELEASE
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

    //TODO change the permission for RELEASE
    pPassword = carService->createCharacteristic(PIN_CODE_CHARACRERISTIC_UUID,
                                                 BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
    pPassword->addDescriptor(new BLE2902());
    pPassword->setCallbacks(pCallback);

    // BLE::setDefault();

    //Initial setup
    uint8_t commands[] = {
        STANDARD, STANDARD, STANDARD, STANDARD};
    string dateBuff(commands, commands + sizeof(commands));
    dateBuff += "00000000000"; //11 elements
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
 * @brief Initializes the pins , calls pinMode() on every GPIO that is going to be used
 */
void BLE::initializePins()
{
    for (auto pin : pins)
    {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, LOW);
    }
}
/**
 * @brief Returns the current Ignition state
 * @return string
 */
string BLE::getIgnitionState()
{
    return pCharacteristic->getValue().substr(0, 1);
}
/**
 * @brief Returns the current Window state
 * @return string
 */
string BLE::getWindowState()
{
    return pCharacteristic->getValue().substr(1, 1);
}
/**
 * @brief Returns the current Lock State
 * @return string
 */
string BLE::getCentralLockState()
{
    return pCharacteristic->getValue().substr(2, 1);
}
/**
 * @brief Returns the current Trunk state
 * @return string
 */
string BLE::getTrunkState()
{
    return pCharacteristic->getValue().substr(3, 1);
}
/**
 * @brief Returns a string representation of the seconds since Epoch
 * @return char[10] seconds
 */
string BLE::getDate()
{
    return pCharacteristic->getValue().substr(4, 10);
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
 * @brief Returns the command for the Date/Time
 * @return uint8_t command
 */
uint8_t BLE::getDateCommand()
{
    return atoi(pCharacteristic->getValue().substr(14, 1).c_str());
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
        STANDARD, STANDARD, STANDARD, STANDARD};
    string buff(data, data + sizeof(data));
    buff += BLE::getDate();
    pCharacteristic->setValue(buff);
    BLE::clearPinCode();
}
/**
 * @brief Method that sets the last byte of the message. 
 * This byte if the date command
 * Mostly used to set the date command to DEFAULT
 * @param {string} c - the command to be set to
  */
void BLE::setDateCommand(string c)
{
    string buff = pCharacteristic->getValue().replace(10, 1, c);
    pCharacteristic->setValue(buff);
}
/**
 * @brief Method to set of update the string value of only the date
 * @param {string} d - second since Epoch in string format (d must be 10 characters long)
 */
void BLE::setDate(string d){
    string buff = pCharacteristic->getValue().replace(4, 10 , d);
    pCharacteristic->setValue(buff);
}
/**
 * @brief Overloaded method to notify the BLE client for all the states
 */
void BLE::notifyAll()
{
    if (isConnected)
    {
        time_t currTime = time(nullptr);
        Serial.print("Current time is   : ");
        Serial.println(ctime(&currTime));
        std::stringstream ss;
        ss << currTime;
        if(isPasswordCorrect)
            BLE::setDate(ss.str());
        pCharacteristic->notify();
    }
}
/**
 * @brief Method that returns a pointer current BLE server
 */
BLEServer *BLE::getServer()
{
    return pServer;
}
/**
 * @brief Method that returns a pointer to the main BLE characteristic
 */
BLECharacteristic *BLE::getMainCharacteristic()
{
    return pCharacteristic;
}

void BLE::triggerCentralLocking(){
    //Central locking
    digitalWrite(CENTRAL_LOCKING_TRIGG_PIN , HIGH);
    delay(200);
    digitalWrite(CENTRAL_LOCKING_TRIGG_PIN , LOW);
    delay(200);
}

void BLE::windowsUp(){
    // //Window up
    // if(c->getWindowState().data()[0] == c->WINDOW_UP){
    //     digitalWrite(WINDOW_UP_PIN , HIGH);
    // }else{
    //     digitalWrite(WINDOW_UP_PIN , LOW);
    // } 
}

void BLE::windowsDown(){
    //Window down
    // if(getWindowState().data()[0] == c->WINDOW_DOWN){
    //     digitalWrite(WINDOW_DOWN_PIN , HIGH);
    // }else{
    //     digitalWrite(WINDOW_DOWN_PIN , LOW);
    // }
}
void BLE::triggerTrunk(){
    //Trunk
    digitalWrite(OPEN_TRUNK_PIN , HIGH);
    delay(200);
    digitalWrite(OPEN_TRUNK_PIN , LOW);
    delay(200);
}
void BLE::ignitionOn(){
    digitalWrite(IGNITION_PIN , HIGH);
}
void BLE::ignitionOff(){
    digitalWrite(IGNITION_PIN , LOW);
}
void BLE::engineCrankOn(){
    digitalWrite(STARTER_MOTOR_PIN , HIGH);
}
void BLE::engineCrankOff(){
    digitalWrite(STARTER_MOTOR_PIN , LOW);
}
