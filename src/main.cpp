#include <Arduino.h>
#include "magnet.hpp"
#include "motor.hpp"
#include "hbot.hpp"

Magnet leftMagnet (9, {-30,  0 });
Magnet rightMagnet(9, { 30,  0 });
Magnet upMagnet   (9, {  0, 23 });
Magnet downMagnet (9, {  0,-33 });

Motor leftMotor(2, 3, 4, 200);
Motor rightMotor(5, 6, 7, 200);

HBot hbot(&leftMotor, &rightMotor, 8);

void setup() {
    Serial.begin(9600);
    Serial.println("Starting...");
    hbot.calibrate();
    Serial.println("Calibrated");
    int speed = 400;
    hbot.gotoPosition({ 100, 100 }, speed);
    hbot.gotoPosition({ 100, 300 }, speed);
    hbot.gotoPosition({ 300, 300 }, speed);
    hbot.gotoPosition({ 300, 100 }, speed);
    hbot.gotoPosition({ 100, 100 }, speed);
}

void loop() {
}
