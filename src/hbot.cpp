#include <Arduino.h>
#include "motor.hpp"
#include "hbot.hpp"

HBot::HBot(Motor leftMotor, Motor rightMotor, int switchPin)
    : leftMotor(leftMotor), rightMotor(rightMotor), switchPin(switchPin)
{
}

/**
 * @brief Calibrates the motors to the switch
 * 
 * @param speed movement speed
 */
void HBot::calibrate(int speed) {
    leftMotor.disable();
    rightMotor.enable();
    rightMotor.setDirection(CW);
    while (digitalRead(switchPin) == LOW) {
        rightMotor.step(speed);
    }
    leftMotor.enable();
    leftMotor.reset();
    rightMotor.reset();
}

/**
 * @brief Moves HBot to a position (mm)
 * 
 * @param position destination position
 * @param speed movement speed
 */
void HBot::gotoPosition(Position position, int speed) {
    // Convert position to diagonal vector
    Vector diagonal = toDiagonal(position);

    // Calculate the delta
    move({ diagonal.x - leftMotor.getPosition(), diagonal.y - rightMotor.getPosition() }, speed);
}

/*
void HBot::move(Vector Vector, int speed) {
    leftMotor.setDirection(Vector.x > 0);
    rightMotor.setDirection(Vector.y > 0);


    // int leftSteps  = vector.x;
    // int rightSteps = vector.x;
    leftMotor.enable();
    rightMotor.enable();
    for (int i = 0; i < abs(Vector.x); i++) {
        leftMotor.step(speed);
    }
    for (int i = 0; i < abs(Vector.y); i++) {
        rightMotor.step(speed);
    }
    leftMotor.disable();
    rightMotor.disable();
}
*/


void HBot::move(Vector vector, int speed) {
    // enable motors
    leftMotor.enable();
    rightMotor.enable();

    // set direction of motors
    leftMotor.setDirection(vector.x > 0);
    rightMotor.setDirection(vector.y > 0);

    // Normalize vector
    vector.x = abs(vector.x);
    vector.y = abs(vector.y);

    // find larger and smaller motors to move
    Motor *larger  = vector.x > vector.y ? &leftMotor : &rightMotor;
    Motor *smaller = vector.x > vector.y ? &rightMotor : &leftMotor;

    int largerSteps  = vector.x > vector.y ? vector.x : vector.y;
    int smallerSteps = vector.x > vector.y ? vector.y : vector.x;

    // Calculate ratio
    float ratio = (float)largerSteps / (float)smallerSteps;
    float current = 0;

    while (largerSteps > 0) {
        current++;
        larger->stepInit();
        if (current >= ratio) smaller->stepInit();
        delayMicroseconds(speed);
        larger->stepEnd();
        if (current >= ratio) {
            smaller->stepEnd();
            current -= ratio;
            smallerSteps--;
        }
        largerSteps--;
        delayMicroseconds(speed);
    }

    while (smallerSteps > 0) {
        smaller->step(speed);
        smallerSteps--;
    }

    // enable motors
    leftMotor.disable();
    rightMotor.disable();
}

/**
 * @brief Converts a position (mm) to a diagonal vector (steps)
 * 
 * @param position 
 * @return Vector 
 */
Vector HBot::toDiagonal(Position position) {
    return { (position.x + position.y) * STEPS_PER_MM, (position.x - position.y) * STEPS_PER_MM };
}
