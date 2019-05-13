#include "ServerCallbacks.hpp"

ServerCallbacks::ServerCallbacks(BLE *c)
{
    this->c = c;
}
void ServerCallbacks::onConnect(BLEServer *pServer)
{
    c->isConnected = true;
    Serial.println(c->isConnected);
}
void ServerCallbacks::onDisconnect(BLEServer *pServer)
{
    c->isConnected = false;
    c->isPasswordCorrect = false;

    //Clear the current PIN_CODE
    c->clearPinCode();
    //
    Serial.println(c->isConnected);
}