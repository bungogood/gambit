#include <Arduino.h>
#include "motor.hpp"
#include "hbot.hpp"

HBot::HBot(Motor* leftMotor, Motor* rightMotor, int switchPin)
    : leftMotor(leftMotor), rightMotor(rightMotor), switchPin(switchPin)
{
}

void HBot::init() {
    leftMotor->init();
    rightMotor->init();
    pinMode(switchPin, INPUT);
}

/**
 * @brief Calibrates the motors to the switch
 * 
 * @param speed movement speed
 */
void HBot::calibrate(int speed) {
    rightMotor->disable();
    leftMotor->enable();
    leftMotor->setDirection(CCW);
    while (digitalRead(switchPin) == LOW) {
        leftMotor->step(speed);
    }
    leftMotor->reset();
    rightMotor->reset();
}

/**
 * @brief Moves HBot to a position (mm)
 * 
 * @param position destination position
 * @param speed movement speed
 */
void HBot::gotoPosition(Position position, int speed) {
    // Convert position to diagonal vector
    if (position.x < 0 || position.x > X_LIMIT) return;
    if (position.y < 0 || position.y > Y_LIMIT) return;
    Vector diagonal = toDiagonal(position);

    // Calculate the delta
    move({ diagonal.x - leftMotor->getPosition(), diagonal.y - rightMotor->getPosition() }, speed);
}


void HBot::move(Vector vector, int speed) {
    leftMotor->setDirection(vector.x > 0);
    rightMotor->setDirection(vector.y > 0);

    vector.x = abs(vector.x);
    vector.y = abs(vector.y);

    leftMotor->enable();
    rightMotor->enable();

    int minSteps = min(vector.x, vector.y);
    int maxSteps = max(vector.x, vector.y);

    Motor *larger  = vector.x > vector.y ? leftMotor : rightMotor;
    Motor *smaller = vector.x > vector.y ? rightMotor : leftMotor;

    int steps = 0;
    for (; steps < minSteps; steps++) {
        larger->stepInit();
        smaller->stepInit();
        delayMicroseconds(speed);
        larger->stepEnd();
        smaller->stepEnd();
        delayMicroseconds(speed);
    }

    larger->steps(maxSteps - steps, speed);
    
    leftMotor->disable();
    rightMotor->disable();

    Serial.print(leftMotor->getPosition());
    Serial.print(", ");
    Serial.println(rightMotor->getPosition());
}

/*
void HBot::move(Vector vector, int speed) {
    // enable motors
    leftMotor->enable();
    rightMotor->enable();

    // set direction of motors
    leftMotor->setDirection(vector.x > 0);
    rightMotor->setDirection(vector.y > 0);

    // Normalize vector
    vector.x = abs(vector.x);
    vector.y = abs(vector.y);

    // find larger and smaller motors to move
    Motor *larger  = vector.x > vector.y ? leftMotor : rightMotor;
    Motor *smaller = vector.x > vector.y ? rightMotor : leftMotor;

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
    leftMotor->disable();
    rightMotor->disable();
}
*/

/**
 * @brief Converts a position (mm) to a diagonal vector (steps)
 * 
 * @param position 
 * @return Vector 
 */
Vector HBot::toDiagonal(Position position) {
    double left  = (double) (position.y + position.x) * STEPS_PER_MM;
    double right = (double) (position.y - position.x) * STEPS_PER_MM;
    return { (int) left, (int) right };
}
