#pragma once

#include "motor.hpp"

#define STEPS_PER_MM 1
#define DEFAULT_SPEED 1000
#define CALIBRATION_SPEED 1000

// Stored in steps
typedef struct Vector {
    int x;
    int y;
} Vector;

// Stored in mm
typedef Vector Position;

class HBot {
    Motor leftMotor;
    Motor rightMotor;

    int switchPin;

    Vector toDiagonal(Position position);
    void move(Vector vector, int speed);
public:
    HBot(Motor leftMotor, Motor rightMotor, int switchPin);

    void calibrate(int speed = CALIBRATION_SPEED);

    void gotoPosition(Position position, int speed = DEFAULT_SPEED);
};
