#include "bitboard.h"
#include "bitboard_move.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

// the board is to be read from top to bottom and from left to right starting from the lightest bit to the heaviest one

struct bitboard new_bitboard(){ //creates a new empty bitboard for black and white
  struct bitboard b;
  b.white=0;
  b.black=0;
  return b;
}
__uint128_t bitwise_or(__uint128_t a, __uint128_t b){
  __uint128_t res= 0;
  unsigned long long c=(unsigned long long) a;
  unsigned long long d=(unsigned long long) b;
  unsigned long long e=c | d;
  unsigned long long f=(unsigned long long) (a>>64);
  unsigned long long g=(unsigned long long) (b>>64);
  unsigned long long h=f | g;
  res= h;
  res<<=64;
  res+=e;
  return res;
}

int play_move (const struct col_move_t *move_done, struct bitboard *board, size_t size){ //plays a move
    int cell_number= move_done->m.row*size+move_done->m.col;
    if (move_done->c==WHITE)
        board->white=bitwise_or(board->white , 1<<cell_number);
    if (move_done->c==BLACK)
        board->black=bitwise_or(board->black , 1<<cell_number);
    return 0;
}

__uint128_t bitwise_and(__uint128_t a, __uint128_t b){
  __uint128_t res= 0;
  unsigned long long c=(unsigned long long) a;
  unsigned long long d=(unsigned long long) b;
  unsigned long long e=c&d;
  unsigned long long f=(unsigned long long) (a>>64);
  unsigned long long g=(unsigned long long) (b>>64);
  unsigned long long h=f&g;
  res= h;
  res<<=64;
  res+=e;
  return res;
}

int pattern_is_matching(__uint128_t pattern, __uint128_t colored_board){
    return ( bitwise_and(pattern, colored_board)== pattern );
}

int color_is_winning(struct bitboard board, enum color_t color, size_t size, size_t winning_threshold){ //winning_threshold is 5 in the classical rules
    __uint128_t colored_board;
    switch (color) {
        case BLACK:
            colored_board=board.black;
            break;
        case WHITE:
            colored_board=board.white;
            break;
        default:
          printf ("other colors are yet to be added");
    }

    __uint128_t pattern_diag_down=0;
    __uint128_t pattern_diag_up=0;
    __uint128_t pattern_horiz=0;
    __uint128_t pattern_vert=0;
    for (size_t k=0; k<winning_threshold; k++){
        pattern_diag_down += 1<<(k*(size+1));
        pattern_diag_up += (1<<(winning_threshold-1)) <<(k*(size-1));
        pattern_vert += 1<<(k*size);
        pattern_horiz += 1<<k;
    }
    for (size_t i=0; i<size-winning_threshold+1; i++) {
        for (size_t j=0; j<size-winning_threshold+1; j++) {
            if (   (  ((i==size-winning_threshold && j==size-winning_threshold) || (i==0 && j==0)) //upward diagonal being stuck in a corner there are no overlines possible
                &&  pattern_is_matching(pattern_diag_up, colored_board )  )
            || (  ( (i==0) || (j==size-winning_threshold) ) //upward diagonal touches left or bottom border
                && ( pattern_is_matching(pattern_diag_up, colored_board) && pattern_is_matching(pattern_diag_up>>(size-1), colored_board)==0 )  )
            || (  ( (i== size-winning_threshold) || (j==0) ) //upward diagonal touches right or top border
                && ( pattern_is_matching(pattern_diag_up, colored_board) && pattern_is_matching(pattern_diag_up<<(size-1), colored_board)==0 )  )
            || ( pattern_is_matching(pattern_diag_up, colored_board) && pattern_is_matching(pattern_diag_up<<(size-1), colored_board)==0  //upward diagonal doesn't touch any corner: overlines are possibles both ways
                && pattern_is_matching(pattern_diag_up>>(size-1), colored_board)==0 ) )
                    return 1;
            pattern_diag_up<<=1;

        }
        pattern_diag_up<<=(winning_threshold-1);
    }
    for (size_t i=0; i<size; i++) {
        for (size_t j=0; j<size-winning_threshold+1; j++) {
            if (  ( (i==0) //vertical line touches top border
                    && ( pattern_is_matching(pattern_vert, colored_board) && (pattern_is_matching(pattern_vert<<size, colored_board)==0) )  )
                || ( (i==size-winning_threshold) //vertical line touches bottom border
                    && ( pattern_is_matching(pattern_vert, colored_board) && pattern_is_matching(pattern_vert>>size, colored_board)==0 )  )
                || ( pattern_is_matching(pattern_vert,colored_board) && pattern_is_matching(pattern_vert>>size, colored_board)==0
                    && pattern_is_matching(pattern_vert<<size, colored_board)==0 )  )//vertical line doesn't touch any border
                return 1;
                pattern_vert<<=1;
        }
    }
    for (size_t i=0; i<size; i++) {
        for (size_t j=0; j<size-winning_threshold+1; j++) {
            if (  ( (j==0) //horizontal line touches left border
                    && ( pattern_is_matching(pattern_horiz, colored_board) && pattern_is_matching(pattern_horiz<<1, colored_board)==0 )  )
                || ( (j==size-winning_threshold) //horizontal line touches right border
                    && ( pattern_is_matching(pattern_horiz, colored_board) && pattern_is_matching(pattern_horiz>>1, colored_board)==0 )  )
                || ( pattern_is_matching(pattern_horiz,colored_board) && pattern_is_matching(pattern_horiz>>1, colored_board)==0
                    && pattern_is_matching(pattern_horiz<<1, colored_board)==0 )  )//horizontal line doesn't touch any border
                return 1;
                pattern_horiz<<=1;
        }
        pattern_horiz<<=(winning_threshold-1);
    }
    for (size_t i=0; i<size-winning_threshold+1; i++) {
        for (size_t j=0; j<size-winning_threshold+1; j++) {
            if (   (  ( (i==0 && j==size-winning_threshold) || (i==size-winning_threshold && j==0) ) //downward diagonal being stuck in a corner there are no overlines possible
                && pattern_is_matching(pattern_diag_down, colored_board)   )
            || (  ( (i==0) || (j==0) ) //downward diagonal touches left or top border
                && ( pattern_is_matching(pattern_diag_down, colored_board) && pattern_is_matching(pattern_diag_down<<(size+1), colored_board)==0 )  )
            || (  ( (i==size-winning_threshold) || (j==0) ) //downward diagonal touches right or bottom border
                && ( pattern_is_matching(pattern_diag_down, colored_board) && pattern_is_matching(pattern_diag_down>>(size+1), colored_board)==0 )  )
            || ( pattern_is_matching(pattern_diag_down, colored_board) && pattern_is_matching(pattern_diag_down>>(size+1), colored_board)==0  //downward diagonal doesn't touch any corner: overlines are possibles both ways
                && pattern_is_matching(pattern_diag_down>>(size+1), colored_board)==0 ) )
                    return 1;
            pattern_diag_down<<=1;

        }
        pattern_diag_down<<=(winning_threshold-1);
    }
    return 0;
  }
  //TODO vérifier que la ligne est de longueur au plus 5 DONE
  int main(){
    struct bitboard b= new_bitboard();
    __uint128_t x= 1+128+16384+16384*128+16384*128*128;
    __uint128_t y= 1+64+64*64+64*64*64+64*64*64*64+64*64*64*64*64;
    //x+=64*64*64*64*64;
    b.white=x;
    printf("%llu\n", bitwise_and(x, y) );
    printf("%d\n", pattern_is_matching( x,y));
    printf ("%d\n" ,color_is_winning(b ,WHITE, 6, 5));
        return 0;
  }
