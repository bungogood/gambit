#include "controller.hpp"

#include <Arduino.h>

Controller::Controller(Board* board, Chess* chess, Indicator* indicator,
                       BluetoothManager* bluetooth)
    : board(board), chess(chess), indicator(indicator), bluetooth(bluetooth) {}

void Controller::init() {
    board->init();
    indicator->init();
    indicator->set(Color::RED);
    bluetooth->begin("Gambit");
    bluetooth->setFEN(chess->get_fen());
}

void Controller::calibrate(int speed) {
    indicator->set(Color::BLUE);
    board->calibrate(speed);
    indicator->set(Color::GREEN);
}

void Controller::makeMove(Move move, int speed) {
    std::vector<MoveStep> steps = moveSteps(move, chess);
    indicator->set(Color::BLUE);
    chess->make_move(move);
    bluetooth->setFEN(chess->get_fen());
    Serial.println(chess->get_fen().c_str());
    board->move(steps, speed);
    // should do comparision between chess and matrix
    indicator->set(Color::GREEN);
}
