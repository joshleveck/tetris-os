#include "lfb.h"
#include "tetro.h"
#include "colour.h"
#include "drawing.h"

void draw_init()
{
    lfb_init();

    draw_area();
}

void draw_area()
{
    for (int i = 0; i <= PLAY_HEIGHT  +1; i++)
    {
        // draw_x_line(TOP_CORNER_X * SQUARE_LENGTH, (TOP_CORNER_Y + i) * SQUARE_LENGTH, PLAY_WIDTH * SQUARE_LENGTH, LINES);
        draw_block(TOP_CORNER_X, TOP_CORNER_Y + i, GREY);
        draw_block(TOP_CORNER_X + PLAY_WIDTH + 1, TOP_CORNER_Y + i, GREY);
    }

    for (int i = 0; i <= PLAY_WIDTH + 1; i++)
    {
        // draw_y_line((TOP_CORNER_X + i) * SQUARE_LENGTH, TOP_CORNER_Y * SQUARE_LENGTH, PLAY_HEIGHT * SQUARE_LENGTH, LINES);
        draw_block(TOP_CORNER_X + i, TOP_CORNER_Y, GREY);
        draw_block(TOP_CORNER_X + i, TOP_CORNER_Y + PLAY_HEIGHT + 1, GREY);
    }
}

void draw_lines()
{
    for (int i = 0; i <= PLAY_HEIGHT; i++)
    {
        draw_x_line((TOP_CORNER_X+1) * SQUARE_LENGTH, (TOP_CORNER_Y + i) * SQUARE_LENGTH, PLAY_WIDTH * SQUARE_LENGTH, LINES);
    }

    for (int i = 0; i <= PLAY_WIDTH; i++)
    {
        draw_y_line((TOP_CORNER_X + i) * SQUARE_LENGTH, (TOP_CORNER_Y+1) * SQUARE_LENGTH, PLAY_HEIGHT * SQUARE_LENGTH, LINES);
    }
}

void draw_on_area(int x, int y, unsigned int colour)
{
    draw_block(TOP_CORNER_X + x, TOP_CORNER_Y + y, colour);
}

void delete_on_area(int x, int y)
{
    draw_block(TOP_CORNER_X + x, TOP_CORNER_Y + y, BLACK);
    draw_x_line((TOP_CORNER_X + x) * SQUARE_LENGTH, (TOP_CORNER_Y + y) * SQUARE_LENGTH, SQUARE_LENGTH, LINES);
    draw_y_line((TOP_CORNER_X + x) * SQUARE_LENGTH, (TOP_CORNER_Y + y) * SQUARE_LENGTH, SQUARE_LENGTH, LINES);
}

void clear_area()
{
    for (int i = 1; i < PLAY_HEIGHT; i++)
    {
        for (int j = 1; j < PLAY_WIDTH; j++)
        {
            draw_on_area(j, i, BLACK);
        }
    }
}