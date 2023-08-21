#include "board.hpp"

Board::Board(HBot* hbot) : hbot(hbot) {}

void Board::init() { hbot->init(); }

void Board::calibrate(int speed) { hbot->calibrate(speed); }

void Board::gotoSquare(Square square, int speed) {
    Position position = {(square % 8) * SQUARE_SIZE + OFFSET_FILE,
                         (square / 8) * SQUARE_SIZE + OFFSET_RANK};
    hbot->gotoPosition(position, speed);
}

void Board::move(Move move, int speed) {
    // add multiphase move support
    // if knight, capture, castle move then move to center of square
    gotoSquare(move.source_square, speed);
    gotoSquare(move.target_square, speed);
}
