#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <getopt.h>
#include <string.h>
#include <bitboard_move.h>

// Different kind of mode
enum mode{ SWAP, STANDARD};

// Global grid size by default
static int grid_size = 5;

// Global mode by default
static int mode = STANDARD;


/**********************************
* Function for parsing the options of the program
* Currently available options are :
* -s <seed> : sets the seed
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
      scanf("%d", &grid_size);
    }

    // declaring all the different variable
    char * lib_player1;
    char * lib_player2;
    void * player1;
    void * player2;
    char * (*get_name_player1)(void);
    char * (*get_name_player2)(void);
    char * error;

    // parameters
    lib_player1 = argv[optind];
    lib_player2 = argv[optind+1];

    // some debug prints
    /*
    printf ("optind: %d\n", optind);
    printf ("Taille du plateau: %d\n", grid_size);
    printf ("Mode du jeu: %d\n", mode);
    printf ("%s\n", lib_player1);
    printf ("%s\n", lib_player2);*/

    // loading both libs
    player1 = dlopen(lib_player1, RTLD_NOW);
    if (!player1) {
      printf("Error in player1 loading\n");
      exit(1);
    }
    player2 = dlopen(lib_player2, RTLD_NOW);
    if (!player2) {
      printf("Error in player2 loading\n");
      exit(1);
    }


    get_name_player1 = dlsym(player1, "get_name");
    if ((error = dlerror()) != NULL)  {
      printf("Error in function loading\n");
      exit(1);
    }
    get_name_player2 = dlsym(player2, "get_name");
    if ((error = dlerror()) != NULL)  {
      printf("Error in function loading\n");
      exit(1);
    }

    // number max laps before full grid
    int laps = 0;
    int max_laps = (grid_size * grid_size) - 3;

/*
    // Swap mode
    if (mode == SWAP)
    {
      // beginning of the game
      moves = first_player->propose_opening(size);
      // 2nd player plays next
      if second_player->accept_opening(size, moves)
      {
        first_player->initialize(size, BLACK);
        second_player->initialize(size, WHITE);
      }
      // 2nd player refuses and 1st player plays next
      else
      {
        first_player->initialize(size, WHITE);
        second_player->initialize(size, BLACK);
      }
    }
    // Standard mdoe
    else
    {
      first_player->initialize(size, BLACK);
      second_player->initialize(size, WHITE);
    }

    while (laps < max_laps)
    {
      p = compute_next_player();
      m = p->play(last_n(moves));
      if (is_winning())
      {
        break;
      }
      enqueue(m, moves);
      laps++;
    }
    // finally clear allocated memory
    first_player->finalize();
    second_player->finalize();*/

    printf ("%s\n", (*get_name_player1)());
    printf ("%s\n", (*get_name_player2)());
    dlclose(player1);
    dlclose(player2);
  }
}
