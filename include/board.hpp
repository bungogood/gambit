#pragma once

#include <vector>

#include "bridge.hpp"
#include "hbot.hpp"
#include "magnet.hpp"

#define SQUARE_SIZE 50
#define OFFSET_FILE 20  // A-H
#define OFFSET_RANK 7   // 1-8

class Board {
   private:
    HBot* hbot;
    Magnet* magnet;

    void runSteps(std::vector<MoveStep>, int speed = DEFAULT_SPEED);

   public:
    Board(HBot* hbot, Magnet* magnet);
    void init();

    void calibrate(int speed = CALIBRATION_SPEED);
    void move(std::vector<MoveStep> steps, int speed = DEFAULT_SPEED);
    void gotoSquare(Square square, int speed = DEFAULT_SPEED);
};
