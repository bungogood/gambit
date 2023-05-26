#pragma once

#include "magnet.hpp"
#include "chess.hpp"
#include "hbot.hpp"

#define SQUARE_SIZE 50
#define OFFSET_RANK 25 // 1-8
#define OFFSET_FILE 50 // A-H

class Board {
    HBot* hbot;
public:
    Board(HBot* hbot);

    void calibrate(int speed = CALIBRATION_SPEED);
    void move(Move move, int speed = DEFAULT_SPEED);
    void gotoSquare(Square square, int speed = DEFAULT_SPEED);
};
