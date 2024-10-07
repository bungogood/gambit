#include "reed.hpp"

#include <Arduino.h>

const int clockPulseDelay = 5;
const int delayInMicroseconds = 30;

ReedMatrix::ReedMatrix(int outClockPin, int outDataPin, int outLatchPin,
                       int inClockPin, int inDataPin, int inLatchPin) {
    shiftOut = new HC595(outClockPin, outDataPin, outLatchPin);
    shiftIn = new HC165(inClockPin, inDataPin, inLatchPin);
}

void ReedMatrix::init() {
    shiftOut->init();
    shiftIn->init();
}

uint64_t ReedMatrix::read() {
    uint64_t board;
    for (int row = 7; row >= 0; row--) {
        shiftOut->write(1 << row);
        delayMicroseconds(delayInMicroseconds);
        byte data = shiftIn->read();
        delayMicroseconds(delayInMicroseconds);
        board = board << 8 | data;
    }
    return board;
}

HC595::HC595(int clockPin, int dataPin, int latchPin)
    : clockPin(clockPin), dataPin(dataPin), latchPin(latchPin) {}

void HC595::init() {
    pinMode(clockPin, OUTPUT);
    pinMode(dataPin, OUTPUT);
    pinMode(latchPin, OUTPUT);
}

void HC595::write(uint8_t data) {
    digitalWrite(latchPin, LOW);
    delayMicroseconds(delayInMicroseconds);
    for (int i = 7; i >= 0; i--) {
        digitalWrite(clockPin, LOW);
        delayMicroseconds(clockPulseDelay);
        if ((data >> i) & 1) {
            digitalWrite(dataPin, HIGH);
        } else {
            digitalWrite(dataPin, LOW);
        }
        digitalWrite(clockPin, HIGH);
        delayMicroseconds(clockPulseDelay);
    }
    digitalWrite(latchPin, HIGH);
    delayMicroseconds(delayInMicroseconds);
}

HC165::HC165(int clockPin, int dataPin, int latchPin)
    : clockPin(clockPin), dataPin(dataPin), latchPin(latchPin) {}

void HC165::init() {
    pinMode(clockPin, OUTPUT);
    pinMode(dataPin, INPUT);
    pinMode(latchPin, OUTPUT);
}

uint8_t HC165::read() {
    digitalWrite(latchPin, LOW);
    delayMicroseconds(delayInMicroseconds);
    digitalWrite(latchPin, HIGH);
    delayMicroseconds(delayInMicroseconds);

    uint8_t value = 0;
    for (int i = 0; i < 8; i++) {
        byte bitValue = digitalRead(dataPin);
        delayMicroseconds(clockPulseDelay);

        value |= (bitValue << (7 - i));

        digitalWrite(clockPin, HIGH);
        delayMicroseconds(clockPulseDelay);
        digitalWrite(clockPin, LOW);
        delayMicroseconds(clockPulseDelay);
    }

    return value;
}
