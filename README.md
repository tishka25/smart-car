# smart-car


## Modifying the ESP32 Arduino library
### I have added this code snippet in BLEServer.cpp in order to have a disconnecting functionalities
   ```c++

  /**
   * @brief Disconnect Client
   *
   * Force a client to disconnect
   */
  void BLEServer::disconnectClient() {
    ESP_LOGD(LOG_TAG, ">> disconnectClient()");
    esp_err_t errRc = ::esp_ble_gatts_close(getGattsIf(), getConnId());
    if (errRc != ESP_OK) {
      ESP_LOGD(LOG_TAG, "esp_ble_gatts_close: rc=%d %s", errRc, GeneralUtils::errorToString(errRc));
      return;
    }
    ESP_LOGD(LOG_TAG, "<< disconnectClient()");
  } // disconnectClient
  ```
