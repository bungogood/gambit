#pragma once

#include <stdint.h>

class HC595 {
   public:
    const int clockPin;
    const int dataPin;
    const int latchPin;

    HC595(int clockPin, int dataPin, int latchPin);
    void init();
    void write(uint8_t data);
};

class HC165 {
   public:
    const int clockPin;
    const int dataPin;
    const int latchPin;

    HC165(int clockPin, int dataPin, int latchPin);
    void init();
    uint8_t read();
};

class ReedMatrix {
   public:
    HC595* shiftOut;
    HC165* shiftIn;

    ReedMatrix(int outClockPin, int outDataPin, int outLatchPin, int inClockPin,
               int inDataPin, int inLatchPin);
    void init();

    uint64_t read();
};
