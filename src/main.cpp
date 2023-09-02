#include <Arduino.h>

#include "board.hpp"
#include "indicator.hpp"

Indicator indicator(LED_RED, LED_GREEN, LED_BLUE);

Magnet leftMagnet(9, {0, -30});
Magnet rightMagnet(9, {0, 30});
Magnet upMagnet(9, {23, 0});
Magnet downMagnet(9, {-33, 0});

Magnet magnet(9, {0, 0});

Motor leftMotor(2, 3, 4, 200);
Motor rightMotor(5, 6, 7, 200);

HBot hbot(&leftMotor, &rightMotor, 8);

Chess chess;

Board board(&hbot, &magnet);

void setup() {
    Serial.begin(9600);
    Serial.println("Starting...");
    indicator.set(Color::RED);
    board.init();
    board.calibrate();
    Serial.println("Calibrated");
    indicator.set(Color::GREEN);
    Move move = chess.parse_move("b1c3", WHITE);
    board.move(move, 500);
    indicator.set(Color::BLUE);
}

void loop() {}
