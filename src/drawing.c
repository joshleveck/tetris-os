#include "lfb.h"
#include "tetro.h"

#define TOP_CORNER_X 15
#define TOP_CORNER_Y 6
#define PLAY_WIDTH 10
#define PLAY_HEIGHT 20

void draw_init() {
    lfb_init();

    for (int i = 0; i <= PLAY_HEIGHT; i++) {
        draw_block(TOP_CORNER_X, TOP_CORNER_Y + i, GREY);
        draw_block(TOP_CORNER_X + PLAY_WIDTH, TOP_CORNER_Y + i, GREY);
    }

    for (int i = 0; i <= PLAY_WIDTH; i++) {
        draw_block(TOP_CORNER_X + i, TOP_CORNER_Y, GREY);
        draw_block(TOP_CORNER_X + i, TOP_CORNER_Y + PLAY_HEIGHT, GREY);
    }
}