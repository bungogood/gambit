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

    moveChar = pService->createCharacteristic(
        MOVE_CHAR_UUID, BLECharacteristic::PROPERTY_READ |
                            BLECharacteristic::PROPERTY_WRITE |
                            BLECharacteristic::PROPERTY_NOTIFY);
    moveChar->addDescriptor(new BLE2902());

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

void BluetoothManager::setFSMState(const FSMState state) {
    if (stateChar != nullptr) {
        std::string stateStr = fsm_state_string(state);
        stateChar->setValue(stateStr.c_str());
        stateChar->notify();
    }
}

void BluetoothManager::setMove(const Move move) {
    if (moveChar != nullptr) {
        std::string moveStr = square_to_string(move.source_square) +
                              square_to_string(move.target_square);
        moveChar->notify();
    }
}
