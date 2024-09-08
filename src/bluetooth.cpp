#include "bluetooth.hpp"

class ServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
        // deviceConnected = true;
        // Serial.println("Device connected");
    }

    void onDisconnect(BLEServer* pServer) {
        // deviceConnected = false;
        // Serial.println("Device disconnected");
        pServer->startAdvertising();  // Restart advertising
    }
};

BluetoothManager::BluetoothManager() {}

void BluetoothManager::begin(const char* deviceName) {
    BLEDevice::init(deviceName);
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new ServerCallbacks());

    pService = pServer->createService(SERVICE_UUID);

    fenChar = pService->createCharacteristic(
        FEN_CHAR_UUID,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
    fenChar->addDescriptor(new BLE2902());

    reedSwitchChar = pService->createCharacteristic(
        REED_SWITCH_CHAR_UUID,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
    reedSwitchChar->addDescriptor(new BLE2902());

    stateChar = pService->createCharacteristic(
        STATE_CHAR_UUID,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
    stateChar->addDescriptor(new BLE2902());

    pService->start();

    // BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();
    // pAdvertising->addServiceUUID(SERVICE_UUID);
    // pAdvertising->setScanResponse(false);
    // pAdvertising->setMinPreferred(0x0);
    pServer->startAdvertising();
}

void BluetoothManager::setFEN(const std::string& fen) {
    if (fenChar != nullptr) {
        fenChar->setValue(fen.c_str());
        fenChar->notify();
    }
}

void BluetoothManager::setReedSwitchValue(uint64_t value) {
    if (reedSwitchChar != nullptr) {
        reedSwitchChar->setValue((uint8_t*)&value, sizeof(value));
        reedSwitchChar->notify();
    }
}

void BluetoothManager::setFSMState(const std::string& state) {
    if (stateChar != nullptr) {
        stateChar->setValue(state.c_str());
        stateChar->notify();
    }
}
