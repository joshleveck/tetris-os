#ifndef DRAWING_H
#define DRAWING_H

#define TOP_CORNER_X 14
#define TOP_CORNER_Y 5
#define PLAY_WIDTH 10
#define PLAY_HEIGHT 20

#define SQUARE_LENGTH 25

void draw_init();
void draw_area();
void draw_on_area(int x, int y, unsigned int colour);
void delete_on_area(int x, int y);
void clear_area();
void draw_lines();

#endif