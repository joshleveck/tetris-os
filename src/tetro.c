#include "drawing.h"
#include "colour.h"
#include "tetro.h"
#include "structs.h"
#include "uart.h"

int locations[7][4][2] = {
    {{0, -1}, {0, 0}, {0, 1}, {0, 2}},
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
int board_data[10][20] = {0};
Board board = {0, 0, &board_data};

unsigned int tetro_to_colour(enum Tetro tetro)
{
    switch (tetro)
    {
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

State *create_state(int x, int y, int rotation, int tetro)
{
    state.x = x;
    state.y = y;
    state.rotation = rotation;
    state.tetro = tetro;
    return &state;
}

Board *get_board()
{
    return &board;
}

long int_to_long(int i, int j)
{
    return (long)i << 32 | (j & 0xFFFFFFFF);
}

// Were going to do something like this
// We'll store a "tuple" in a long
// (x,y)
int long_to_int(long l, int index)
{
    if (index == 0)
    {
        return (int)(l >> 32);
    }
    else
    {
        return (int)l;
    }
}

long get_rotation(State *state, int sq)
{
    if (state->rotation == 0 || state->tetro == O)
    {
        return int_to_long(locations[state->tetro][sq][0], locations[state->tetro][sq][1]);
    }
    else if (state->rotation == 1)
    {
        return int_to_long(-locations[state->tetro][sq][1], locations[state->tetro][sq][0]);
    }
    else if (state->rotation == 2)
    {
        if (state->tetro == I || state->tetro == S || state->tetro == Z)
        {
            return int_to_long(locations[state->tetro][sq][0], locations[state->tetro][sq][1]);
        }
        return int_to_long(-locations[state->tetro][sq][0], -locations[state->tetro][sq][1]);
    }
    else if (state->rotation == 3)
    {
        if (state->tetro == I || state->tetro == S || state->tetro == Z)
        {
            return int_to_long(-locations[state->tetro][sq][1], locations[state->tetro][sq][0]);
        }
        return int_to_long(locations[state->tetro][sq][1], -locations[state->tetro][sq][0]);
    }

    return 0;
}

void draw_board() {
    for (int i = 0; i < PLAY_WIDTH; i++) {
        for (int j = 0; j < PLAY_HEIGHT; j++) {
            draw_on_area(i, j, board_data[i][j]);
        }
    }

    draw_lines();
}

void draw_piece(State *state)
{
    for (int i = 0; i < 4; i++)
    {
        long l = get_rotation(state, i);
        draw_on_area(long_to_int(l, 0) + state->x, long_to_int(l, 1) + state->y, tetro_to_colour(state->tetro));
    }
}

void delete_piece(State *state)
{
    for (int i = 0; i < 4; i++)
    {
        long l = get_rotation(state, i);
        delete_on_area(long_to_int(l, 0) + state->x, long_to_int(l, 1) + state->y);
    }
}

int legal_state(State *state)
{
    for (int i = 0; i < 4; i++)
    {
        long l = get_rotation(state, i);
        int x = long_to_int(l, 0) + state->x;
        int y = long_to_int(l, 1) + state->y;
        if (x < 0 || x >= PLAY_WIDTH || y >= PLAY_HEIGHT)
        {
            return 0;
        }
        if (y < 0) {
            return 1;
        }
        if (board_data[x][y] != 0)
        {
            return 0;
        }
    }

    return 1;
}

int update_board(State *state) {
    int sq;
    for (sq = 0; sq < 4; sq++)
    {
        long l = get_rotation(state, sq);
        int x = long_to_int(l, 0);
        int y = long_to_int(l, 1);
        board_data[state->x + x][state->y + y] = tetro_to_colour(state->tetro);
    }


    int changed = 0;
    int cleared_rows = 0;
    for (int row = 0; row < PLAY_HEIGHT; row++) {
        int full = 1;
        for (int col = 0; col < PLAY_WIDTH; col++) {
            if (board_data[col][row] == 0) {
                full = 0;
                break;
            }
        }

        if (full) {
            cleared_rows++;
            changed = 1;
            for (int col = 0; col < PLAY_WIDTH; col++) {
                board_data[col][row] = 0;
            }

            for (int row2 = row; row2 > 0; row2--) {
                for (int col = 0; col < PLAY_WIDTH; col++) {
                    board_data[col][row2] = board_data[col][row2 - 1];
                }
            }
        }
    }

    if (changed) {
        draw_board();
    }

    return cleared_rows;
}