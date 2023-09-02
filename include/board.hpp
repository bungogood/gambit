#pragma once

#include <vector>

#include "hbot.hpp"
#include "magnet.hpp"
#include "translator.hpp"

#define SQUARE_SIZE 50
#define OFFSET_FILE 20  // A-H
#define OFFSET_RANK 7   // 1-8

class Board {
    HBot* hbot;
    Magnet* magnet;

    void runSteps(std::vector<MoveStep>, int speed = DEFAULT_SPEED);

   public:
    Board(HBot* hbot, Magnet* magnet);
    void init();

    void calibrate(int speed = CALIBRATION_SPEED);
    void move(Move move, int speed = DEFAULT_SPEED);
    void gotoSquare(Square square, int speed = DEFAULT_SPEED);
};
