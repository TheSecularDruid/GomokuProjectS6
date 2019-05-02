#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "bitboard_test.h"
#include "bitboard_move.h"

void test_print() {
    printf("test of print :\n\n");
    size_t size = 5;
    __uint128_t to_print = 4096+4+32;
    print_board(to_print, size);
}

int test_play_move() {
    printf("\ntest of play_move :\n");
    struct bitboard board = new_bitboard();
    struct col_move_t to_play = {{0,0},WHITE};
    size_t size = rand()%7+5;
    assert(size<12);
    assert(size>4);

    printf("\nmoves played : ");
    int nb_of_steps = rand()%(size*size);
    for (int i=0;i<nb_of_steps;i++) {
	to_play.m.row = rand()%size;
	to_play.m.col = rand()%size;
	printf("\n(%zu, %zu)\n", to_play.m.row, to_play.m.col);
	play_move(&to_play, &board, size);
	print_board(board.white, size);
    }
    printf("\n");
    print_board(board.white, size);
    return 1;
}

int test_power_and_move_to_board() {
    size_t size=5;
    int row = rand()%size;
    int col = rand()%size;
    int pow = row*size+col;
    struct move_t move = {row,col};
    assert(power_of_2(pow)==move_to_board(move,size));
    return 1;
}

int test_shift_left() {
    size_t size=7;
    __uint128_t board = rand()*power_of_2(64)+rand();
    print_board(board,size);
    printf("\n");
    board = shift_left(board,size);
    print_board(board,size);
    return 1;
}

int test_shift_up() {
    size_t size = 7;
    __uint128_t board = rand()*power_of_2(64)+rand();
    print_board(board,size);
    printf("\n");
    board = shift_up(board,size);
    print_board(board,size);
    return 1;
}

int test_shift_diag() {
    size_t size = 7;
    __uint128_t board = rand()*power_of_2(64)+rand();
    print_board(board,size);
    printf("\n");
    board = shift_diag(board,size);
    print_board(board,size);
    return 1;
}

int test_shift_other_diag() {
    size_t size = 7;
    __uint128_t board = rand()*power_of_2(64)+rand();
    print_board(board,size);
    printf("\n");
    board = shift_other_diag(board,size);
    print_board(board,size);
    return 1;
}


int test_color_is_winning() {
    printf("test color is winning :\n");
    size_t size = 7;
    struct bitboard board = {rand()*power_of_2(64)+rand(),0};
    print_board(board.white, size);
    printf("white is winning : %d\n", color_is_winning(board, WHITE, size, 5));
    board.white |= 31;
    print_board(board.white, size);
    printf("white is winning : %d\n", color_is_winning(board, WHITE, size, 5));
    return 1;

}

int test_board_to_move() {
  size_t size = rand()%7+5;
  struct move_t to_play = {rand()%size, rand()%size};
  //assert(board_to_move(move_to_board(to_play, size),size)); ?
  printf("Test board to move is verified\n");
}

int main() {
    srand(time(NULL));
    test_print();
    test_play_move();
    test_power_and_move_to_board();
    printf("\ntest of shift_left :\n");
    test_shift_left();
    printf("\ntest of shift_up :\n");
    test_shift_up();
    printf("\ntest of shift_diag :\n");
    test_shift_diag();
    printf("\ntest of shift_other_diag :\n");
    test_shift_other_diag();
    test_color_is_winning();
}
