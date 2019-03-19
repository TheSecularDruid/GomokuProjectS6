#include "bitboard.h"
#include "bitboard_move.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
// the board is to be read from top to bottom and from left to right starting from the lightest bit to the heaviest one

struct bitboard new_bitboard(){
  struct bitboard b;
  b.white=0;
  b.black=0;
  return b;
}

int move_played (struct col_move_t *move_done, struct bitboard *board, size_t size){
    int cell_number= move_done->m.row*size+move_done->m.col;
    if (move_done->c==WHITE)
        board->white=board->white | (1<<cell_number);
    if (move_done->c==BLACK)
        board->black=board->black | (1<<cell_number);
    return 0;
}

int color_is_winning(__uint128_t colored_board, size_t size, size_t winning_threshold){ //winning_threshold is 5 in the classical rules
    __uint128_t pattern_diag_down=0;
    __uint128_t pattern_diag_up=0;
    __uint128_t pattern_horiz=0;
    __uint128_t pattern_vert=0;
    for (size_t k=0; k<winning_threshold; k++){
        pattern_diag_down += 1<<(k*(size+1));
        pattern_diag_up += (1<<(size-1))<<(k*(size-1));
        pattern_vert += 1<<(k*size);
        pattern_horiz += 1<<k;
    }
    for (size_t i=0; i<size-winning_threshold+1; i++) {
        for (size_t j=0; j<size-winning_threshold+1; j++) {
            if ( ( (pattern_diag_up & colored_board)== pattern_diag_up) || ( (pattern_diag_down & colored_board) == pattern_diag_down) )
                return 1;
            pattern_diag_down<<=1;
            pattern_diag_up>>=1;
      }
        pattern_diag_up>>=(winning_threshold-1);
        pattern_diag_down<<=(winning_threshold-1);
    }
    for (size_t i=0; i<size; i++) {
        for (size_t j=0; j<size-winning_threshold+1; j++) {
            if ((pattern_vert & colored_board ) == pattern_vert)
                return 1;
            pattern_vert<<=1;
        }
    }
    for (size_t i=0; i<size-winning_threshold+1; i++) {
        for (size_t j=0; j<size; j++) {
            if ((pattern_horiz & colored_board) == pattern_horiz)
                return 1;
            pattern_horiz<<=1;
        }
        pattern_horiz<<=winning_threshold-1;
    }
    return 0;
  }
  //TODO vérifier que la ligne est de longueur au plus 5
  int main(){
    printf ("%d\n" ,color_is_winning(1+32+32*32+32*32*32+32*32*32*32, 5, 5));
        return 0;
  }
