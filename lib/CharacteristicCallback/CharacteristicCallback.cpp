#include "CharacteristicCallback.hpp"

using namespace std;


void CharacteristicCallback::passwordHandler(){
    string pass = c->getPinCode();
    BLEServer *pServer = c->getServer();


    if(pass != c->pin.PIN_CODE){
        c->setDefault();
        c->pin.failedEntries++;
        Serial.print("Failed entries: ");
        Serial.println(c->pin.failedEntries);
        pServer->disconnectClient();
    }else if (pass == c->pin.PIN_CODE){
        Serial.println("Password Correct!");
        //Clear the failed attempts
        c->pin.failedEntries = 0;
        //
    }

    if (c->pin.failedEntries >= c->pin.MAX_FAILED_ENTRIES)
        pServer->disconnectClient();
}


void CharacteristicCallback::dateHandler(){
    // BLECharacteristic *pCharacteristic = c->getMainCharacteristic();
    // string bleTime = pCharacteristic->getValue().substr(4,10);
    uint8_t param = c->getDateCommand();
    string _time = c->getDate();

    //TODO
    if(param == SET_TIME_ON_RTC){
        t = atoi(_time.c_str());
        struct timeval now = {.tv_sec = t};
        settimeofday(&now, NULL);
        c->setDateCommand("0");

        //Debug
        time_t currTime = time(nullptr);
        Serial.print("Setting time: ");
        Serial.println(ctime(&currTime));
    }
}


void CharacteristicCallback::onRead(BLECharacteristic *pCharacteristic){

}
void CharacteristicCallback::onWrite(BLECharacteristic *pCharacteristic){
    // if(pCharacteristic->getUUID().toString() == PIN_CODE_CHARACRERISTIC_UUID){
    CharacteristicCallback::passwordHandler();
    // }

    if(pCharacteristic->getUUID().toString()!= PIN_CODE_CHARACRERISTIC_UUID){
        dateHandler();
    }
    Serial.println(pCharacteristic->getValue().data());
}