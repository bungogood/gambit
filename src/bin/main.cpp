#include <Arduino.h>

#include <string>

#include "bluetooth.hpp"
#include "controller.hpp"
#include "fsm.hpp"

Indicator indicator(LED_RED, LED_GREEN, LED_BLUE);

Magnet leftMagnet(9, {0, -30});
Magnet rightMagnet(9, {0, 30});
Magnet upMagnet(9, {20, 0});  // 23
Magnet downMagnet(9, {-33, 0});

Motor leftMotor(2, 3, 4, 200);
Motor rightMotor(5, 6, 7, 200);

HBot hbot(&leftMotor, &rightMotor, 8);

Chess chess;

Board board(&hbot, &upMagnet);

BluetoothManager ble;

Controller controller(&board, &chess, &indicator, &ble);

// clang-format off
std::vector<std::string> game = {
    "e2e4", "c7c6",
    "d2d4", "d7d5",
    "b1c3", "d5e4",
    "c3e4", "b8d7",
    "e4g5", "g8f6",
    "f1d3", "e7e6",
    // "g1f3", "h7h6",
    // "g5e6", "d8e7",
    // "e1g1", "f7e6",
    // "d3g6", "e8d8",
    // "c1f4", "b7b5",
    // "a2a4", "c8b7",
    // "f1e1", "f6d5",
    // "f4g3", "d8c8",
    // "a4b5", "c6b5",
    // "d1d3", "b7c6",
    // "g6f5", "e6f5",
    // "e1e7", "f8e7",
    // "c2c4"
};
// clang-format on

void replay() {
    for (std::string move : game) {
        controller.makeMove(move, 500);
        delay(1000);
    }
}

uint64_t reedSwitchValue;

void setup() {
    Serial.begin(9600);
    delay(1000);  // wait for serial monitor

    // ble.begin("Gambit");
    reedSwitchValue = 0;

    // Serial.println("Starting...");
    controller.init();
    controller.calibrate();
    ble.setFSMState(FSMState::Idle);
    // upMagnet.enable();
    // indicator.init();
    // Serial.println("Calibrated");
    // Move move = {SQD2, SQE3, WPAWN | WHITE, WPAWN, BPAWN | BLACK, SQE3};
    // Move move = {SQC3, SQE4, KNIGHT | WHITE, KNIGHT};
    // Move move = chess.parse_move("a2a4", WHITE);
    // controller.makeMove(move, 500);
    // board.gotoSquare(SQB4);
    // hbot.gotoPosition({250, 0});
    replay();
}

void loop() {
    ble.setReedSwitchValue(reedSwitchValue);
    reedSwitchValue++;
    ble.setFEN(chess.get_fen());
    delay(1000);
}
