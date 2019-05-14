#include "CharacteristicCallbacks.hpp"

using namespace std;

void CharacteristicCallbacks::gpioStateHandler(){
    //Window up
    if(c->getWindowState().data()[0] == c->WINDOW_UP){
        digitalWrite(WINDOW_UP_PIN , HIGH);
    }else{
        digitalWrite(WINDOW_UP_PIN , LOW);
    } 
    //Window down
    if(c->getWindowState().data()[0] == c->WINDOW_DOWN){
        digitalWrite(WINDOW_DOWN_PIN , HIGH);
    }else{
        digitalWrite(WINDOW_DOWN_PIN , LOW);
    }
    //Central locking
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
        // digitalWrite(IGNITION_PIN , HIGH);
    }else if(c->getIgnitionState().data()[0] == c->IGNITION_STARTER_OFF){
        digitalWrite(STARTER_MOTOR_PIN , LOW);
    }
    //Trunk
    if(c->getTrunkState().data()[0] == c->OPEN_TRUNK){
        digitalWrite(OPEN_TRUNK_PIN , HIGH);
    }else if(c->getTrunkState().data()[0] == c->STANDARD){
        digitalWrite(OPEN_TRUNK_PIN , LOW);
    }
}

void CharacteristicCallbacks::passwordHandler(){
    string pass = c->getPinCode();
    BLEServer *pServer = c->getServer();

    if(pass != c->pin.PIN_CODE){
        c->setDefault();
        c->pin.failedEntries++;
        Serial.print("Failed entries: ");
        Serial.println(c->pin.failedEntries);
        c->isPasswordCorrect = false;
        pServer->disconnectClient();
    }else if (pass == c->pin.PIN_CODE){
        Serial.println("Password Correct!");
        c->isPasswordCorrect = true;
        //Clear the failed attempts
        c->pin.failedEntries = 0;
        //
    }

    if (c->pin.failedEntries >= c->pin.MAX_FAILED_ENTRIES)
        pServer->disconnectClient();
}


void CharacteristicCallbacks::dateHandler(){
    uint8_t param = c->getDateCommand();
    string _time = c->getDate();
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

void CharacteristicCallbacks::eventHandler(){
    uint8_t param = c->getDateCommand();
    if(param == CENTRAL_LOCK){
        Serial.println("ARE KYREC");
        TaskHandle_t xHandle = NULL;
        xTaskCreate(CharacteristicCallbacks::secondThread , "ASYNC_TASK" , 1024 , NULL , tskIDLE_PRIORITY , &xHandle);
    }
    
}

void CharacteristicCallbacks::secondThread(void *p){
    while(true){
        Serial.println("AZ SUM KUREC");
    }
}


void CharacteristicCallbacks::onRead(BLECharacteristic *pCharacteristic){
    Serial.println("Reading value");
}
void CharacteristicCallbacks::onWrite(BLECharacteristic *pCharacteristic){
    CharacteristicCallbacks::passwordHandler();
    Serial.println("Writing values: " + String(pCharacteristic->getValue().data()));
    dateHandler();
    CharacteristicCallbacks::gpioStateHandler();
    CharacteristicCallbacks::eventHandler();

    if(pCharacteristic->getUUID().toString() == PIN_CODE_CHARACRERISTIC_UUID){
        Serial.println("Password is: " + String(pCharacteristic->getValue().data()));
    }
}
