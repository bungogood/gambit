#pragma once

#include <string>

#include "board.hpp"
#include "bridge.hpp"
#include "chess.hpp"
#include "indicator.hpp"

class Controller {
   private:
    Board* board;
    Chess* chess;
    Indicator* indicator;

   public:
    Controller(Board* board, Chess* chess, Indicator* indicator);
    void init();

    void makeMove(Move move, int speed = DEFAULT_SPEED);
    void makeMove(std::string move, int speed = DEFAULT_SPEED);
};
