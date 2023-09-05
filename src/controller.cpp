
#include "controller.hpp"

Controller::Controller(Board* board, Chess* chess, Indicator* indicator)
    : board(board), chess(chess), indicator(indicator) {}

void Controller::init() {
    indicator->init();
    indicator->set(Color::RED);
    board->init();
    board->calibrate();
    indicator->set(Color::GREEN);
}

void Controller::makeMove(Move move, int speed) {
    std::vector<MoveStep> steps = moveSteps(move, chess);
    indicator->set(Color::BLUE);
    board->move(steps, speed);
    chess->make_move(move);
    indicator->set(Color::GREEN);
}

void Controller::makeMove(std::string move, int speed) {
    Move m = chess->parse_move(move.c_str());
    makeMove(m, speed);
}
