#include <Arduino.h>
#include "magnet.hpp"

Magnet::Magnet(int pin, Vector offset)
    : pin(pin), offset(offset)
{
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
