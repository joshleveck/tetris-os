#include "structs.h"

#ifndef TETRO_H
#define TETRO_H

State *create_state(int x, int y, int rotation, int tetro);
void draw_piece(State *state);
void delete_piece(State *state);
int legal_state(State *state);
int update_board(State *state);

#endif