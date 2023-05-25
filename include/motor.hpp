#pragma once

#define CW true
#define CCW false
#define DEFAULT_INTERVAL 1000

class Motor {
    bool enabled;
    bool direction;
    int position;

public:
    const int enablePin;
    const int dirPin;
    const int stepPin;
    const int stepsPerRevolution;

    Motor(int enablePin, int dirPin, int stepPin, int stepsPerRevolution);

    void reset() {
        position = 0;
        disable();
        setDirection(CW);
    }

    void enable() { 
        digitalWrite(enablePin, LOW);
        enabled = true;
    }
    void disable() { 
        digitalWrite(enablePin, HIGH);
        enabled = false;
    }

    void step(int interval = DEFAULT_INTERVAL);
    void steps(int steps, int interval = DEFAULT_INTERVAL);
    void stepInit();
    void stepEnd();

    void setDirection(bool dir);
    bool isEnabled() { return enabled; };
    bool getDirection() { return direction; };
    int getPosition() { return position; };
};
