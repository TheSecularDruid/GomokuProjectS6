#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <getopt.h>
#include <string.h>

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
  if ( argc < 2 || argc > 6)
  {
    printf("Usage ./install/server -n [N] -o ./install/player1.so ./install/player2.so\n");
  }
  else
  {
    // parse the differents arguments
    parse_opts(argc, argv);

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
    /*
    printf ("optind: %d\n", optind);
    printf ("Taille du plateau: %d\n", grid_size);
    printf ("Mode du jeu: %d\n", mode);
    printf ("%s\n", lib_player1);
    printf ("%s\n", lib_player2);*/

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

    printf ("%s\n", (*get_name_player1)());
    printf ("%s\n", (*get_name_player2)());
    dlclose(player1);
    dlclose(player2);
  }
}
