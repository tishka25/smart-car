#include "CharacteristicCallback.hpp"

using namespace std;

void CharacteristicCallback::gpioStateHandler(){
    //Window left up
    if(c->getWindowState().data()[0] == c->WINDOW_UP){
        digitalWrite(WINDOW_UP_PIN , HIGH);
    }else{
        digitalWrite(WINDOW_UP_PIN , LOW);
    } 
    //Window left down
    if(c->getWindowState().data()[0] == c->WINDOW_DOWN){
        digitalWrite(WINDOW_DOWN_PIN , HIGH);
    }else{
        digitalWrite(WINDOW_DOWN_PIN , LOW);
    }
    // //Window right up
    // if(c->getWindowRightState().data()[0] == c->WINDOW_RIGHT_UP){
    //     digitalWrite(WINDOW_RIGHT_UP_PIN , HIGH);
    // }else{
    //     digitalWrite(WINDOW_RIGHT_UP_PIN , LOW);
    // }
    // //Window right down
    // if(c->getWindowRightState().data()[0] == c->WINDOW_RIGHT_DOWN){
    //     digitalWrite(WINDOW_RIGHT_DOWN_PIN , HIGH);
    // }else{
    //     digitalWrite(WINDOW_RIGHT_DOWN_PIN , LOW);
    // }
    //Central locking
    // if(c->getCentralLockState().data()[0] == c->LOCK){
    //     digitalWrite(CENTRAL_LOCKING_LOCK_PIN , HIGH);
    // }else{
    //     digitalWrite(CENTRAL_LOCKING_LOCK_PIN , LOW);
    // }
    if(c->getCentralLockState().data()[0] == c->UNLOCK){
        digitalWrite(CENTRAL_LOCKING_TRIGG_PIN , HIGH);
    }else{
        digitalWrite(CENTRAL_LOCKING_TRIGG_PIN , LOW);
    }
    //Ignition
    if(c->getIgnitionState().data()[0] == c->IGNITION_ON){
        digitalWrite(IGNITION_PIN , HIGH);
    }else if(c->getIgnitionState().data()[0] == c->IGNITION_OFF){
        digitalWrite(IGNITION_PIN , LOW);
    }
    //Starter
    if(c->getIgnitionState().data()[0] == c->IGNITION_STARTER_ON){
        digitalWrite(STARTER_MOTOR_PIN , HIGH);
    }else if(c->getIgnitionState().data()[0] == c->IGNITION_STARTER_OFF){
        digitalWrite(STARTER_MOTOR_PIN , LOW);
    }
}

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
    if(pCharacteristic->getUUID().toString() == PIN_CODE_CHARACRERISTIC_UUID){
        CharacteristicCallback::passwordHandler();
        Serial.println("Password is: " + String(pCharacteristic->getValue().data()));
    }


    if(pCharacteristic->getUUID().toString()!= PIN_CODE_CHARACRERISTIC_UUID){
        Serial.println("Writing values: " + String(pCharacteristic->getValue().data()));
        dateHandler();
        CharacteristicCallback::gpioStateHandler();
    }
}
