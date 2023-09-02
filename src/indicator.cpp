#include "indicator.hpp"

#include <Arduino.h>

Indicator::Indicator(int red_pin, int green_pin, int blue_pin)
    : red_pin(red_pin), green_pin(green_pin), blue_pin(blue_pin) {
    init();
}

void Indicator::init() {
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

void Indicator::setRed(bool state) {
    red_state = state;
    update();
}

void Indicator::setGreen(bool state) {
    green_state = state;
    update();
}

void Indicator::setBlue(bool state) {
    blue_state = state;
    update();
}

void Indicator::set(bool red, bool green, bool blue) {
    red_state = red;
    green_state = green;
    blue_state = blue;
    update();
}
