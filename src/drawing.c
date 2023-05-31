#include "lfb.h"
#include "tetro.h"
#include "colour.h"
#include "drawing.h"

void draw_init()
{
    lfb_init();
    draw_area();
    draw_title();
    score_init();
    draw_score(0);
}

void draw_area()
{
    for (int i = -1; i <= PLAY_HEIGHT; i++)
    {
        draw_block(TOP_CORNER_X - 1, TOP_CORNER_Y + i, GREY);
        draw_block(TOP_CORNER_X + PLAY_WIDTH, TOP_CORNER_Y + i, GREY);
    }

    for (int i = -1; i <= PLAY_WIDTH ; i++)
    {
        draw_block(TOP_CORNER_X + i, TOP_CORNER_Y - 1, GREY);
        draw_block(TOP_CORNER_X + i, TOP_CORNER_Y + PLAY_HEIGHT, GREY);
    }
}

void draw_title() {
    lfb_print(TITLE_X, TITLE_Y, "TETRIS");
}

// I just really don't want to implement malloc
// So we'll jank our way through this
char score_string[15];
int score_start = 7;
void score_init() {
    score_string[0] = 'S';
    score_string[1] = 'c';
    score_string[2] = 'o';
    score_string[3] = 'r';
    score_string[4] = 'e';
    score_string[5] = ':';
    score_string[6] = ' ';
    score_string[7] = '0';
    score_string[8] = '\0';
}

void draw_score(int score) {
    int digits = 0;
    int temp = score;
    while (temp > 0) {
        temp /= 10;
        digits++;
    }

    for (int i = score_start + digits - 1; i >= score_start; i--) {
        score_string[i] = (score % 10) + '0';
        score /= 10;
    }

    score_string[score_start + digits] = '\0';

    lfb_print(SCORE_X, SCORE_Y, &score_string[0]);
}

void delete_score() {
    lfb_print(SCORE_X, SCORE_Y, &score_string[0]);
}

void draw_lines()
{
    for (int i = 0; i < PLAY_HEIGHT; i++)
    {
        draw_x_line((TOP_CORNER_X) * SQUARE_LENGTH, (TOP_CORNER_Y + i + 1) * SQUARE_LENGTH, PLAY_WIDTH * SQUARE_LENGTH, LINES);
    }

    for (int i = 0; i < PLAY_WIDTH; i++)
    {
        draw_y_line((TOP_CORNER_X + i + 1) * SQUARE_LENGTH, (TOP_CORNER_Y) * SQUARE_LENGTH, PLAY_HEIGHT * SQUARE_LENGTH, LINES);
    }
}

void draw_on_area(int x, int y, unsigned int colour)
{
    if (y < 0) return;
    draw_block(TOP_CORNER_X + x, TOP_CORNER_Y + y, colour);
    draw_x_line((TOP_CORNER_X + x) * SQUARE_LENGTH, (TOP_CORNER_Y + y) * SQUARE_LENGTH, SQUARE_LENGTH, LINES);
    draw_y_line((TOP_CORNER_X + x) * SQUARE_LENGTH, (TOP_CORNER_Y + y) * SQUARE_LENGTH, SQUARE_LENGTH, LINES);
}

void delete_on_area(int x, int y)
{
    if (y < 0) return;
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