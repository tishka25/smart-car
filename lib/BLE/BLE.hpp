#ifndef BLE_HPP
#define BLE_HPP

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

#include <FreeRTOS.h>
#include <string>
#include <ctime>
#include <algorithm>
#include <vector>

using namespace std;

//Manufacturer service and characteristics
#define MANUFACTURER_SERVICE_UUID "0000180a-0000-1000-8000-00805F9B34FB"
#define MANUFACTURER_NAME_UUID "00002a29-0000-1000-8000-00805F9B34FB"
#define MANUFACTURER_NAME "Teodor Stanishev"
//

//Car service and characteristics
#define CAR_SERVICE_UUID "ca227c6b-d187-4aaf-b330-37144d84b02c"
#define WINDOW_LEFT_CHARACTERISTIC_UUID "3ff8860e-72ca-4a25-9c4e-99c7d3b08e9b"
#define WINDOW_RIGHT_CHARACTERISTIC_UUID "96cc6576-b9b0-443b-b8e6-546bbd20d374"
#define IGNITION_CHARACTERISTIC_UUID "4ad1bbf1-b3e6-4239-a3bb-520624ee1329"
#define LOCK_CONTROL_CHARACTERISTIC_UUID "aface04c-7963-43ec-a172-bedeb1b49570"
#define CLOCK_CHARACTERISTIC_UUID "6bbdd85f-c398-4075-abad-62d3ba40916a"

#define LOG_CHARACTERISTIC_UUID "9c64b5fd-9d4c-49fe-99b0-b9cf4f091026"
#define PIN_CODE_CHARACRERISTIC_UUID "def231dc-07d4-4a71-b735-811e07d44c07" 
//

//Constants
#define WINDOW_LEFT 0
#define WINDOW_RIGHT 1

// TODO make it a variable
// #define PIN_CODE "pticata"

//

class CharacteristicCallback;
class BLE;
class ServerCallbacks;



class BLE{
    private:
    BLEServer *pServer;
    BLEService *manufacturerService;
    BLECharacteristic *manufacturerName;
    BLEService *carService;
    BLECharacteristic *windows[2];
    BLECharacteristic *ignition;
    BLECharacteristic *lockControl;
    BLECharacteristic *rtc_clock;

    
    //CharacteristicCallback characteristic (part of the car service)
    BLECharacteristic *logCharacteristic;
    CharacteristicCallback *pCallback;
    //

    BLEAdvertising *pAdvertising;

    std::string deviceName = "Smart Car";
    
    //TODO 
    //Write this to flash memory
    string _log = "";
    //

    //Defailt state
    uint8_t STANDARD = 0x50;
    //
    //Window commands
    uint8_t WINDOW_LEFT_UP = 0x51;
    uint8_t WINDOW_LEFT_DOWN = 0x52;
    uint8_t WINDOW_RIGHT_UP = 0x54;
    uint8_t WINDOW_RIGHT_DOWN = 0x55;
    //
    //Ignition
    uint8_t IGNITION_ON = 0x60;
    uint8_t IGNITION_OFF = STANDARD;
    uint8_t IGNITION_START = 0x62;
    //Lock/unlock
    uint8_t LOCK = STANDARD;
    uint8_t UNLOCK = 0x81;
    //

    public:
    bool isConnected = false;
    
    struct{
        BLECharacteristic *characteristic;
        string PIN_CODE = "pticata";
        byte failedEntries = 0;
        byte MAX_FAILED_ENTRIES = 3;
    }pin;

    BLE();
    BLE(std::string deviceName);

    void begin();

    void readLog();
    void writeLog(string s);
    //Debug
    vector<string> getValues();
    string getPinCode();
    void clearPinCode();

    void setDefaultAll();
    void notifyAll();

    static void blockedTimeout(void *p);
    void block();

};


class CharacteristicCallback : public BLECharacteristicCallbacks{
    private:
    std::time_t t = std::time(nullptr);
    //BLE car reference
    BLE *c;

  public:
    CharacteristicCallback(BLE *c)
    {
        this->c = c;
    }
    void onRead(BLECharacteristic *pCharacteristic);
    void onWrite(BLECharacteristic *pCharacteristic);
    
    std::string string_to_hex(const std::string &input)
    {
        static const char *const lut = "0123456789ABCDEF";
        size_t len = input.length();

        std::string output;
        output.reserve(2 * len);
        for (size_t i = 0; i < len; ++i)
        {
            const unsigned char c = input[i];
            output.push_back(lut[c >> 4]);
            output.push_back(lut[c & 15]);
        }
        return output;
    }
};

class ServerCallbacks : public BLEServerCallbacks
{
  public:
    BLE *c;
    ServerCallbacks(BLE *c)
    {
        this->c = c;
    }
    void onConnect(BLEServer *pServer)
    {
        c->isConnected = true;
        Serial.println(c->isConnected);
    }
    void onDisconnect(BLEServer *pServer)
    {
        c->isConnected = false;
        //Clear the current PIN_CODE
        c->clearPinCode();
        //
        Serial.println(c->isConnected);
    }
};

#endif