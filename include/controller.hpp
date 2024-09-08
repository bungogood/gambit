#pragma once

#include <string>

#include "bluetooth.hpp"
#include "board.hpp"
#include "bridge.hpp"
#include "chess.hpp"
#include "indicator.hpp"

class Controller {
   private:
    Board* board;
    Chess* chess;
    Indicator* indicator;
    BluetoothManager* bluetooth;

   public:
    Controller(Board* board, Chess* chess, Indicator* indicator,
               BluetoothManager* bluetooth);
    void init();

    void calibrate(int speed = CALIBRATION_SPEED);

    void makeMove(Move move, int speed = DEFAULT_SPEED);
    void makeMove(std::string move, int speed = DEFAULT_SPEED);
};
