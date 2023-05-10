#include <Arduino.h>
#include "motor.hpp"
#include "hbot.hpp"

Motor leftMotor(2, 3, 200);
Motor rightMotor(4, 5, 200);

HBot hbot(leftMotor, rightMotor, 6);

void setup() {
    Serial.begin(9600);
    Serial.println("Starting...");
    hbot.calibrate();
    Serial.println("Calibrated");
}

void loop() {
}
