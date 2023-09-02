#include "board.hpp"

#include <Arduino.h>

Board::Board(HBot* hbot, Magnet* magnet) : hbot(hbot), magnet(magnet) {
    init();
}

void Board::init() { hbot->init(); }

void Board::calibrate(int speed) { hbot->calibrate(speed); }

void Board::gotoSquare(Square square, int speed) {
    Position position = {(square % 16) * SQUARE_SIZE + OFFSET_FILE,
                         (7 - square / 16) * SQUARE_SIZE + OFFSET_RANK};
    hbot->gotoPosition(position, speed);
}

void Board::move(Move move, int speed) {
    std::vector<MoveStep> steps = moveSteps(move);
    runSteps(steps, speed);
}

void Board::runSteps(std::vector<MoveStep> steps, int speed) {
    for (MoveStep step : steps) {
        // magnet selection
        Position position = {(step.point.x - 2) * SQUARE_SIZE / 2 +
                                 OFFSET_FILE - magnet->offset.x,
                             (step.point.y - 2) * SQUARE_SIZE / 2 +
                                 OFFSET_RANK - magnet->offset.y};
        if (step.magnet) {
            magnet->enable();  // magnet on
        } else {
            magnet->disable();  // magnet off
        }
        delay(200);
        hbot->gotoPosition(position, speed);
    }
}
