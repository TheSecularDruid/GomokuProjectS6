#include <stdio.h>
#include <stdlib.h>

#include "player.h"

void test_get_player_name() {
    assert(get_player_name()!=NULL);
}

void test_propose_opening(size_t size) {
    col_mov_t* opening_ptr = propose_opening(size);
    //verify the pointer is alright
    assert(opening_ptr!=NULL);
    //verify all moves are of coordinates < size
    for (int i=0;i<3;i++)
	assert((opening_ptr+i)->m.row<size && (opening_ptr+i)->m.col<size);	    
    //verify mov1 is BLACK, mov2 is WHITE, BLACK
    for (int i=0;i<3;i++)
	if (i%2)
	    assert((opening_ptr+i)->c == BLACK);
	else
	    assert((opening_ptr+i)->c == WHITE);
}

void main() {
    int board_size = rand()%7+5;
    
    
}
