#ifndef SECURITY_HPP
#define SECURITY_HPP


#include <BLE.hpp>
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include <sys/time.h>


#include <FreeRTOS.h>
#include <string>
#include <ctime>


class BLE;


class Security: public BLECharacteristicCallbacks
{
private: 
    BLE *c;
    BLEServer *server;
public:
    Security();
    Security(BLEServer *p  , BLE *c);
    
    void block();
    void passwordHandler();

    void onRead(BLECharacteristic *pCharacteristic);
    void onWrite(BLECharacteristic *pCharacteristic);
};



#endif