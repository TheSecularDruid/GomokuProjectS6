#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "bitboard_move.h"
#include "bitboard_player.h"

//This player tries his hardest to create a vertical line starting from (0,0), thus being an effective counter to random players

char* grid = NULL;
size_t grid_size = 0;
int color = 0;

char const* get_player_name() {
    return("Cousin of Kevin");
}

struct col_move_t* propose_opening(size_t size) {
    struct col_move_t* opening = malloc(sizeof(struct col_move_t[3]));
    opening[0].m.col=0;
    opening[0].m.row=0;
    opening[0].c=BLACK;
    opening[1].m.col=size-1;
    opening[1].m.row=0;
    opening[1].c=WHITE;
    opening[2].m.col=0;
    opening[2].m.row=1;
    opening[2].c=BLACK;
    return opening; //WARNING : this memory would be freed by the server
}

int accept_opening(size_t size, const struct col_move_t* opening) {
    (void) size;
    (void) opening;
    return 1;
}

void initialize(size_t size, enum color_t id) {
    grid = malloc(sizeof(char[size*size]));
    for (unsigned int k=0;k<size*size;k++) {
	grid[k]=-1;
    }
    grid_size = size;
    color  = id;
}

unsigned int move_to_grid_cell(struct move_t move) {
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
    for (unsigned int i=0;i<n_moves;i++) {
	if(was_already_played(previous_moves[i].m) || is_out_of_bounds(previous_moves[i].m))
	    return 0;
	grid[move_to_grid_cell(previous_moves[i].m)] = previous_moves[i].c;
    }
    return 1;
}

struct move_t play(struct col_move_t const previous_moves[], size_t n_moves) {
    assert(update_grid(previous_moves, n_moves));
    struct move_t move = {0,0};
    while(move.col<grid_size) {
	if (move.row>grid_size) {
	    move.row = 0;
	    move.col++;
	}
	if (grid[move_to_grid_cell(move)]==color)
	    move.row++;
	else if (grid[move_to_grid_cell(move)]!=color && grid[move_to_grid_cell(move)]!=-1) {
	    if (grid_size - move.row>5)
		move.row++;
	    else {
		move.row=0;
		move.col++;
	    }
	}
	else return(move);
    }
    for(move.row=0;move.row<grid_size;move.row++) {
	for(move.col=0;move.col<grid_size;move.col++){
	    if (grid[move_to_grid_cell(move)]==-1)
		return(move);
	}
    }
    struct move_t error = {-1,-1};
    return error;
}

void finalize() {
    free(grid);
}
