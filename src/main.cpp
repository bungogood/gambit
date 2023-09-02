#include <Arduino.h>

#include "board.hpp"

Magnet leftMagnet(9, {0, -30});
Magnet rightMagnet(9, {0, 30});
Magnet upMagnet(9, {23, 0});
Magnet downMagnet(9, {-33, 0});

Magnet magnet(9, {0, 0});

Motor leftMotor(2, 3, 4, 200);
Motor rightMotor(5, 6, 7, 200);

HBot hbot(&leftMotor, &rightMotor, 8);

Chess chess;

Board board(&hbot, &magnet, &chess);

void setup() {
    Serial.begin(9600);
    Serial.println("Starting...");
    board.init();
    board.calibrate();
    Serial.println("Calibrated");
    Move move = chess.parse_move("a2a4", WHITE);
    board.move(move, 500);
}

void loop() {}
