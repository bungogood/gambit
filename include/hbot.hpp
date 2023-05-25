#pragma once

#include "motor.hpp"

#define STEPS_PER_MM 5.6
#define DEFAULT_SPEED 500
#define CALIBRATION_SPEED 500

#define X_LIMIT 400
#define Y_LIMIT 500

// Stored in steps
typedef struct Vector {
    int x;
    int y;
} Vector;

// Stored in mm
typedef Vector Position;

class HBot {
    Motor* leftMotor;
    Motor* rightMotor;

    int switchPin;
public:
    HBot(Motor* leftMotor, Motor* rightMotor, int switchPin);

    Vector toDiagonal(Position position);
    void move(Vector vector, int speed);

    void calibrate(int speed = CALIBRATION_SPEED);
    void gotoPosition(Position position, int speed = DEFAULT_SPEED);
};
