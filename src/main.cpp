#include <Arduino.h>
#include "motor.hpp"
#include "hbot.hpp"

Motor leftMotor(2, 3, 4, 200);
Motor rightMotor(5, 6, 7, 200);

HBot hbot(&leftMotor, &rightMotor, 8);

void setup() {
    Serial.begin(9600);
    Serial.println("Starting...");
    hbot.calibrate();
    Serial.println("Calibrated");
    hbot.gotoPosition({ 200, 250 }, 500);
}

void loop() {
}
