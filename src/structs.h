#ifndef STRUCTS_H
#define STRUCTS_H

typedef struct
{
    int x;
    int y;
    int rotation;
    int tetro;
} State;

typedef struct
{
    int width;
    int height;
    int (*data)[10][20];
} Board;

#endif