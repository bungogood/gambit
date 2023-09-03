#include <Arduino.h>

#include <string>

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

Board board(&hbot, &upMagnet);

// clang-format off
std::vector<std::string> game = {
    "e2e4", "c7c6",
    "d2d4", "d7d5", 
    // "b1c3", "d5d4"
};
// clang-format on

void replay() {
    int side = WHITE;
    for (std::string move : game) {
        indicator.set(Color::GREEN);
        Move parsed_move = chess.parse_move(move.c_str(), side);
        board.move(parsed_move, 500);
        chess.make_move(parsed_move, side);
        side = side == WHITE ? BLACK : WHITE;
        indicator.set(Color::BLUE);
        delay(1000);
    }
}

void setup() {
    Serial.begin(9600);
    Serial.println("Starting...");
    indicator.set(Color::RED);
    board.init();
    board.calibrate();
    Serial.println("Calibrated");
    // Move move = {SQD2, SQE3, WPAWN | WHITE, WPAWN, BPAWN | BLACK, SQE3};
    // Move move = chess.parse_move("a2a4", WHITE);
    // board.move(move, 600);
    replay();
}

void loop() {}
