#include <Arduino.h>

#include <string>

#include "bluetooth.hpp"
#include "controller.hpp"
#include "fsm.hpp"

Indicator indicator(LED_RED, LED_GREEN, LED_BLUE);

Magnet leftMagnet(D9, {0, -30});
Magnet rightMagnet(D9, {0, 30});
Magnet upMagnet(D9, {20, 0});  // 23
Magnet downMagnet(D9, {-33, 0});

Motor leftMotor(D2, D3, D4, 200);
Motor rightMotor(D5, D6, D7, 200);

HBot hbot(&leftMotor, &rightMotor, D8);

Chess chess;

Board board(&hbot, &upMagnet);

ReedMatrix detection(A3, A5, A4, A1, A0, A2);

BluetoothManager ble;

Controller controller(&board, &detection, &chess, &indicator, &ble);

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

FSMState state;

void replay() {
    for (std::string moveStr : game) {
        Move move = chess.parse_move(moveStr.c_str());
        controller.makeMove(move, 500);
        delay(1000);
    }
}

uint64_t prevBoard;

State_Memory state_memory;
Move_List cur;
Move_List other;

void setup() {
    Serial.begin(9600);
    delay(1000);
    Serial.println("Starting...");
    controller.init();
    // controller.calibrate();
    // Serial.println("Calibrated");
    // Move move = {SQD2, SQE3, WPAWN | WHITE, WPAWN, BPAWN | BLACK, SQE3};
    // Move move = {SQC3, SQE4, KNIGHT | WHITE, KNIGHT};
    // Move move = chess.parse_move("a2a4", WHITE);
    // controller.makeMove(move, 500);
    // board.gotoSquare(SQB4);
    // replay();

    prevBoard = detection.read();
    // state = FSMState::EnemyPU;
    state = FSMState::Idle;
    ble.setFSMState(state);
    ble.setReedSwitchValue(prevBoard);
}

void enemy() {
    if (ble.getMoveUpdated()) {
        delay(10);  // FIX: delay stops ble crash
        std::string moveStr = ble.readMove();
        Serial.println(moveStr.c_str());
        Move move = chess.parse_move(moveStr.c_str());
        chess.make_move(move);
        state = FSMState::FriendlyPU;
        ble.setFEN(chess.get_fen());
        ble.setMove(move);
        ble.setFSMState(state);
    }
}

void friendly() {
    Search_Info search_info;
    chess.get_best_move(3, &search_info);
    Move move = search_info.best_move;
    chess.make_move(move);
    state = FSMState::EnemyPU;
    ble.setFEN(chess.get_fen());
    ble.setMove(move);
    ble.setFSMState(state);
}

int countOnes(uint64_t value) {
    return __builtin_popcountll(value);  // Works for uint64_t
}

int findLSBIndex(uint64_t value) {
    if (value == 0) return -1;  // No bits are set, return -1
    // ffsll returns 1-based index, so subtract 1 for 0-based index
    return __builtin_ffsll(value) - 1;
}

void loop() {
    // switch (state) {
    //     case FSMState::EnemyPU:
    //         enemy();
    //         break;
    //     case FSMState::FriendlyPU:
    //         friendly();
    //         break;
    // }
    uint64_t currentBoard = detection.read();
    ble.setReedSwitchValue(currentBoard);
    delay(10);
    uint64_t diff = currentBoard ^ prevBoard;
    if (diff && countOnes(diff) == 1) {
        bool pickedUp = prevBoard & diff;
        // xor and find the changed bit and its index
        int sq8x8 = findLSBIndex(diff);
        int square = to_square(sq8x8);
        if (pickedUp) {
            Serial.println("Picked up: " + String(sq8x8) +
                           " Square: " + String(square));
        } else {
            Serial.println("Put down: " + String(sq8x8) +
                           " Square: " + String(square));
        }

        chess.generate_moves(&cur);
        state =
            update_state(&chess, square, state, &state_memory, &cur, &other);

        if (state == FSMState::MoveComplete) {
            ble.setFEN(chess.get_fen());
            state = FSMState::Idle;
            state_memory.length = 0;
        }

        ble.setFSMState(state);
        Serial.println(String(fsm_state_string(state).c_str()) + " " +
                       String(state_memory.length));
    } else if (state == FSMState::Error &&
               currentBoard == chess.get_occupied()) {
        state = FSMState::Idle;
        ble.setFSMState(state);
    }
    prevBoard = currentBoard;
}
