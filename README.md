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
  ## Commands and UUID's for BLE characteristics
  ```javascript
  var CHARACTERISTICS = {
  WINDOWS: {
    LEFT: {
      UUID: "3ff8860e-72ca-4a25-9c4e-99c7d3b08e9b",
      UP: 0x51,
      DOWN: 0x52
    },
    RIGHT: {
      UUID: "96cc6576-b9b0-443b-b8e6-546bbd20d374",
      UP: 0x54,
      DOWN: 0x55
    }
  },
  IGNITION: {
    UUID: "4ad1bbf1-b3e6-4239-a3bb-520624ee1329",
    ON: 0x60,
    START: 0x62
  },
  PIN_CODE: {
    UUID: "def231dc-07d4-4a71-b735-811e07d44c07",
  },
  DEFAULT: 0x50
};
  ```
