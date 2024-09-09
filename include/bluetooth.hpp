#pragma once

#include <BLE2902.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

#include <string>

#include "fsm.hpp"

#define SERVICE_UUID "f88918be-312c-4a9b-a7a2-97db83b2e3a9"
#define FEN_CHAR_UUID "82ca99da-f6c3-4eb7-ac2d-ea12cac9af5c"
#define REED_SWITCH_CHAR_UUID "3f7ebdc2-6d63-426f-a2ad-8d10c450743f"
#define STATE_CHAR_UUID "1a100eac-62c1-4c73-955c-55c97f4c57be"
#define MOVE_CHAR_UUID "27dc4447-33d4-4e8a-88fa-c683d4545d47"
#define CMD_CHAR_UUID "b375a4f9-2c61-47ab-a14e-3dbacdb9c25d"

class BluetoothManager {
   public:
    BluetoothManager();
    void begin(const char* deviceName);
    void setFEN(const std::string& fen);
    void setReedSwitchValue(uint64_t value);
    void setFSMState(const FSMState state);
    void setMove(const Move move);

   private:
    BLEServer* pServer = nullptr;
    BLEService* pService = nullptr;
    BLECharacteristic* fenChar = nullptr;
    BLECharacteristic* reedSwitchChar = nullptr;
    BLECharacteristic* stateChar = nullptr;
    BLECharacteristic* moveChar = nullptr;
    // bool deviceConnected = false;
};
