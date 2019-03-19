#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <getopt.h>
#include <string.h>
#include <bitboard_move.h>

// Different kind of mode
enum mode{ SWAP, STANDARD};

// Global grid size by default
static size_t grid_size = 5;

// Global mode by default
static int mode = STANDARD;

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
struct player * compute_next_player(struct player * previous_player,struct player * player1, struct player * player2)
{
  struct player * current_player = NULL;
  switch (previous_player->id) {
    case 0:
      return player1;
      break;
    case 1:
      return player2;
      break;
    default:
    fprintf(stderr,"Error in function next_player\n");
    exit(EXIT_FAILURE);
  }

  return current_player;
}

/**********************************
* Function for displaying the player turn
************************************/
void display_player_turn(struct player * player)
{
  printf("%s is playing: \n",player->get_name() );
}

/**********************************
* Function for parsing the options of the program
* Currently available options are :
* -n <size> : sets the size of the grid
* -o : sets the SWAP mode
************************************/
void parse_opts(int argc, char* argv[]) {
  int opt;
  while ((opt = getopt(argc, argv, "n:o")) != -1) {
    switch (opt) {
      case 'n':
      grid_size = atoi(optarg);
      break;
      case 'o':
      printf("LA PARTIE COMMENCE EN MODE SWAP\n" );
      mode = SWAP;
      break;
      default:
      fprintf(stderr,"Usage ./install/server -n [N] -o ./install/player1.so ./install/player2.so\n");
      exit(EXIT_FAILURE);
    }
  }
}


int main(int argc, char *argv[]) {
  // If we forgot the filenames in arguments
  if ( argc < 2 || argc > 6){
    printf("Usage ./install/server -n [N] -o ./install/player1.so ./install/player2.so\n");
  }
  else{
    // parse the differents arguments
    parse_opts(argc, argv);

    // to avoid a grid with a bad size
    while (grid_size < 5 || grid_size > 11)
    {
      printf("Entrez une taille doit être comprise entre 5 et 11: ");
      scanf("%zu", &grid_size);
    }

    // pointers to the lib name
    char * lib_player1;
    char * lib_player2;

    // pointers to the lib opening
    void * handle_player1;
    void * handle_player2;

    // array of three moves for the opening in SWAP mode
    const struct col_move_t* moves;

    // get the lib player names
    lib_player1 = argv[optind];
    lib_player2 = argv[optind+1];

    // some debug prints for the parameters
    /*
    printf ("optind: %d\n", optind);
    printf ("Taille du plateau: %d\n", grid_size);
    printf ("Mode du jeu: %d\n", mode);
    printf ("%s\n", lib_player1);
    printf ("%s\n", lib_player2);*/

    // number max laps before full grid
    int laps = 0;
    int max_laps = (grid_size * grid_size);

    // loading both libs
    handle_player1 = dlopen(lib_player1, RTLD_NOW);
    if (!handle_player1) {
      printf("Error in 'dlopen' for player 1\n");
      exit(1);
    }
    handle_player2 = dlopen(lib_player2, RTLD_NOW);
    if (!handle_player2) {
      printf("Error in 'dlopen' for player 2\n");
      exit(1);
    }

    // initializing of the different player of the game
    struct player * first_player = initialize_player(handle_player1,0);
    struct player * second_player = initialize_player(handle_player2,1);

    // Swap mode
    if (mode == SWAP)
    {
      printf("GAME STARTS IN SWAP MODE\n");
      // beginning of the game
      moves = first_player->propose_opening(grid_size);

      // 2nd player plays next
      if (second_player->accept_opening(grid_size, moves))
      {
        first_player->initialize(grid_size, BLACK);
        second_player->initialize(grid_size, WHITE);
      }
      // 2nd player refuses and 1st player plays next
      else
      {
        first_player->initialize(grid_size, WHITE);
        second_player->initialize(grid_size, BLACK);
      }

      // update max_laps
      max_laps = max_laps - 3;

    }
    // Standard mdoe
    else
    {
      printf("GAME STARTS IN STANDARD MODE\n");
      printf("%s\n",first_player->get_name());
      first_player->initialize(grid_size, BLACK);
      second_player->initialize(grid_size, WHITE);
    }

    // declareing current player
    struct player * current_player = NULL;

    while (laps < max_laps)
    {
      current_player = compute_next_player(first_player,first_player,second_player);
      display_player_turn(current_player);
      /*m = p->play(last_n(moves));
      if (is_winning())
      {
        break;
      }
      enqueue(m, moves);*/
      laps++;
    }

    // finally clear allocated memory
    first_player->finalize();
    second_player->finalize();


    dlclose(handle_player1);
    dlclose(handle_player2);
  }
}
