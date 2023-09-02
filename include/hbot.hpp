#pragma once

#include "motor.hpp"

#define STEPS_PER_MM 6.3
#define DEFAULT_SPEED 500
#define CALIBRATION_SPEED 500

#define X_LIMIT 400
#define Y_LIMIT 500

// Stored in steps
typedef struct Coord {
    int x;
    int y;
} Coord;

// Stored in mm
typedef Coord Position;

class HBot {
    Motor* leftMotor;
    Motor* rightMotor;

    int switchPin;

   public:
    HBot(Motor* leftMotor, Motor* rightMotor, int switchPin);
    void init();

    Coord toDiagonal(Position position);
    void move(Coord vector, int speed);

    void calibrate(int speed = CALIBRATION_SPEED);
    void gotoPosition(Position position, int speed = DEFAULT_SPEED);
};
