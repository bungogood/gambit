#include <Arduino.h>

#include "bluetooth.hpp"
#include "indicator.hpp"

Indicator indicator(LED_RED, LED_GREEN, LED_BLUE);

//   a  b  c  d  e  f  g   h
// 8 .  .  .  .  .  .  .   .  8 D8

// 7 .  .  .  .  .  .  .   .  7 D7

// 6 .  .  .  .  .  .  .   .  6 D6

// 5 .  .  .  .  .  .  .   .  5 D5

// 4 .  .  .  .  .  .  .   .  4 D4

// 3 .  .  .  .  .  .  .   .  3 D3

// 2 .  .  .  .  .  .  .   .  2 D2

// 1 .  .  .  .  .  .  .   .  1 D1 (TX1)
//  A0  A1 A2 A3 A4 A5 D10 D11

const int A_H_PINS[8] = {A0, A1, A2, A3, A4, A5, 10, 11};
const int ONE_EIGHT_PINS[8] = {D1, D2, D3, D4, D5, D6, D7, D8};
const bool VERBOSE = false;
int boardState[8][8];
BluetoothManager ble;

void setup() {
    Serial.begin(9600);

    ble.begin("Gambit");

    indicator.init();
    indicator.set(Color::BLUE);

    for (int i = 0; i < 8; i++) {
        pinMode(A_H_PINS[i], OUTPUT);
        pinMode(ONE_EIGHT_PINS[i], INPUT_PULLDOWN);
    }
}

void loop() {
    for (int col = 0; col < 8; col++) {
        digitalWrite(A_H_PINS[col], HIGH);
        for (int row = 0; row < 8; row++) {
            boardState[row][col] = digitalRead(ONE_EIGHT_PINS[row]);
        }
        digitalWrite(A_H_PINS[col], LOW);
    }

    // print the board state from 8 to 1
    if (VERBOSE) {
        Serial.println("Board State:");
        Serial.println("  a b c d e f g h");
        for (int row = 7; row >= 0; row--) {
            Serial.print(row);
            for (int col = 0; col < 8; col++) {
                Serial.print(" ");
                Serial.print(boardState[row][col]);
            }
            Serial.println();
        }
        Serial.println("  a b c d e f g h");
    }

    delay(50);

    // Convert board state to u64
    uint64_t boardStateU64 = 0;
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            boardStateU64 |= (uint64_t)boardState[row][col] << (row * 8 + col);
        }
    }
    ble.setReedSwitchValue(boardStateU64);
}