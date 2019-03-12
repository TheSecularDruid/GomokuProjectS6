#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "bitboard_player.h"

void test_get_player_name() {
    assert(get_player_name()!=NULL);
}

void test_propose_opening(size_t size) {
    struct col_move_t* opening_ptr = propose_opening(size);
    //verify the pointer is alright
    assert(opening_ptr!=NULL);
    //verify all moves are of coordinates < size
    for (int i=0;i<3;i++)
	assert((opening_ptr+i)->m.row<size && (opening_ptr+i)->m.col<size);
    //verify mov1 is BLACK, mov2 is WHITE, BLACK
    for (int i=0;i<3;i++)
	if (i%2==0)
	    assert((opening_ptr+i)->c == BLACK);
	else
	    assert((opening_ptr+i)->c == WHITE);
}

/*void test_accept_opening(size_t size, const struct col_move_t* opening) {
    opening=propose_opening(size);
    //returns a boolean
    assert( accept_opening(size, opening) != 0 || accept_opening(size,opening)==0 );
  }*/


/*void test_initialize(size_t size, enum color_t id) {

}
*/

//auxiliary function
int not_played_before(size_t size, struct col_move_t moves[], struct move_t move_done) {
    for (size_t i=0; i<size; i++){
        if (moves[i].m.row ==move_done.row && moves[i].m.col ==move_done.col){
            return 0;
          }
        }
    return 1;
}


void test_play(struct col_move_t moves[], size_t size,struct col_move_t const previous_moves[], size_t n_moves) {
  //move done is within the boundaries
  struct move_t move_done=play(previous_moves , n_moves);
  assert(move_done.row<size);
  assert(move_done.col<size);
  //move done has not been played before
  assert(not_played_before (size, moves, move_done));
}




/*void test_finalize(){

}*/



int main() {
    int board_size = rand()%7+5;
    struct col_move_t moves[5]= {{ {5,4}, WHITE}, { {4,4}, BLACK}, { {5,5}, WHITE}, { {4,3} ,BLACK}, { {3,3}, WHITE}};
    int n_moves=2;
    struct col_move_t const previous_moves[2]={{ {4,3} ,BLACK}, { {3,3}, WHITE}};
    test_get_player_name() ;
    test_propose_opening(board_size);
    test_play(moves, board_size, previous_moves, n_moves);
    printf("every test is successful");
    return 0;
}
