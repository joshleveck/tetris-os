#include "drawing.h"
#include "colour.h"
#include "tetro.h"
#include "structs.h"

int locations[7][4][2] = {
    {{-2, 0}, {-1, 0}, {0, 0}, {1, 0}},
    {{-1, -1}, {0, -1}, {0, 0}, {1, 0}},
    {{-1, 0}, {0, 0}, {1, 0}, {1, -1}},
    {{0, 0}, {1, 0}, {1, -1}, {0, -1}},
    {{-1, 0}, {0, 0}, {0, -1}, {1, -1}},
    {{-1, 0}, {0, 0}, {1, 0}, {0, -1}},
    {{-1, -1}, {0, -1}, {0, 0}, {1, 0}}};

enum Tetro
{
    I = 0,
    J = 1,
    L = 2,
    O = 3,
    S = 4,
    T = 5,
    Z = 6
};

State state = {0, 0, 0, 0};

State *create_state(int x, int y, int rotation, int tetro) {
    state.x = x;
    state.y = y;
    state.rotation = rotation;
    state.tetro = tetro;
    return &state;
}

unsigned int tetro_to_colour(enum Tetro tetro) {
    switch (tetro) {
        case I:
            return TEAL;
        case J:
            return BLUE;
        case L:
            return ORANGE;
        case O:
            return YELLOW;
        case S:
            return GREEN;
        case T:
            return PURPLE;
        case Z:
            return RED;  
    }
    return 0x000000;
}

void tick(State *state, Board *board)
{
}

void advance_piece(State *state, Board *board)
{
}

void draw_piece(State *state) 
{
    for (int i = 0; i < 4; i++) {
        draw_on_area(locations[state->tetro][i][0] + state->x, locations[state->tetro][i][1] + state->y, tetro_to_colour(state->tetro));
    }
}

void delete_piece(State *state) 
{
    for (int i = 0; i < 4; i++) {
        delete_on_area(locations[state->tetro][i][0] + state->x, locations[state->tetro][i][1] + state->y);
    }
}