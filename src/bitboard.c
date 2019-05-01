#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "bitboard.h"
#include "bitboard_move.h"

#define THRESH 5

struct bitboard new_bitboard() {
    struct bitboard b = {0,0};
    return b;
}

__uint128_t power_of_2(int power) {
    __uint128_t result = 2;
    while(power>1) {
	result *= 2;
	power--;
    }
    return result;
}

__uint128_t move_to_board(struct move_t move, size_t size) {
    return (power_of_2( move.row * size + move.col ));
}
//--------------------outdated---------------------------//
/*
__uint128_t eqor(__uint128_t arg1, __uint128_t arg2) {
    unsigned long long int arg1_top = (arg1>>64);
    unsigned long long int arg1_bot = (unsigned long long int) arg1;
    unsigned long long int arg2_top = (arg2>>64);
    unsigned long long int arg2_bot = (unsigned long long int) arg2;
    arg1_top |= arg2_top;
    arg1_bot |= arg2_bot;
    return (((__uint128_t)arg1_top<<64) + (__int128_t)(arg1_bot));
}
*/
int play_move(const struct col_move_t* move_done, struct bitboard* board, size_t size) {
    __uint128_t boardmove = move_to_board(move_done->m, size);
    if (move_done->c==WHITE)
	board->white |= boardmove;
    if (move_done->c==BLACK)
        board->black |= boardmove;
    return 1;
}

__uint128_t shift_left(__uint128_t to_shift, size_t size) {
    __uint128_t mask = 0;
    for(size_t i=0;i<size*size;i++) {
	if (i%size==0)
	    mask*=2;
	else {
	    mask+=1;
	    mask*=2;
	}
    }
    to_shift = to_shift & mask;
    return (to_shift>>1);
}

__uint128_t shift_up(__uint128_t to_shift, size_t size) {
    __uint128_t mask = power_of_2(size*size)-power_of_2(size);
    to_shift &= mask;
    return (to_shift>>size);
}

__uint128_t shift_diag(__uint128_t to_shift, size_t size) {
    __uint128_t mask = 0;
    for(size_t i=size;i<size*size;i++) {
	if (i%size==0)
	    mask*=2;
	else {
	    mask+=1;
	    mask*=2;
	}
    }
    mask *= power_of_2(size);
    
    to_shift &= mask;
    return (to_shift>>(size+1));
}

__uint128_t shift_other_diag(__uint128_t to_shift, size_t size) {
    __uint128_t mask = 0;
    for(size_t i=size;i<size*size;i++) {
	if (i%size==1)
	    mask*=2;
	else {
	    mask+=1;
	    mask*=2;
	}
    }
    mask *= power_of_2(size);
    mask += power_of_2(size);
 
    to_shift&= mask;
    return (to_shift>>(size-1));
}

int color_is_winning(struct bitboard board, enum color_t color, size_t size, size_t winning_threshold) {
    __uint128_t bits;
    __uint128_t (*shift) (__uint128_t, size_t);

    for (size_t i = 0;i<4;i++) {
	if (color==WHITE)
	    bits = board.white;
	if (color==BLACK)
	    bits = board.black;
	
	switch (i) {
	case 0:
	    shift = &shift_left;
	    break;
	case 1:
	    shift = &shift_up;
	    break;
	case 2:
	    shift = &shift_diag;
	    break;
	case 3:
	    shift = &shift_other_diag;
	    break;
	}
	__uint128_t shifted = bits;
	for(size_t k=0;k<(winning_threshold-1);k++) {
	    shifted = shift(shifted,size);
	    bits &= shifted;
	}
	if (bits!=0)
	    return 1;
    }
    return 0;
}

struct move_t board_to_move(__uint128_t board, size_t size) {    //board must, of course, contain only one move
  struct move_t ret = {0,0};
  while(board!=1) {
    ret.row++;
    if(ret.row==size) {
      ret.col++;
      ret.row = 0;
    }
    board/=2;
  }
}

move_t detect_line(struct bitboard board, enum color_t color, size_t size, size_t size_line) {
  __uint128_t 
};

void print_board(__uint128_t to_print, size_t size) {
    __uint128_t mask = 1;
    for (size_t i=0;i<size;i++) {
	for (size_t j=0;j<size;j++) {
	    if (mask&to_print)
		printf("%d ", 1);
	    else
		printf("%d ", 0);
	    mask*=2;
	}
	printf("\n");
    }
}
