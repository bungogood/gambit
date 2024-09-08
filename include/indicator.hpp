#pragma once

enum class Color : int {
    OFF = 0,
    RED = 1,
    GREEN = 2,
    YELLOW = 3,
    BLUE = 4,
    PURPLE = 5,
    CYAN = 6,
    WHITE = 7
};

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

    void set(Color state);
};
