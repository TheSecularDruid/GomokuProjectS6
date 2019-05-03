#ifndef PLAYER_TEST_H
#define PLAYER_TEST_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "bitboard_player.h"

int test_get_player_name();
int test_propose_opening(size_t size);
int test_play(struct col_move_t moves[], size_t size,struct col_move_t const previous_moves[], size_t n_moves);
#endif
