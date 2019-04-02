#ifndef BITBOARD_H
#define BITBOARD_H

#include <stdlib.h>
#include <stdio.h>
#include "bitboard_move.h"



struct bitboard{
   __uint128_t white;
   __uint128_t black;
};


struct bitboard new_bitboard(void);
int play_move(const struct col_move_t *move_done, struct bitboard *board, size_t size);
int color_is_winning(struct bitboard board, enum color_t color, size_t size, size_t winning_threshold);
#endif
