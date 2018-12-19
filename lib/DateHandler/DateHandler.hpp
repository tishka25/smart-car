#ifndef DATE_HANDLER_HPP
#define DATE_HANDLER_HPP

#include <BLE.hpp>
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include <sys/time.h>
#include <Security.hpp>

#include <FreeRTOS.h>
#include <string>
#include <ctime>

using namespace std;

class DateHandler{
private:
    BLE *c;
    std::time_t t = std::time(nullptr);

public:
    DateHandler();
    DateHandler(BLE *c);


    void setTime();
};

#endif