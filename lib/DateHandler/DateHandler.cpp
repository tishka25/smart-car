#include "DateHandler.hpp"

DateHandler::DateHandler(BLE *c)
{
    this->c = c;
}

void DateHandler::setTime()
{
    BLECharacteristic *pCharacteristic = c->getCharacteristic();
    string bleTime = pCharacteristic->getValue().substr(4,10);
    // if (bleTime != "0")
    // {
        // struct tm tm;
        // tm.tm_year = atoi(bleTime.substr(9, 2).c_str()) - 1900;
        // tm.tm_mon = atoi(bleTime.substr(11, 2).c_str());
        // tm.tm_mday = atoi(bleTime.substr(13, 2).c_str());
        // tm.tm_hour = atoi(bleTime.substr(15, 2).c_str());
        // tm.tm_min = atoi(bleTime.substr(17, 2).c_str());
        // t = mktime(&tm);
        t = atoi(bleTime.c_str());
        Serial.println(String("Time in seconds is:" + t));
        struct timeval now = {.tv_sec = t};
        settimeofday(&now, NULL);
    // }
}