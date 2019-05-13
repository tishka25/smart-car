#ifndef SERVER_CALLBACK_HPP
#define SERVER_CALLBACK_HPP

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

using namespace std;

class ServerCallbacks : public BLEServerCallbacks
{
  public:
    BLE *c;
    ServerCallbacks(BLE *c);
    void onConnect(BLEServer *pServer);
    void onDisconnect(BLEServer *pServer);
};
#endif