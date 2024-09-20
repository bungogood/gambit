#include <Arduino.h>

#include "bluetooth.hpp"
#include "indicator.hpp"

Indicator indicator(LED_RED, LED_GREEN, LED_BLUE);
BluetoothManager ble;

const int loadPin = A0;
const int clockPin = A1;
const int dataPin = A2;
const int clockEnablePin = A3;

// Data pins to shift in
const int dataPins[8] = {D1, D2, D3, D4, D5, D6, D7, D8};
const bool USE_SHIFT_OUT = true;

// Shift out pins
const int shiftOutDataPin = D1;
const int shiftOutClockPin = D2;
const int shiftOutLatchPin = D3;

byte A_H_ACTIVATION_NUMS[8];
byte shiftOutData;

// Delay time for the shift register
const int delayInMicroseconds = 30;
const int clockPulseDelay = 5;  // 0.002;

byte shiftIn(int dataPin, int clockPin, int bitOrder) {
    byte value = 0;
    for (int i = 0; i < 8; i++) {
        byte bitValue = digitalRead(dataPin);

        if (bitOrder == LSBFIRST) {
            value |= (bitValue << i);
        } else {
            value |= (bitValue << (7 - i));
        }

        digitalWrite(clockPin, HIGH);
        delayMicroseconds(clockPulseDelay);
        digitalWrite(clockPin, LOW);
        delayMicroseconds(clockPulseDelay);
    }
    return value;
}

void shiftOut(int dataPin, int clockPin, byte dataOut) {
    for (int i = 7; i >= 0; i--) {
        digitalWrite(clockPin, LOW);
        delayMicroseconds(clockPulseDelay);
        if (dataOut & (1 << i)) {
            digitalWrite(dataPin, HIGH);
        } else {
            digitalWrite(dataPin, LOW);
        }
        digitalWrite(clockPin, HIGH);
        delayMicroseconds(clockPulseDelay);
        digitalWrite(dataPin, LOW);
    }
}

void writeOut(byte data) {
    digitalWrite(shiftOutLatchPin, LOW);
    delayMicroseconds(delayInMicroseconds);
    shiftOut(shiftOutDataPin, shiftOutClockPin, data);
    digitalWrite(shiftOutLatchPin, HIGH);
    delayMicroseconds(delayInMicroseconds);
}

byte readIn() {
    digitalWrite(loadPin, LOW);
    delayMicroseconds(delayInMicroseconds);
    digitalWrite(loadPin, HIGH);
    delayMicroseconds(delayInMicroseconds);

    // Shift in the data
    return shiftIn(dataPin, clockPin, MSBFIRST);
}

void setup() {
    Serial.begin(9600);
    indicator.init();
    indicator.set(Color::BLUE);
    ble.begin("Gambit");

    // Initing shift register pins
    pinMode(loadPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    pinMode(dataPin, INPUT);
    pinMode(clockEnablePin, OUTPUT);

    // Initing data pins
    if (USE_SHIFT_OUT) {
        pinMode(shiftOutDataPin, OUTPUT);
        pinMode(shiftOutClockPin, OUTPUT);
        pinMode(shiftOutLatchPin, OUTPUT);

        for (int i = 0; i < 8; i++) {
            A_H_ACTIVATION_NUMS[i] = 0xFF;
        }
        // 127 in hex
        A_H_ACTIVATION_NUMS[7] = B10000000;
    } else {
        for (int i = 0; i < 8; i++) {
            pinMode(dataPins[i], OUTPUT);
        }
    }

    // Setting clock enable to low
    digitalWrite(clockEnablePin, LOW);
}

void printInputData(byte data, int pinSetHigh) {
    // For each of the data pins print the value
    // Print the labels of each pins
    Serial.println("--------------------");
    Serial.println("Input Data with D" + String(pinSetHigh) + " set high");
    for (int i = 0; i < 8; i++) {
        Serial.print("D");
        Serial.print(i);
        Serial.print(" ");
    }
    Serial.println();
    for (int i = 0; i < 8; i++) {
        Serial.print((data & (1 << i)) >> i);
        Serial.print("  ");
    }
    Serial.println();
    Serial.println();
    Serial.println("--------------------");
}

void printBoardState(uint64_t boardState) {
    Serial.println("Board State:");
    Serial.println("  a b c d e f g h");
    for (int row = 7; row >= 0; row--) {
        Serial.print(row);
        for (int col = 0; col < 8; col++) {
            Serial.print(" ");
            bool value = (boardState >> (row * 8 + col)) & 1;
            Serial.print(value);
        }
        Serial.println();
    }
    Serial.println("  a b c d e f g h");
}

uint64_t readBoard() {
    // row by row
    uint64_t board;
    for (int row = 7; row >= 0; row--) {
        writeOut(1 << row);
        byte data = readIn();
        board = board << 8 | data;
    }
    return board;
}

uint64_t prev = 0;
long count = 0;
long fails = 0;

uint64_t consensus(uint64_t A, uint64_t B, uint64_t C) {
    // If A equals B, return A
    if (A == B) return A;

    // If B equals C, return B
    if (B == C) return B;

    // If A equals C, return A
    if (A == C) return A;

    // If none are equal, you could return A or handle the error as needed
    return A;  // or return B or C depending on your logic
}

void loop() {
    uint64_t boardA = readBoard();
    delay(1);
    uint64_t boardB = readBoard();
    delay(1);
    uint64_t boardC = readBoard();

    uint64_t board = consensus(boardA, boardB, boardC);

    // uint64_t board = readBoard();

    // printBoardState(board);
    ble.setReedSwitchValue(board);
    // delay(200);

    count++;
    if (board != prev) {
        fails++;
    }
    prev = board;

    if (count % 10000 == 0) {
        float prec = 100.0 * (float)fails / (float)count;
        Serial.println(prec);
        Serial.println(fails);
        Serial.println(count);
        fails = 0;
        count = 0;
    }

    if (count % 1000 == 0) {
        float prec = 100.0 * (float)fails / (float)count;
        Serial.println(prec);
    }
}
