#include "player_test.h"

int test_get_player_name() {
    assert(get_player_name()!=NULL);
    return 1;
    //printf("Assertion of get_player_name() is successful!\n");
}

int test_propose_opening(size_t size) {
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
  return 1;
  //printf("Assertion of propose_opening() is successful!\n");
}

//auxiliary function
int not_played_before(size_t size, struct col_move_t moves[], struct move_t move_done) {
    for (size_t i=0; i<size; i++){
        if (moves[i].m.row ==move_done.row && moves[i].m.col ==move_done.col){
            return 0;
          }
        }
    return 1;
}

int test_play(struct col_move_t moves[], size_t size,struct col_move_t const previous_moves[], size_t n_moves) {
  //move done is within the boundaries
  struct move_t move_done=play(previous_moves , n_moves);
  assert(move_done.row<size);
  assert(move_done.col<size);
  //move done has not been played before
  assert(not_played_before (size, moves, move_done));
  return 1;
  //printf("Assertion of play() is successful!\n");
}
