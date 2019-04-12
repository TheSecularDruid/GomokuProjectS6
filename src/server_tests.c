#include "server.h"

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
  handle_player = dlopen("./install/player1.so", RTLD_NOW);
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
  handle_player = dlopen("./install/player1.so", RTLD_NOW);
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
  /*struct player previous_player ;
  struct player player1 ;
  struct player player2;
  previous_player = compute_next_player(previous_player,player1,player2);

  void * handle_player1;
  void * handle_player2;

  // loading both libs
  handle_player1 = dlopen("./install/player1.so", RTLD_NOW);
  if (!handle_player1) {
    printf("Error in 'dlopen' for player 1\n");
    exit(1);
  }
  handle_player2 = dlopen("./install/player2.so", RTLD_NOW);
  if (!handle_player2) {
    printf("Error in 'dlopen' for player 2\n");
    exit(1);
  }
*/

  return 0;
}

int test_compute_next_board()
{

  return 0;
}

int test_update_last_moves()
{
  struct col_move_t * moves = malloc(sizeof(struct col_move_t[grid_size*grid_size]));;
  struct col_move_t * previous_moves = malloc(sizeof(struct col_move_t[4]));
  //struct col_move_t move_one = {{1,1},1};
  //struct col_move_t move_two = {{2,2},2};
  //struct col_move_t move_three = {{3,3},3};
  // moves[0] = move_one;
  //moves[1] = move_two;
  //moves[3] = move_three;

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
  return 0;
}

int main()
{
  int passed = 0;
  int total = 0;
  printf("============= TEST SERVER FUNCTIONS ===============\n");
  // /test_case("Add a negative int",test_add_negative(),&passed,&total);
  test_case("Test get symbol",test_get_symbol(),&passed,&total);
  test_case("Test initialize player",test_initialize_player(),&passed,&total);
  test_case("Test compute next player",test_compute_next_player(),&passed,&total);
  test_case("Test compute next board",test_compute_next_board(),&passed,&total);
  test_case("Test update last moves",test_update_last_moves(),&passed,&total);
  test_case("Test get move number",test_get_move_number(),&passed,&total);
  test_case("Test convert move to col move",test_move_to_col_move(),&passed,&total);
  test_case("Test enqueue move",test_enqueue(),&passed,&total);
  printf("%sTESTS PASSED: %d/%d\n",((passed-total)==0? KGRN : KRED),passed,total);
  printf("%s", KWHT);
  return 0;
}
