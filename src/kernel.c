#include "uart.h"
#include "lfb.h"
#include "delay.h"
#include "drawing.h"
#include "tetro.h"
#include "game.h"

void main()
{
    // set up serial console and linear frame buffer
    uart_init();
    lfb_init();

    game_loop();


    // echo everything back
    while(1) {
        // uart_puts("HERE\n");
        uart_send(uart_getc());
        wait_us(1000000);
        draw_block(4,5, 0x00FF00);
        uart_puts("PINGING\n");
    }
}
