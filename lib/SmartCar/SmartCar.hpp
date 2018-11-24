#ifndef SMART_CAR
#define SMART_CAR

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <string>

using namespace std;

//
#define MANUFACTURER_SERVICE_UUID "0000180a-0000-1000-8000-00805F9B34FB"
#define MANUFACTURER_NAME_UUID "00002a29-0000-1000-8000-00805F9B34FB"
#define MANUFACTURER_NAME "Teodor Stanishev"
//

//
#define CAR_SERVICE_UUID "0000160a-0000-1000-8000-00805F9B34FB"
#define WINDOW_CHARACTERISTIC_UUID "00002a28-0000-1000-8000-00805F9B34FB"
#define LOG_CHARACTERISTIC_UUID "00002a30-0000-1000-8000-00805F9B34FB"

//
class BLECallback;
class SmartCar;

extern SmartCar car;

class SmartCar{
    private:
    BLEServer *pServer;
    BLEService *manufacturerService;
    BLECharacteristic *manufacturerName;
    BLEService *carService;
    BLECharacteristic *logChar;
    BLECharacteristic *buttonStateChar;
    BLECallback *pCallback;
    BLEAdvertising *pAdvertising;

    std::string deviceName = "Smart Car";

    uint8_t WINDOW_LEFT_DEFAULT = 0x50;
    uint8_t WINDOW_LEFT_UP = 0x51;
    uint8_t WINDOW_LEFT_DOWN = 0x52;
    uint8_t WINDOW_RIGHT_DEFAULT = 0x53;
    uint8_t WINDOW_RIGHT_UP = 0x54;
    uint8_t WINDOW_RIGHT_DOWN = 0x55;

    public:
    

    SmartCar();
    SmartCar(std::string deviceName);

    void begin();

    std::string getValues();
    void setValue(string s);
};


class BLECallback : public BLECharacteristicCallbacks{
    public:
    BLECallback(){}
    void onRead(BLECharacteristic *pCharacteristic);
    void onWrite(BLECharacteristic *pCharacteristic);
};

#endif