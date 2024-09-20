#include "bluetooth.hpp"

class ServerCallbacks : public BLEServerCallbacks {
    BluetoothManager* pBluetoothManager;

   public:
    ServerCallbacks(BluetoothManager* pBluetoothManager)
        : pBluetoothManager(pBluetoothManager) {}

    void onConnect(BLEServer* pServer) { pBluetoothManager->connected = true; }

    void onDisconnect(BLEServer* pServer) {
        pBluetoothManager->connected = false;
        pServer->startAdvertising();
    }
};

class MoveCallbacks : public BLECharacteristicCallbacks {
    BluetoothManager* pBluetoothManager;

   public:
    MoveCallbacks(BluetoothManager* pBluetoothManager)
        : pBluetoothManager(pBluetoothManager) {}

    void onWrite(BLECharacteristic* pCharacteristic) {
        std::string value = pCharacteristic->getValue();
        pBluetoothManager->writeMove(value);
    }
};

BluetoothManager::BluetoothManager() {}

void BluetoothManager::begin(const char* deviceName) {
    BLEDevice::init(deviceName);
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new ServerCallbacks(this));

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
    moveChar->setCallbacks(new MoveCallbacks(this));

    pService->start();
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

void BluetoothManager::writeMove(const std::string move) {
    moveStr = move;
    moveUpdated = true;
}

std::string BluetoothManager::readMove() {
    moveUpdated = false;
    return moveStr;
}
