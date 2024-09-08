#include "indicator.hpp"

#include <Arduino.h>

Indicator::Indicator(int red_pin, int green_pin, int blue_pin)
    : red_pin(red_pin), green_pin(green_pin), blue_pin(blue_pin) {
    pinMode(red_pin, OUTPUT);
    pinMode(green_pin, OUTPUT);
    pinMode(blue_pin, OUTPUT);
    update();
}

void Indicator::update() {
    digitalWrite(red_pin, !red_state);
    digitalWrite(green_pin, !green_state);
    digitalWrite(blue_pin, !blue_state);
}

void Indicator::set(Color state) {
    int value = static_cast<int>(state);
    red_state = value & 1;
    green_state = value & 2;
    blue_state = value & 4;
    update();
}
