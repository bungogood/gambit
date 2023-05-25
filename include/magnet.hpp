#pragma once

#include "hbot.hpp"

class Magnet {
    bool enabled = false;
public:
    const int pin;
    const Vector offset;

    Magnet(int pin, Vector offset);

    void enable();
    void disable();
};
