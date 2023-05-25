#include "uart.h"
#include "lfb.h"
#include "delay.h"
#include "drawing.h"
#include "tetro.h"

void main()
{
    // set up serial console and linear frame buffer
    uart_init();
    lfb_init();

    // display an ASCII string on screen with PSF
    //lfb_print(80, 80, "Hello World!");

    // display a UTF-8 string on screen with SSFN
    //lfb_proprint(80, 120, "Hello 多种语言 Многоязычный többnyelvű World!");

    draw_init();


    // echo everything back
    while(1) {
        uart_puts("HERE\n");
        uart_send(uart_getc());
        wait_us(1000000);
        draw_block(4,5, 0x00FF00);
        uart_puts("PINGING\n");
    }
}
