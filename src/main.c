#include "server.h"

// Global mode by default
int mode = STANDARD;

// Global number of pawns in a row to win
size_t winning_threshold = 5;

// Global grid size by default
size_t grid_size = 5;

/**********************************
* Function for parsing the options of the program
* Currently available options are :
* -n <size> : sets the size of the grid
* -o : sets the SWAP mode2
************************************/
void parse_opts(int argc, char* argv[]) {
  int opt;
  while ((opt = getopt(argc, argv, "n:o")) != -1) {
    switch (opt) {
      case 'n':
      grid_size = atoi(optarg);
      break;
      case 'o':
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
      printf("The size of the grid must be between 5 and 11: ");
      scanf("%zu", &grid_size);
    }

    printf("%zu\n",grid_size );

    // pointers to the lib name
    char * lib_player1;
    char * lib_player2;

    // pointers to the lib opening
    void * handle_player1;
    void * handle_player2;

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
    struct player * first_player = initialize_player(handle_player1,1);
    struct player * second_player = initialize_player(handle_player2,2);

    // declaring current player
    struct player * current_player = NULL;

    // array of three moves for the opening in SWAP mode
    struct col_move_t * moves;
    size_t size_moves = 0;

    // declaring the current move played
    struct move_t current_move;

    // declaring the current move played
    struct col_move_t * current_col_move = malloc(sizeof(struct col_move_t));

    // declaring array of previous moves
    struct col_move_t * previous_moves = malloc(sizeof(struct col_move_t[4]));

    // declaring new bitboard
    struct bitboard board = new_bitboard();

    // Swap mode
    if (mode == SWAP)
    {
      printf("GAME STARTS IN SWAP MODE\n------------ OPPENING -------------\n");
      // beginning of the game
      moves = first_player->propose_opening(grid_size);

      // update the number of elements of the array moves
      size_moves = size_moves + 3;

      // update max_laps
      max_laps = max_laps - 3;

      printf("%s proposes: ",first_player->get_name());
      display_moves(moves,size_moves);
      moves = realloc(moves,sizeof(struct col_move_t[grid_size*grid_size]));

      // 2nd player plays next
      if (second_player->accept_opening(grid_size, moves))
      {
        printf("%s accepts those moves ...\n",second_player->get_name());
        first_player->initialize(grid_size, BLACK);
        first_player->id = BLACK;
        second_player->initialize(grid_size, WHITE);
        second_player->id = WHITE;
        current_player = first_player;
      }
      // 2nd player refuses and 1st player plays next
      else
      {
        printf("%s refuses those moves ...\n",second_player->get_name());
        first_player->initialize(grid_size, WHITE);
        first_player->id = WHITE;
        second_player->initialize(grid_size, BLACK);
        second_player->id = BLACK;
        current_player = second_player;
      }

    }
    // Standard mdoe
    else
    {
      printf("GAME STARTS IN STANDARD MODE\n");
      first_player->initialize(grid_size, BLACK);
      first_player->id = BLACK;
      second_player->initialize(grid_size, WHITE);
      second_player->id = WHITE;
      current_player = second_player;
      moves = malloc(sizeof(struct col_move_t[grid_size*grid_size]));;
    }


    printf("-------------- MOVES ----------------\n");
    while (laps < max_laps)
    {
      current_player = compute_next_player(current_player,first_player,second_player);
      update_last_moves(previous_moves,moves,size_moves);
      current_move = current_player->play(previous_moves, get_move_number(size_moves));
      display_player_move(current_player,current_move);
      play_move(move_to_col_move(current_player,current_move,current_col_move),&board,grid_size);
      enqueue(current_player,current_move, moves, size_moves);
      size_moves ++;
      if (color_is_winning(board,current_player->id,grid_size,winning_threshold))
      {
        break;
      }
      printf("%d\n",laps);
      laps++;

    }

    printf("-------------- STATE OF THE BOARD ----------------\n");
    display_moves(moves,size_moves);
    show_grid(moves,size_moves);

    printf("-------------- RESULTS ----------------\n");
    if ( laps == max_laps)
    {
      printf("Draw\n");
    }
    else
    {
      printf("%s wins!\n",current_player->get_name());
    }

    // finally clear allocated memory
    free(current_col_move);
    free(previous_moves);
    free(moves);
    first_player->finalize();
    second_player->finalize();
    free(first_player);
    free(second_player);

    // close the libs
    dlclose(handle_player1);
    dlclose(handle_player2);
  }
}
