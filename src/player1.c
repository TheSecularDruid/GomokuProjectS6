#include <stdio.h>
#include <stdlib.h>

#include "bitboard_move.h"
#include "bitboard_player.h"

//This player just plays randomly something that they can play - pretty much a test in itself

char* grid = NULL;
size_t grid_size = 0;
int color = 0;

char const* get_player_name() {
  return("Random 1");
}

int is_already_played(size_t a, size_t b, struct col_move_t const moves[], int size) {
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
    } while (is_already_played(abscissa,ordinate,opening,i));
    opening[i].m.row = abscissa;
    opening[i].m.col = ordinate;
    if (i%2)   //if i is odd
    opening[i].c = WHITE;
    else
    opening[i].c = BLACK;
  }
  return opening; //WARNING : this memory would be freed by the server in
                  //the current implementation - should be further thought out
}

int accept_opening(size_t size, const struct col_move_t* opening) {
  return 1;
}

void initialize(size_t size, enum color_t id) {
  grid = malloc(sizeof(char[size*size]));
  for (unsigned int k=0;k<size;k++) {
    grid[k]=-1;
  }
  grid_size = size;
  color  = id;
}

struct move_t play(struct col_move_t const previous_moves[], size_t n_moves) {
  int abscissa = -1;
  int ordinate = -1;
  do {
    abscissa = rand()%grid_size;
    ordinate = rand()%grid_size;
  }  while(is_already_played(abscissa, ordinate, previous_moves, n_moves));
  struct move_t next_move = {abscissa,ordinate};
  return next_move;
}

void finalize() {
  free(grid);
}
