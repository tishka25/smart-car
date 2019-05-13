#ifndef CHARACTERISTIC_CALLBACK_HPP
#define CHARACTERISTIC_CALLBACK_HPP


#include <BLE.hpp>
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include <sys/time.h>
#include <ServerCallbacks.hpp>


#include <FreeRTOS.h>
#include <string>
#include <ctime>


class BLE;

using namespace std;

class CharacteristicCallback : public BLECharacteristicCallbacks{
    private:
    //BLE car reference
    BLE *c;
    std::time_t t = std::time(nullptr);


  public:
    CharacteristicCallback(BLE *c)
    {
        this->c = c;
    }
    void onRead(BLECharacteristic *pCharacteristic);
    void onWrite(BLECharacteristic *pCharacteristic);
    
    void gpioStateHandler();
    void passwordHandler();
    void dateHandler();
    void eventHandler(void);

    static void secondThread(void *p);

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
    std::string converter(uint8_t *str){
        return std::string((char *)str);
    }
};



#endif