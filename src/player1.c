#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "bitboard_move.h"
#include "bitboard_player.h"

//This player just plays randomly something that they can play - pretty much a test in itself



char* grid = NULL;
size_t grid_size = 0;
int color = 0;

char const* get_player_name() {
  return("Random 1");
}

int is_already_played_opening(size_t a, size_t b, struct col_move_t const moves[], int size) {
    int k = 0;
    while (k++<size)
	if (a==moves[k].m.row && b==moves[k].m.col)
	    return 1;
    return 0;
}

struct col_move_t* propose_opening(size_t size) {
    struct col_move_t* opening = malloc(sizeof(struct col_move_t[3]));
    size_t abscissa=-1;
    size_t ordinate=-1;
    for (int i=0;i<3;i++) {
	do {
	    abscissa = rand()%size;
	    ordinate = rand()%size;
	} while (is_already_played_opening(abscissa,ordinate,opening,i));
	opening[i].m.row = abscissa;
	opening[i].m.col = ordinate;
	if (i%2)   //if i is odd
	    opening[i].c = WHITE;
	else
	    opening[i].c = BLACK;
    }
    return opening; //WARNING : this memory would be freed by the server in the current implementation - should be further thought out
}

int accept_opening(size_t size, const struct col_move_t* opening) {
    return 1;
}

void initialize(size_t size, enum color_t id) {
    grid = malloc(sizeof(char[size*size]));
    for (int k=0;k<size*size;k++) {
	grid[k]=-1;
    }
    grid_size = size;
    color  = id;
    srand(time(NULL));
}

int move_to_grid_cell(struct move_t move) {
    return (move.row*grid_size+move.col);
}

int was_already_played(struct move_t move) {
    if (grid[move_to_grid_cell(move)]==-1)
	return 0;
    else
	return 1;
}

int is_out_of_bounds(struct move_t move) {
    return (move_to_grid_cell(move)>(grid_size*grid_size));
}


int update_grid(struct col_move_t const previous_moves[], size_t n_moves) {
    for (int i=0;i<n_moves;i++) {
	if(was_already_played(previous_moves[i].m) || is_out_of_bounds(previous_moves[i].m))
	    return 0;
	grid[move_to_grid_cell(previous_moves[i].m)] = previous_moves[i].c;
	return 1;
    }
}

struct move_t play(struct col_move_t const previous_moves[], size_t n_moves) {
    assert(update_grid(previous_moves, n_moves), "the server returned incorrect moves");
    struct move_t next_move;
    do {
	next_move.row = rand()%grid_size;
	next_move.col = rand()%grid_size;
    }  while(was_already_played(next_move));
    return next_move;
}

void finalize() {
    free(grid);
}
