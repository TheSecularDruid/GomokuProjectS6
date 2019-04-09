#include "server.h"

/**********************************
* Function for displaying error in loading
************************************/
void * get_symbol(void * handle, const char * symbol, int id)
{
  void* res = dlsym(handle, symbol);
  if (res  == NULL)  {
    printf("Impossible to load '%s' of player %d!\n",symbol,id);
    exit(1);
  }
  return res;
}

/**********************************
* Function for initialising the struct player
************************************/
struct player * initialize_player(void * handle_player, int id)
{
  struct player * new_player = malloc(sizeof(struct player));
  new_player->id = id;
  new_player->get_name = get_symbol(handle_player, "get_player_name", id);
  new_player->propose_opening = get_symbol(handle_player, "propose_opening", id);
  new_player->accept_opening = get_symbol(handle_player, "accept_opening", id);
  new_player->initialize = get_symbol(handle_player, "initialize", id);
  new_player->play = get_symbol(handle_player, "play", id);
  new_player->finalize = get_symbol(handle_player, "finalize", id);
  return new_player;
}

/**********************************
* Function for switching the player turn
************************************/
struct player * compute_next_player(int id,struct player * player1, struct player * player2)
{
  struct player * current_player = NULL;
  switch (id) {
    case 0:
    return player2;
    break;
    case 1:
    return player1;
    break;
    default:
    fprintf(stderr,"Error in function next_player\n");
    exit(EXIT_FAILURE);
  }

  return current_player;
}

/**********************************
* Function for switching the board color
************************************/
__uint128_t compute_next_board(int id, struct bitboard * board)
{
  __uint128_t current_board = 0;
  switch (id) {
    case BLACK:
    return board->black;
    break;
    case WHITE:
    return board->white;
    break;
    default:
    fprintf(stderr,"Error in function compute_next_board\n");
    exit(EXIT_FAILURE);
  }

  return current_board;
}

/**********************************
* Function for displaying the player turn
************************************/
void display_player_move(const char * name, struct move_t current_move)
{
  printf("%s plays: (%zu,%zu) \n",name, current_move.row, current_move.col );
}

/**********************************
* Function for returning the last moves played
************************************/
void update_last_moves(struct col_move_t previous_moves[], struct col_move_t * moves, size_t size_moves)
{
  int openning_swap = (mode == SWAP && (size_moves == 0 || size_moves == 1 || size_moves == 3 || size_moves == 4));
  int openning_standard = (mode == STANDARD && (size_moves == 0 || size_moves == 1));

  if (  openning_swap || openning_standard )
  {
    for (size_t i=0; i<size_moves; i++)
    {
      previous_moves[i] = moves[i];
    }
  }
  else
  {
    previous_moves[0] = moves[size_moves-2];
    previous_moves[1] = moves[size_moves-1];
  }
}

/**********************************
* Function for returning the number of moves played
************************************/
size_t get_move_number(size_t size_moves, int mode)
{
  int size;
  int openning_swap = (mode == SWAP && (size_moves == 0 || size_moves == 1 || size_moves == 3 || size_moves == 4));
  int openning_standard = (mode == STANDARD && (size_moves == 0 || size_moves == 1));
  if (  openning_swap || openning_standard )
  {
    size = size_moves;
  }
  else
  {
    size = 2;
  }

  return size;
}

/**********************************
* Function converting move_t into col_move_t
************************************/
struct col_move_t * move_to_col_move(int id, struct move_t move, struct col_move_t * col_move)
{
  col_move->m.row = move.row;
  col_move->m.col = move.col;
  col_move->c = id;
  return col_move;
}

/**********************************
* Function for saving all the moves
************************************/
void enqueue(int id, struct move_t current_move, struct col_move_t moves[], size_t size_moves)
{
  // we add the current move at the end
  if (size_moves < grid_size *grid_size)
  {
    moves[size_moves].m = current_move;
    moves[size_moves].c = id;
  }
  else
  {
    printf("Max number of element on the grid reached");
  }
}


/**********************************
* Function for displaying all the moves
************************************/
void display_moves(struct col_move_t moves[],size_t size_moves)
{
  printf("[");
  for(unsigned int i=0; i < size_moves; i++)
  {
    printf("([%zu,%zu],%d) ",moves[i].m.row,moves[i].m.col,moves[i].c );
  }
  printf("]\n");
}

/**********************************
* Function for showing the grid
************************************/
void show_grid(struct col_move_t moves[], size_t size_moves)
{
  int grid[grid_size][grid_size];
  for (size_t x=0; x<grid_size; x++)
  {
    for (size_t y=0; y<grid_size; y++)
    {
      grid[x][y] = 5;
    }
  }
  for(unsigned int i=0; i <  size_moves; i++)
  {
    grid[moves[i].m.row][moves[i].m.col] = moves[i].c;
  }
  printf("-------------------\n");
  for (size_t x=0; x<grid_size; x++)
  {
    printf("\n");
    for (size_t y=0; y<grid_size; y++)
    {
      printf("%d ",grid[x][y] );
    }
  }
  printf("\n-------------------\n");
}
