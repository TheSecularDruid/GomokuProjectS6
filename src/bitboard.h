#ifndef BITBOARD_H
#define BITBOARD_H

#include <stdlib.h>
#include <stdio.h>
#include "bitboard_move.h"

struct bitboard{
   __uint128_t white;
   __uint128_t black;
};


struct bitboard new_bitboard();
int play_move(const struct col_move_t* move_done, struct bitboard* board, size_t size);
__uint128_t shift_left(__uint128_t to_shift, size_t size);
__uint128_t shift_up(__uint128_t to_shift, size_t size);
__uint128_t shift_diag(__uint128_t to_shift, size_t size);
__uint128_t shift_other_diag(__uint128_t to_shift, size_t size);
__uint128_t power_of_2(int power);
__uint128_t move_to_board(struct move_t move, size_t size);
int color_is_winning(struct bitboard board, enum color_t color, size_t size, size_t winning_threshold);
struct move_t board_to_move(__uint128_t board, size_t size);
int min(int a, int b);
int max(int a, int b);
int max_line_length(struct bitboard board, enum color_t color, size_t size);
int nb_of_poss(struct bitboard board,size_t size);
void possible_moves(struct move_t memory[],int length, struct bitboard board, size_t size);
void print_board(__uint128_t to_print, size_t size);
#endif
