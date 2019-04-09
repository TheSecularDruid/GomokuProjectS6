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
  return 0;
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

  dlclose(handle_player);
  return result;


}

int test_compute_next_player()
{
  /*struct player previous_player ;
  struct player player1 ;
  struct player player2;
  previous_player = compute_next_player(previous_player,player1,player2);*/
  return 0;
}

int test_compute_next_board()
{
  return 0;
}

int test_update_last_moves()
{
  return 0;
}

int test_get_move_number()
{
  return 0;
}

int test_move_to_col_move()
{

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


  return 0;
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
  test_case("Test convert move to col move",test_move_to_col_move(),&passed,&total);
  test_case("Test enqueue move",test_enqueue(),&passed,&total);
  printf("%sTESTS PASSED: %d/%d\n",((passed-total)==0? KGRN : KRED),passed,total);
  printf("%s", KWHT);
  return 0;
}
