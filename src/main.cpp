#include <Arduino.h>

#include "board.hpp"
#include "indicator.hpp"

Indicator indicator(14, 15, 16);

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
    indicator.init();
    indicator.set(true, false, false);
    board.init();
    board.calibrate();
    Serial.println("Calibrated");
    indicator.set(false, true, false);
    Move move = chess.parse_move("a2a4", WHITE);
    board.move(move, 500);
    indicator.set(false, false, true);
}

void loop() {}
