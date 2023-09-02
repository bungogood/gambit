#pragma once

class Indicator {
    bool red_state = false;
    bool green_state = false;
    bool blue_state = false;

    void update();

   public:
    const int red_pin;
    const int green_pin;
    const int blue_pin;

    Indicator(int red_pin, int green_pin, int blue_pin);
    void init();

    void setRed(bool state);
    void setGreen(bool state);
    void setBlue(bool state);
    void set(bool red, bool green, bool blue);
};
