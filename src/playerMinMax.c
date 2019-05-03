#include "bitboard.h"
#include "bitboard_player.h"
#include "bitboard_move.h"

#ifndef DEPTH
#define DEPTH 4
#endif
#define THRESH 5

struct bitboard board;
enum color_t identity;
size_t grid_size;

char const* get_player_name() {
    return "MiniMaxime";
}

struct col_move_t* propose_opening(size_t size) {
    struct col_move_t* move = malloc(sizeof(struct col_move_t)*3);
    move[0].c=BLACK;
    move[0].m.row = size/2;
    move[0].m.col = size/2;
    move[1].c=WHITE;
    move[1].m.row = size/2-1;
    move[1].m.col = size/2;
    move[2].c=BLACK;
    move[2].m.row = size/2-2;
    move[2].m.col = size/2 + 2;
    return move;   //apparently this is an equal opening, so that the opponent cannot take advantage of the proposed opening
}

//int accept_opening(size_t size, const struct col_move_t* opening);

void initialize(size_t size, enum color_t id) {
    board = new_bitboard();
    identity = id;
    grid_size = size;
}

void update_board(struct col_move_t const previous_moves[], size_t n_moves) {
    for(unsigned int i=0;i<n_moves;i++)
	play_move(&previous_moves[i],&board,grid_size);
}

int weigh(const struct bitboard* cur_pos) {
    enum color_t ennemy;
    if (identity==BLACK)
	ennemy=WHITE;
    else
	ennemy=BLACK;
    if (color_is_winning(*cur_pos, identity, grid_size, THRESH))
	return 10000;
    if (color_is_winning(*cur_pos,ennemy,grid_size,THRESH))
	return -10000;
    return (max_line_length(*cur_pos,identity,grid_size)-max_line_length(*cur_pos,ennemy,grid_size));
}

int minimax(struct bitboard cur_pos,int cur_depth, enum color_t cur_color) {
    int move_value;
    enum color_t other_color;
    if (cur_color==WHITE)
	other_color=BLACK;
    else
	other_color=WHITE;

    if (cur_depth ==0)
	return weigh(play_move(&move,&board,grid_size));
    int length = nb_of_poss(cur_board);    //we only consider adjacent move for complexity purposes
    struct bitboard poss[length];
    possible_boards(poss,cur_pos);
    if (cur_color==identity) {
	move_value = -1000000;
	for(size_t i=0;i<length;i++)
	    move_value = max(move_value, minimax(poss[i],cur_depth-1,other_color));
    }
    else {
	move_value = 1000000;
	for(size_t i=0;i<length;i++)
	    move_value = min(move_value,minimax(poss[i],cur_depth-1,other_color));
    }
    return move_value;
}

struct move_t play(struct col_move_t const previous_moves[], size_t n_moves) {
    board = update_board(previous_moves,n_moves);
    struct move_t[size*size] poss = possible_moves();
    int[size*size] weigh;
    for(size_t i=0;i<length;i++)
	weigh[i] = minimax(move[i]);
    int k = indice_max(weigh,length);
    return poss[k];
}

void finalize() { 
}
//*******sources*********//
/*
https://stackoverflow.com/questions/5469914/a-good-minimax-representation-in-gomoku

https://openclassrooms.com/fr/courses/1202501-lalgorithme-min-max

https://blog.theofekfoundation.org/artificial-intelligence/2015/12/11/minimax-for-gomoku-connect-five/
*/
