#include "CharacteristicCallback.hpp"

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
    BLECharacteristic *pCharacteristic = c->getMainCharacteristic();
    string bleTime = pCharacteristic->getValue().substr(4,10);

    //TODO
    t = atoi(bleTime.c_str());
    Serial.println(String("Time in seconds is:" + t));
    struct timeval now = {.tv_sec = t};
    settimeofday(&now, NULL);
}


void CharacteristicCallback::onRead(BLECharacteristic *pCharacteristic){

}
void CharacteristicCallback::onWrite(BLECharacteristic *pCharacteristic){
    // if(pCharacteristic->getUUID().toString() == PIN_CODE_CHARACRERISTIC_UUID){
        CharacteristicCallback::passwordHandler();
    // }
    string buff = pCharacteristic->getValue();
    // if(pCharacteristic->getUUID().toString()!= PIN_CODE_CHARACRERISTIC_UUID){
    //     if(buff)
    // }
}