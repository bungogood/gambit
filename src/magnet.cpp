#include "magnet.hpp"

#include <Arduino.h>

Magnet::Magnet(int pin, Coord offset) : pin(pin), offset(offset) { init(); }

void Magnet::init() {
    pinMode(pin, OUTPUT);
    disable();
}

void Magnet::enable() {
    digitalWrite(pin, HIGH);
    enabled = true;
}

void Magnet::disable() {
    digitalWrite(pin, LOW);
    enabled = false;
}
