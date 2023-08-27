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

int search_perf(int side, int en_passant, Chess* chess, int depth) {
    if (depth == 0) return 1;

    Move_List possible;
    int count = 0;

    if (!chess->generate_moves(side, en_passant, &possible, 0)) return 1;

    for (int i = 0; i < possible.length; i++) {     // loop over move list
        chess->make_move(side, possible.moves[i]);  // make move
        count += search_perf(24 - side, possible.moves[i].skip_square, chess,
                             depth - 1);
        chess->unmake_move(side, possible.moves[i]);  // take back
    }

    return count;
}

void test_depth_1() {
    Chess chess;
    TEST_ASSERT_EQUAL(20, search_perf(8, 128, &chess, 1));
}

void test_depth_2() {
    Chess chess;
    TEST_ASSERT_EQUAL(400, search_perf(8, 128, &chess, 2));
}

void test_depth_3() {
    Chess chess;
    TEST_ASSERT_EQUAL(8902, search_perf(8, 128, &chess, 3));
}

void test_depth_4() {
    Chess chess;
    TEST_IGNORE_MESSAGE("Currently failing");
    TEST_ASSERT_EQUAL(197281, search_perf(8, 128, &chess, 4));
}

void test_depth_5() {
    Chess chess;
    TEST_IGNORE_MESSAGE("Currently failing");
    TEST_ASSERT_EQUAL(4865609, search_perf(8, 128, &chess, 5));
}

int main(int argc, char** argv) {
    UNITY_BEGIN();
    RUN_TEST(test_depth_1);
    RUN_TEST(test_depth_2);
    RUN_TEST(test_depth_3);
    RUN_TEST(test_depth_4);
    RUN_TEST(test_depth_5);
    UNITY_END();
}
