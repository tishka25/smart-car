#include "Security.hpp"

Security::Security(BLEServer *s , BLE *c){
        this->server = s;
        this->c = c;
}


void Security::block(){
    server->disconnectClient();
}

void Security::passwordHandler(){
    string pass = c->getPinCode();
    if(pass != c->pin.PIN_CODE){
        c->setDefault();
        c->pin.failedEntries++;
        Serial.print("Failed entries: ");
        Serial.println(c->pin.failedEntries);
        block();
    }else if (pass == c->pin.PIN_CODE){
        Serial.println("Password Correct!");
        //Clear the failed attempts
        c->pin.failedEntries = 0;
        //
    }

    if (c->pin.failedEntries >= c->pin.MAX_FAILED_ENTRIES)
        block();
}


void Security::onRead(BLECharacteristic *pCharacteristic){

}
void Security::onWrite(BLECharacteristic *pCharacteristic){
    // if(pCharacteristic->getUUID().toString() == PIN_CODE_CHARACRERISTIC_UUID){
        Security::passwordHandler();
    // }
}