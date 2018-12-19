#ifndef BLE_HPP
#define BLE_HPP

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include <sys/time.h>
#include <CharacteristicCallback.hpp>

#include <FreeRTOS.h>
#include <string>
#include <ctime>
#include <algorithm>
#include <vector>

using namespace std;

// //Manufacturer service and characteristics
// #define MANUFACTURER_SERVICE_UUID "0000180a-0000-1000-8000-00805F9B34FB"
// #define MANUFACTURER_NAME_UUID "00002a29-0000-1000-8000-00805F9B34FB"
// #define MANUFACTURER_NAME "Teodor Stanishev"
// //

// //Car service and characteristics
#define CAR_SERVICE_UUID "ca227c6b-d187-4aaf-b330-37144d84b02c"

#define PIN_CODE_CHARACRERISTIC_UUID "def231dc-07d4-4a71-b735-811e07d44c07" 
#define CHARACRERISTIC_UUID "3ff8860e-72ca-4a25-9c4e-99c7d3b08e9b" 

//

//Constants
#define WINDOW_LEFT     0
#define WINDOW_RIGHT    1
#define IGNITION        2
#define CENTRAL_LOCK    3
#define CLOCK           4
#define PASSWORD        5

// TODO make it a variable
// #define PIN_CODE "pticata"

//

class CharacteristicCallback;
class BLE;
class ServerCallbacks;




class BLE{
    private:
    BLEServer *pServer;
    BLEService *carService;
    BLECharacteristic *pCharacteristic;
    BLECharacteristic *pPassword;
    BLEAdvertising *pAdvertising;

    CharacteristicCallback *pCallback;

    string deviceName = "Smart Car";
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
    uint8_t UNLOCK = 0x75;
    //

    public:
    bool isConnected = false;
    
    struct{
        string PIN_CODE = "penis";
        byte MAX_PASSWORD_LENGTH = 5;
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
    string getState(void);
    string getState(uint8_t c);
    string getIgnitionState(void);
    string getWindowStates(void);
    string getCentralLockState(void);
    string getDate(void);

    BLEServer* getServer();
    BLECharacteristic* getMainCharacteristic();
    void setDefault();
    void notifyAll();

    static void blockedTimeout(void *p);
    void block();

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