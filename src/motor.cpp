#include <Arduino.h>
#include "motor.hpp"

Motor::Motor(int enablePin, int dirPin, int stepPin, int stepsPerRevolution)
    : enablePin(enablePin), dirPin(dirPin), stepPin(stepPin), stepsPerRevolution(stepsPerRevolution)
{
    init();
}

void Motor::init() {
    pinMode(enablePin, OUTPUT);
    pinMode(dirPin, OUTPUT);
    pinMode(stepPin, OUTPUT);
    reset();
}

void Motor::enable() {
    digitalWrite(enablePin, LOW);
    enabled = true;
}

void Motor::disable() {
    digitalWrite(enablePin, HIGH);
    enabled = false;
}

void Motor::stepInit() {
    digitalWrite(stepPin, HIGH);
}

void Motor::stepEnd() {
    digitalWrite(stepPin, LOW);
    position += direction ? 1 : -1;
}

/**
 * @brief Motor moves one step
 * 
 * @param interval 
 */
void Motor::step(int interval) {
    stepInit();
    delayMicroseconds(interval);
    stepEnd();
    delayMicroseconds(interval);
}

/**
 * @brief Steps the motor a certain amount of steps
 * 
 * @param steps number of steps to take
 * @param interval
 */
void Motor::steps(int steps, int interval) {
    enable();
    for(int i = 0; i < steps; i++) {
        step(interval);
    }
}

/**
 * @brief Sets the direction of the motor
 * 
 * @param dir true being clockwise, false being counter-clockwise
 */
void Motor::setDirection(bool dir) {
    digitalWrite(dirPin, dir);
    direction = dir;
}
