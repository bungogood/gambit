#pragma once

#include "hbot.hpp"

class Magnet {
    bool enabled = false;

   public:
    const int pin;
    const Coord offset;

    Magnet(int pin, Coord offset);

    void enable();
    void disable();

    bool isOn() { return enabled; }
};
