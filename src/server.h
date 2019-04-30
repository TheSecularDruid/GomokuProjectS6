#ifndef SERVER_H
#define SERVER_H

#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <getopt.h>
#include <string.h>
#include "bitboard_move.h"
#include "bitboard.h"

// Different kind of mode
enum mode{ SWAP, STANDARD};

int mode;

// Global number of pawns in a row to win
size_t winning_threshold;

// Global grid size by default
size_t grid_size;

// Defining a struct Player
struct player {
  int id;
  char const * (*get_name)(void);
  struct col_move_t * (*propose_opening)(size_t);
  int (*accept_opening)(size_t, const struct col_move_t*);
  void (*initialize)(size_t, enum color_t);
  struct move_t (*play)(struct col_move_t const [],size_t);
  void (*finalize)(void);
};

void * get_symbol(void * handle, const char * symbol, int id);
struct player * initialize_player(void * handle_player, int id);
struct player * compute_next_player(int id, struct player * player1, struct player * player2);
__uint128_t compute_next_board(int id, struct bitboard * board);
void display_player_move(const char * name, struct move_t current_move);
void update_last_moves(struct col_move_t previous_moves[], struct col_move_t * moves, size_t size_moves);
size_t get_move_number(size_t size_moves, int mode);
struct col_move_t * move_to_col_move(int id, struct move_t move, struct col_move_t * col_move);
void enqueue(int id, struct move_t current_move, struct col_move_t moves[], size_t size_moves, size_t size_max);
void display_moves(struct col_move_t moves[],size_t size_moves);
void show_grid(struct col_move_t moves[], size_t size_moves);

#endif
