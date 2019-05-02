#include <time.h>

#include "server.h"
#include "player_test.h"
#include "bitboard_test.h"

#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KWHT  "\x1B[37m"

void test_case(char msg[],int result,int * passed,int * total)
{
  *total = *total + 1;
  if (result != 0)
  {
    *passed = *passed + 1;
  }
  printf("%s....%s\n",msg,(result==0? "\x1B[31mFAILED\x1B[37m" : "\x1B[32mPASSED\x1B[37m"));
}

int test_get_symbol()
{
  void * handle_player;

  // loading both libs
  handle_player = dlopen("./install/playerKevin.so", RTLD_NOW);
  if (!handle_player) {
    printf("Error in 'dlopen' for player 1\n");
    return 0;
  }
  int id = 1;
  int * result = get_symbol(handle_player, "get_player_name", id);


  dlclose(handle_player);
  return (result != NULL);
}

int test_initialize_player()
{
  void * handle_player;

  // loading both libs
  handle_player = dlopen("./install/playerKevin.so", RTLD_NOW);
  if (!handle_player) {
    printf("Error in 'dlopen' for player 1\n");
    exit(1);
  }
  int id = 1;

  // initializing of the different player of the game
  struct player * player = initialize_player(handle_player,id);

  int result = (player->id == 1
    && player->get_name == get_symbol(handle_player, "get_player_name", id)
    && player->propose_opening == get_symbol(handle_player, "propose_opening", id)
    && player->accept_opening == get_symbol(handle_player, "accept_opening", id)
    && player->initialize == get_symbol(handle_player, "initialize", id)
    && player->play == get_symbol(handle_player, "play", id)
    && player->finalize == get_symbol(handle_player, "finalize", id));
  player->finalize();
  free(player);
  dlclose(handle_player);
  return result;
}

int test_compute_next_player()
{

  void * handle_player1;
  void * handle_player2;

  // loading both libs
  handle_player1 = dlopen("./install/playerKevin.so", RTLD_NOW);
  if (!handle_player1) {
    printf("Error in 'dlopen' for player 1\n");
    exit(1);
  }
  handle_player2 = dlopen("./install/playerRandom.so", RTLD_NOW);
  if (!handle_player2) {
    printf("Error in 'dlopen' for player 2\n");
    exit(1);
  }

  struct player * next_player;
  struct player * player1 = initialize_player(handle_player1,1);
  struct player * player2 = initialize_player(handle_player2,2);
  next_player = compute_next_player(player1->id,player1,player2);

  int result = (next_player->id == player2->id);

  player1->finalize();
  player2->finalize();
  free(player1);
  free(player2);
  dlclose(handle_player1);
  dlclose(handle_player2);
  return result;
}


int test_update_last_moves()
{
  struct col_move_t * moves = malloc(sizeof(struct col_move_t[grid_size*grid_size]));;
  struct col_move_t * previous_moves = malloc(sizeof(struct col_move_t[4]));

  update_last_moves(previous_moves,moves,3);

  int first = (previous_moves[0].m.row == moves[0].m.row
    && previous_moves[0].m.col == moves[0].m.col && previous_moves[0].c== moves[0].c);

  int second = (previous_moves[1].m.row == moves[1].m.row
    && previous_moves[1].m.col == moves[1].m.col && previous_moves[1].c== moves[1].c);

  int third = (previous_moves[2].m.row == moves[2].m.row
    && previous_moves[2].m.col == moves[2].m.col && previous_moves[2].c== moves[2].c);


  free(previous_moves);
  free(moves);
  return (first && second && third);
}

int test_get_move_number()
{
  size_t first = 4;
  size_t second = 1;
  return ( get_move_number(first,STANDARD) == 2 && get_move_number(second,STANDARD) == second
        && get_move_number(first,SWAP) == first && get_move_number(second,SWAP) == second);
}

int test_move_to_col_move()
{

  struct move_t move = {2,2};
  struct col_move_t * col_move = malloc(sizeof(struct col_move_t));

  move_to_col_move(BLACK,move,col_move);

  int result = (col_move->m.row == 2 && col_move->m.col == 2 && col_move->c == BLACK);
  free(col_move);
  return result;
}

int test_enqueue()
{
  struct col_move_t * moves = malloc(sizeof(struct col_move_t[25]));
  struct move_t * move1 = malloc (sizeof(struct move_t));
  move1->row = 0;
  move1->row = 0;
  struct move_t * move2 = malloc (sizeof(struct move_t));
  move2->row = 1;
  move2->row = 1;
  moves[0].m = *move1;
  moves[0].c = 1;
  moves[1].m = *move2;
  moves[1].c = 2;
  struct move_t * new_move = malloc (sizeof(struct move_t));
  move2->row = 2;
  move2->row = 2;
  size_t id_player = 1;
  int number_of_element = 2;
  size_t size_max = 25;
  enqueue(id_player,*new_move,moves,number_of_element,size_max);
  int result = (moves[number_of_element].m.row == new_move->row && moves[number_of_element].m.col == new_move->col
    && moves[number_of_element].c == id_player);
  free(move1);
  free(move2);
  free(new_move);
  free(moves);
  return result;
}

int main()
{
  srand(time(NULL));
  int passed = 0;
  int total = 0;
  printf("============= TESTING SERVER FUNCTIONS ===============\n");
  test_case("Test get symbol",test_get_symbol(),&passed,&total);
  test_case("Test initialize player",test_initialize_player(),&passed,&total);
  test_case("Test compute next player",test_compute_next_player(),&passed,&total);
  test_case("Test update last moves",test_update_last_moves(),&passed,&total);
  test_case("Test get move number",test_get_move_number(),&passed,&total);
  test_case("Test convert move to col move",test_move_to_col_move(),&passed,&total);
  test_case("Test enqueue move",test_enqueue(),&passed,&total);
  printf("%sTESTS PASSED: %d/%d\n",((passed-total)==0? KGRN : KRED),passed,total);
  printf("%s", KWHT);

  printf("============= TESTING PLAYER FUNCTIONS ===============\n");
  int board_size = rand()%7+5;
  struct col_move_t moves[5]= {{ {5,4}, WHITE}, { {4,4}, BLACK}, { {5,5}, WHITE}, { {4,3} ,BLACK}, { {3,3}, WHITE}};
  int n_moves=2;
  struct col_move_t const previous_moves[2]={{ {4,3} ,BLACK}, { {3,3}, WHITE}};
  initialize(board_size, WHITE);
  test_case("Test get player name",test_get_player_name(),&passed,&total);
  test_case("Test propose opening",test_propose_opening(board_size),&passed,&total);
  test_case("Test play move",test_play(moves, board_size, previous_moves, n_moves),&passed,&total);
  printf("%sTESTS PASSED: %d/%d\n",((passed-total)==0? KGRN : KRED),passed,total);
  printf("%s", KWHT);

  printf("============= TESTING BITBOARD FUNCTIONS ===============\n");

  test_case("Test creation of a bitboard",test_new_bitboard(),&passed,&total);
  test_case("Test updating bitboard after a move",test_play_move(),&passed,&total);
  test_case("Test if a player won",test_color_is_winning(),&passed,&total);
  printf("%sTESTS PASSED: %d/%d\n",((passed-total)==0? KGRN : KRED),passed,total);
  printf("%s", KWHT);

  return 0;
}
