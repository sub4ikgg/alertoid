#include "ble.h"
#include <Arduino.h>
#include "wifi/wifi.h"

static BLECharacteristic *pTxChar;

class ServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer *s) {
        Serial.println("[BLE] Device connected");
    }
    void onDisconnect(BLEServer *s) {
        Serial.println("[BLE] Device disconnected");
        s->startAdvertising();
    }
};

class WifiConfCallbacks : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pChar) {
        // conf format: "ssid;password"
        String msg = pChar->getValue().c_str();
        Serial.println("[BLE] Received msg as WifiConf: " + msg);

        if (!msg.isEmpty()) {
            int separatorIndex = msg.indexOf(';');

            String ssid = msg.substring(0, separatorIndex);;
            String password = msg.substring(separatorIndex + 1);;

            stopConnectingToWifi();
            writeWifiConf(ssid, password);
            connectToWifi();
        }
    }
};

void initBle() {
    BLEDevice::init(getDeviceName().c_str());

    BLEServer *pServer = BLEDevice::createServer();
    pServer->setCallbacks(new ServerCallbacks());

    BLEService *pService = pServer->createService(BLEUUID(BLE_SERVICE_UUID), 20);

    pTxChar = pService->createCharacteristic(
        BLE_TX_UUID,
        BLECharacteristic::PROPERTY_NOTIFY
    );
    pTxChar->addDescriptor(new BLE2902());

    BLECharacteristic *pCmdChar = pService->createCharacteristic(
        BLE_WIFI_CONF_UUID,
        BLECharacteristic::PROPERTY_WRITE
    );
    pCmdChar->setCallbacks(new WifiConfCallbacks());

    pService->start();
    pServer->getAdvertising()->start();
    Serial.println("[BLE] Advertising started as " + getDeviceName());
}

String getDeviceName() {
    uint64_t chipId = ESP.getEfuseMac();
    String id = "";
    for (int i = 5; i >= 0; i--) {
        if ((chipId >> (i * 8) & 0xFF) < 16) id += "0";
        id += String((chipId >> (i * 8) & 0xFF), HEX);
    }
    id.toUpperCase();
    return "Alertoid-" + id;
}