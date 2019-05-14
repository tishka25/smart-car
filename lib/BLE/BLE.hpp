#ifndef BLE_HPP
#define BLE_HPP

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include <CharacteristicCallbacks.hpp>
#include <ServerCallbacks.hpp>
#include <sys/time.h>
#include <FreeRTOS.h>
#include <string>
#include <sstream>
#include <ctime>
#include <algorithm>
#include <vector>

using namespace std;


// //Car service and characteristics
#define CAR_SERVICE_UUID "ca227c6b-d187-4aaf-b330-37144d84b02c"

#define PIN_CODE_CHARACRERISTIC_UUID "def231dc-07d4-4a71-b735-811e07d44c07" 
#define CHARACRERISTIC_UUID "3ff8860e-72ca-4a25-9c4e-99c7d3b08e9b" 
//

//GPIO pins
#define WINDOW_UP_PIN               (uint8_t)4
#define WINDOW_DOWN_PIN             (uint8_t)5
#define OPEN_TRUNK_PIN              (uint8_t)14
#define IGNITION_PIN                (uint8_t)15
#define CENTRAL_LOCKING_TRIGG_PIN   (uint8_t)12
#define STARTER_MOTOR_PIN           (uint8_t)17
//

//Constants
#define DEFAULT_DATE_COMMAND    0
#define SET_TIME_ON_RTC         1
// #define IGNITION                "2'
#define CENTRAL_LOCK            3


class CharacteristicCallbacks;
class BLE;
class ServerCallbacks;


class BLE{
    private:
    BLEServer *pServer;
    BLEService *carService;
    BLECharacteristic *pCharacteristic;
    BLECharacteristic *pPassword;
    BLEAdvertising *pAdvertising;

    CharacteristicCallbacks *pCallback;

    string deviceName = "Smart Car";

    uint8_t pins[6] = {WINDOW_UP_PIN , WINDOW_DOWN_PIN ,
    IGNITION_PIN , CENTRAL_LOCKING_TRIGG_PIN , STARTER_MOTOR_PIN , OPEN_TRUNK_PIN};

    public:
    //Defailt state
    uint8_t STANDARD = 0x50;
    //
    //Window commands
    uint8_t WINDOW_UP = 0x51;
    uint8_t WINDOW_DOWN = 0x52;
    //Ignition
    uint8_t IGNITION_ON = 0x60;
    uint8_t IGNITION_OFF = STANDARD;
    uint8_t IGNITION_STARTER_ON = 0x62;
    uint8_t IGNITION_STARTER_OFF = 0x63;
    //Lock/unlock
    uint8_t LOCK = STANDARD;
    uint8_t UNLOCK = 0x75;
    //
    //Open trunk
    uint8_t OPEN_TRUNK = 0x77;
    //

    bool isConnected = false;
    bool isPasswordCorrect = false;
    
    struct{
        string PIN_CODE = "elsys";
        byte failedEntries = 0;
        byte MAX_FAILED_ENTRIES = 3;
    }pin;


    BLE();
    BLE(std::string deviceName);

    void begin();
    void initializePins();

    void readLog();
    void writeLog(string s);
    //Debug
    vector<string> getValues();

    string getPinCode();
    void clearPinCode();
    string getIgnitionState(void);
    string getWindowState(void);
    string getTrunkState(void);
    string getCentralLockState(void);
    string getDate(void);
    uint8_t getDateCommand(void);
    void setDateCommand(string c);
    void setDate(string d);

    BLEServer* getServer();
    BLECharacteristic* getMainCharacteristic();
    void setDefault();
    void notifyAll();

    //Method that are used for the main actions of the Smart car
    void triggerCentralLocking(void);
    void windowsUp(void);
    void windowsDown(void);
    void triggerTrunk(void);
    void ignitionOn(void);
    void ignitionOff(void);
    void engineCrankOn(void);
    void engineCrankOff(void);
    void startEngine(void);
    //

};


#endif