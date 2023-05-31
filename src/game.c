#include "drawing.h"
#include "delay.h"
#include "tetro.h"
#include "uart.h"
#include "rand.h"

void init_all()
{
    draw_init();
    draw_lines();
    rand_init();
}

void game_loop()
{
    init_all();
    unsigned int time = 0;
    int score = 0;
    int y = -1;
    int rotation = 0;
    int x = 4;
    int piece = 0;

    while (1)
    {
        if (time == 1000000)
        {
            if (legal_state(create_state(x, y + 1, rotation, piece)))
            {
                delete_piece(create_state(x, y, rotation, piece));
                y++;
                draw_piece(create_state(x, y, rotation, piece));
            }
            else
            {
                int cleared_rows = update_board(create_state(x, y, rotation, piece));
                
                switch (cleared_rows)
                {
                case 1:
                    score += 100;
                    break;
                case 2:
                    score += 300;
                    break;
                case 3:
                    score += 500;
                    break;
                case 4:
                    score += 800;
                    break;

                default:
                    break;
                }

                delete_score();
                draw_score(score);

                y = 1;
                x = 4;
                rotation = 0;
                piece = rand(0, 6);
                draw_piece(create_state(x, y, rotation, piece));
            }
            time = 0;
        }

        time++;
        char c = check_uart_getc();
        if (c != 0)
        {
            if (c == 'a')
            {
                if (legal_state(create_state(x - 1, y, rotation, piece)))
                {
                    delete_piece(create_state(x, y, rotation, piece));
                    x--;
                    draw_piece(create_state(x, y, rotation, piece));
                }
            }
            else if (c == 'd')
            {
                if (legal_state(create_state(x + 1, y, rotation, piece)))
                {
                    delete_piece(create_state(x, y, rotation, piece));
                    x++;
                    draw_piece(create_state(x, y, rotation, piece));
                }
            }
            else if (c == 'w')
            {
                if (legal_state(create_state(x, y, (rotation + 1) % 4, piece)))
                {
                    delete_piece(create_state(x, y, rotation, piece));
                    rotation = (rotation + 1) % 4;
                    draw_piece(create_state(x, y, rotation, piece));
                }
            }
            else if (c == 's')
            {
                if (legal_state(create_state(x, y + 1, rotation, piece)))
                {
                    delete_piece(create_state(x, y, rotation, piece));
                    y++;
                    draw_piece(create_state(x, y, rotation, piece));
                }
            }
            else if (c == ' ')
            {
                int score_add = 0;
                while (legal_state(create_state(x, y + 1, rotation, piece)))
                {
                    delete_piece(create_state(x, y, rotation, piece));
                    y++;
                    draw_piece(create_state(x, y, rotation, piece));
                    score_add++;
                }

                score += score_add;
                delete_score();
                draw_score(score);
            }
        }
        wait_us(1);
    }
}