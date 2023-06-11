#include "state_machine.hpp"
#include "chess.hpp"
#include <stdio.h>
#include <string.h>

/*********************************************************************************\
;---------------------------------------------------------------------------------;
;                                Helper Functions                                 ;
;---------------------------------------------------------------------------------;
\*********************************************************************************/

void print_bitboard(unsigned long long bitboard){
    for(int i = 0 ; i < 64 ; i++){
        if(i%8 == 0) {printf("\n");}
        if(bitboard & 1ull<<(63-i)){
            printf("X ");
        }
        else{
            printf(". ");
        }
    }
}

unsigned long long get_bitboard_from_square(int square){
    int mod = square%8;
    int div = square/8;
    return 1ull<<(63 - ((div/2*8) + mod));
}

/*********************************************************************************\
;---------------------------------------------------------------------------------;
;                                Actual Functions                                 ;
;---------------------------------------------------------------------------------;
\*********************************************************************************/

State update_state(Chess chess, int instruction, bool colour, State state, int state_memory[]){
    //chess.print_board();
    unsigned long long square = get_bitboard_from_square(instruction);
    unsigned long long occupied = chess.get_occupied();
    unsigned long long friendlies;
    unsigned long long enemies;

    if(colour){  //black turn
        friendlies = chess.get_black();
        enemies = chess.get_white();
    }
    else{ //white turn
        friendlies = chess.get_white();
        enemies = chess.get_black();
    }

    printf("%llu \n", friendlies & square);
    printf("%d \n", ((friendlies & square) == 0ull));

    switch(state){
        case Idle:
            if((friendlies & square) != 0ULL){
                state_memory[0] = instruction;
                return State::FriendlyPU;
            }
            if((enemies & square) != 0ULL){

            }
            else{
                return State::Error;
            }
            break;
        case FriendlyPU:
            Move_List move_list[1];
            chess.generate_moves(colour, 128/*change that later*/, move_list, false);
            if(instruction == state_memory[0]){
                return State::Idle;
            }
            else if ((friendlies & square) != 0ULL){
                return State::InvalidPiecePU;
            }
            else{
                chess.make_move(colour, chess.move)
            }
            break;
    }

    return state;
}

/*********************************************************************************\
;---------------------------------------------------------------------------------;
;                                      Main                                       ;
;---------------------------------------------------------------------------------;
\*********************************************************************************/

int main() {
    Chess chess;
    enum State state = Idle;
    int state_memory[] = {0};

    printf("hello world\n");
    printf("%d \n", state);

    print_bitboard(chess.get_occupied());
    printf("\n");
    bool colour = 0; //false = white, true = black
    while(true){
        int square;
        printf("Type number of square: \n");
        printf("current state is: %d \n", state);
        scanf("%d", &square);
        printf("the square is %d \n", square);
        state = update_state(chess, square, colour, state, state_memory);
        printf("current state is: %d \n", state);
        if(colour) {colour = false;} else {colour = true;}
    }
}

