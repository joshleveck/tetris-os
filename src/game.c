#include "drawing.h"
#include "delay.h"
#include "tetro.h"
#include "uart.h"

void game_loop()
{
    draw_init();
    draw_lines();
    unsigned int time = 0;
    int i = 1;

    while (1)
    {
        if (i <= PLAY_HEIGHT && time == 1000000)
        {
            // clear_area();
            if (i != 1) delete_piece(create_state(4, i - 1, 0, 0));
            draw_piece(create_state(4, i, 0, 0));
            // draw_lines();
            i++;
            time = 0;
        }
        time++;
        char c = check_uart_getc();
        if (c != 0) {
            uart_send(c);
        }
        wait_us(1);
    }
}