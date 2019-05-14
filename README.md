# smart-car


## Modifying the ESP32 Arduino library
### I have added this code snippet in BLEServer.cpp in order to have disconnecting functionalities
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
  ## Commands and UUID's for BLE characteristics
  ```javascript
  var Characteristic = {
  UUID: "3ff8860e-72ca-4a25-9c4e-99c7d3b08e9b",
  DEFAULT: 0x50,
  windows: {
    UP: 0x51,
    DOWN: 0x52,
  },
  ignition: {
    ON: 0x60,
    OFF: 0x50,
    STARTER_ON: 0x62,
    STARTER_OFF: 0x63,
  },
  centralLocking: {
    UNLOCK: 0x75,
  },
  trunk: {
    OPEN: 0x77,
  },
  DATE: {
    DEFAULT: "0",
    SET_DATE_TIME: "1",
    CENTRAL_LOCK: "3",
  },
  password: {
    UUID: "def231dc-07d4-4a71-b735-811e07d44c07",
  },
};
  ```
  # This project has been made with PlatformIO IDE
