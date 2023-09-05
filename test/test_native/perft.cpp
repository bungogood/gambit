#include <stdio.h>
#include <unity.h>

#include "chess.hpp"

/*
https://www.chessprogramming.org/Perft_Results

Depth	Nodes
0	1
1	20
2	400
3	8,902
4	197,281
5	4,865,609
*/

int perft(int side, int en_passant, Chess* chess, int depth) {
    if (depth == 0) return 1;

    Move_List possible;
    int count = 0;

    if (!chess->generate_moves(side, en_passant, &possible, 0)) return 1;

    for (int i = 0; i < possible.length; i++) {     // loop over move list
        chess->make_move(possible.moves[i], side);  // make move
        count +=
            perft(24 - side, possible.moves[i].skip_square, chess, depth - 1);
        chess->unmake_move(possible.moves[i], side);  // take back
    }

    return count;
}

void testDepth1() {
    Chess chess;
    TEST_ASSERT_EQUAL(20, perft(8, 128, &chess, 1));
}

void testDepth2() {
    Chess chess;
    TEST_ASSERT_EQUAL(400, perft(8, 128, &chess, 2));
}

void testDepth3() {
    Chess chess;
    TEST_ASSERT_EQUAL(8902, perft(8, 128, &chess, 3));
}

void testDepth4() {
    Chess chess;
    TEST_IGNORE_MESSAGE("Currently failing");
    TEST_ASSERT_EQUAL(197281, perft(8, 128, &chess, 4));
}

void testDepth5() {
    Chess chess;
    TEST_IGNORE_MESSAGE("Currently failing");
    TEST_ASSERT_EQUAL(4865609, perft(8, 128, &chess, 5));
}

int main(int argc, char** argv) {
    UNITY_BEGIN();
    RUN_TEST(testDepth1);
    RUN_TEST(testDepth2);
    RUN_TEST(testDepth3);
    RUN_TEST(testDepth4);
    RUN_TEST(testDepth5);
    UNITY_END();
}
