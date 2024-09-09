#include "reed.hpp"

#include <Arduino.h>

Reed::Reed(int outClk, int outData, int outLch, int inClk, int inData,
           int inLch)
    : outClk(outClk),
      outData(outData),
      outLch(outLch),
      inClk(inClk),
      inData(inData),
      inLch(inLch) {
    init();
}

void Reed::init() {
    pinMode(outClk, OUTPUT);
    pinMode(outData, OUTPUT);
    pinMode(outLch, OUTPUT);
    pinMode(inClk, OUTPUT);
    pinMode(inData, INPUT);
    pinMode(inLch, OUTPUT);
}

uint64_t Reed::read() {
    uint64_t data = 0;
    digitalWrite(outLch, HIGH);
    digitalWrite(outClk, LOW);
    digitalWrite(outLch, LOW);
    for (int i = 0; i < 64; i++) {
        digitalWrite(outClk, HIGH);
        digitalWrite(outClk, LOW);
        data |= digitalRead(inData) << i;
    }
    digitalWrite(outLch, HIGH);
    return data;
}
