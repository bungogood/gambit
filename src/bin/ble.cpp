#include <Arduino.h>

#include <string>

#include "bluetooth.hpp"

// Global variable to store FEN string
std::string startFen =
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
std::string otherFen =
    "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2";

BluetoothManager* ble;
uint64_t reedSwitchValue;

void setup() {
    Serial.begin(9600);
    ble = new BluetoothManager();
    ble->begin("Gambit");

    reedSwitchValue = 0;
}

void loop() {
    ble->setReedSwitchValue(reedSwitchValue);
    reedSwitchValue++;
    ble->setFEN(startFen);
    delay(1000);
    ble->setFEN(otherFen);
    delay(1000);
}
