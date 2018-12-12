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
    //
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new ServerCallbacks(this));
    pCallback = new CharacteristicCallback(this);

    manufacturerService = pServer->createService(MANUFACTURER_SERVICE_UUID);
    manufacturerName = manufacturerService->createCharacteristic(MANUFACTURER_NAME_UUID, BLECharacteristic::PROPERTY_READ);

    //Create the service
    carService = pServer->createService(CAR_SERVICE_UUID);
    //Create the chararteristics for that service
    //WINDOW LEFT
    windows[WINDOW_LEFT] = carService->createCharacteristic(WINDOW_LEFT_CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY);
    windows[WINDOW_LEFT]->addDescriptor(new BLE2902());
    windows[WINDOW_LEFT]->setCallbacks(pCallback);
    //WINDOW RIGHT
    windows[WINDOW_RIGHT] = carService->createCharacteristic(WINDOW_RIGHT_CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY);
    windows[WINDOW_RIGHT]->addDescriptor(new BLE2902());
    windows[WINDOW_RIGHT]->setCallbacks(pCallback);
    //IGNITION
    ignition = carService->createCharacteristic(IGNITION_CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY);
    ignition->addDescriptor(new BLE2902());
    ignition->setCallbacks(pCallback);
    //LOCK CONTROL
    lockControl = carService->createCharacteristic(LOCK_CONTROL_CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY);
    lockControl->addDescriptor(new BLE2902());
    lockControl->setCallbacks(pCallback);
    //
    //CLOCK
    rtc_clock = carService->createCharacteristic(CLOCK_CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
    rtc_clock->addDescriptor(new BLE2902());
    rtc_clock->setCallbacks(pCallback);
    //
    //PIN CODE
    pin.characteristic = carService->createCharacteristic(PIN_CODE_CHARACRERISTIC_UUID,
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
    pin.characteristic->addDescriptor(new BLE2902());
    pin.characteristic->setCallbacks(pCallback);
    //

    BLE::logCharacteristic = carService->createCharacteristic(LOG_CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_READ);
    logCharacteristic->addDescriptor(new BLE2902());
    logCharacteristic->setCallbacks(pCallback);

    //Set the default value
    manufacturerName->setValue(MANUFACTURER_NAME);
    manufacturerService->start();

    //set default value for the windows and start the service
    BLE::setDefaultAll();

    carService->start();
    //

    //Start advertising so you can search for it
    pAdvertising = pServer->getAdvertising();
    pAdvertising->start();
}

void BLE::writeLog(string s)
{
    _log += s + "\n";
    Serial.print(_log.data());

    //TODO
    logCharacteristic->setValue(_log);
}
void BLE::readLog()
{
    Serial.println("READ LOG");
}

//DEBUG
vector<string> BLE::getValues()
{
    vector<string> buff;
    buff.push_back(windows[WINDOW_LEFT]->getValue());
    buff.push_back(windows[WINDOW_RIGHT]->getValue());

    return buff;
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
    windows[WINDOW_LEFT]->setValue(&STANDARD, sizeof(&STANDARD));
    windows[WINDOW_RIGHT]->setValue(&STANDARD, sizeof(&STANDARD));
    ignition->setValue(&STANDARD, sizeof(&STANDARD));
    lockControl->setValue(&LOCK , sizeof(&LOCK));
}
void BLE::notifyAll()
{
    if (isConnected)
    {
        windows[WINDOW_LEFT]->notify();
        windows[WINDOW_RIGHT]->notify();
        ignition->notify();
        lockControl->notify();
    }
}

void BLE::blockedTimeout(void *p)
{
    vTaskDelay(10000);
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
    string buffUUID = pCharacteristic->getUUID().toString();
    // if (buffUUID == PIN_CODE_CHARACRERISTIC_UUID)
    // {
        if (c->getPinCode() != c->pin.PIN_CODE)
        {
            c->setDefaultAll();
            c->pin.failedEntries++;
            Serial.print("Failed entries: ");
            Serial.println(c->pin.failedEntries);
            c->block();

        }
        else if (c->getPinCode() == c->pin.PIN_CODE)
        {
            Serial.println("Password Correct!");
            //Clear the failed attempts
            c->pin.failedEntries = 0;
            //
        }
    // }
    if (c->pin.failedEntries >= c->pin.MAX_FAILED_ENTRIES)
        c->block();

    Serial.print(string_to_hex(pCharacteristic->getValue()).data());
    Serial.println();
}
