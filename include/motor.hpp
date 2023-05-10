#pragma once

#define CW true
#define CCW false
#define DEFAULT_INTERVAL 1000

class Motor {
    bool direction;
    int position;

public:
    const int dirPin;
    const int stepPin;
    const int stepsPerRevolution;

    Motor(int dirPin, int stepPin, int stepsPerRevolution);

    void reset() { position = 0; }
    void step(int interval = DEFAULT_INTERVAL);
    void steps(int steps, int interval = DEFAULT_INTERVAL);
    void stepInit();
    void stepEnd();

    void setDirection(bool dir);
    int getPosition();
};
