#include "uart.h"

void main()
{
    uart_init();
    uart_writeText("Hello, world!\n");
    int a  = 1/0;
    while (1)
        ;
}
