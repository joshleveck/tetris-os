#ifndef DRAWING_H
#define DRAWING_H

#define TOP_CORNER_X 15
#define TOP_CORNER_Y 6
#define PLAY_WIDTH 10
#define PLAY_HEIGHT 20

#define TITLE_X 475
#define TITLE_Y 75

#define SCORE_X 700
#define SCORE_Y 200

#define SQUARE_LENGTH 25

void draw_init();
void draw_area();
void draw_title();
void score_init();
void draw_score(int score);
void delete_score();
void draw_on_area(int x, int y, unsigned int colour);
void delete_on_area(int x, int y);
void clear_area();
void draw_lines();

#endif