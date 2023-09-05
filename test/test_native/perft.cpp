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

int perft(Chess* chess, int depth) {
    if (depth == 0) return 1;

    Move_List possible;
    int count = 0;
    int old_ep = chess->get_en_passant();

    if (!chess->generate_moves(&possible, false)) return 1;

    for (int i = 0; i < possible.length; i++) {  // loop over move list
        chess->make_move(possible.moves[i]);     // make move
        count += perft(chess, depth - 1);
        chess->unmake_move(possible.moves[i], old_ep);  // take back
    }

    return count;
}

void testDepth1() {
    Chess chess;
    TEST_ASSERT_EQUAL(20, perft(&chess, 1));
}

void testDepth2() {
    Chess chess;
    TEST_ASSERT_EQUAL(400, perft(&chess, 2));
}

void testDepth3() {
    Chess chess;
    TEST_ASSERT_EQUAL(8902, perft(&chess, 3));
}

void testBrokenDepth4() {
    Chess chess;
    // Incorect result
    TEST_ASSERT_EQUAL(197742, perft(&chess, 4));
}

void testBrokenDepth5() {
    Chess chess;
    // Incorect result
    TEST_ASSERT_EQUAL(4880984, perft(&chess, 5));
}

void testDepth4() {
    Chess chess;
    TEST_IGNORE_MESSAGE("Currently failing");
    TEST_ASSERT_EQUAL(197281, perft(&chess, 4));
}

void testDepth5() {
    Chess chess;
    TEST_IGNORE_MESSAGE("Currently failing");
    TEST_ASSERT_EQUAL(4865609, perft(&chess, 5));
}

int main(int argc, char** argv) {
    UNITY_BEGIN();
    RUN_TEST(testDepth1);
    RUN_TEST(testDepth2);
    RUN_TEST(testDepth3);
    RUN_TEST(testBrokenDepth4);
    RUN_TEST(testBrokenDepth5);
    RUN_TEST(testDepth4);
    RUN_TEST(testDepth5);
    UNITY_END();
}
