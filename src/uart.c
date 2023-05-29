#include "gpio.h"
#include "mbox.h"

#define UART0_DR ((volatile unsigned int *)(MMIO_BASE + 0x00201000))
#define UART0_FR ((volatile unsigned int *)(MMIO_BASE + 0x00201018))
#define UART0_IBRD ((volatile unsigned int *)(MMIO_BASE + 0x00201024))
#define UART0_FBRD ((volatile unsigned int *)(MMIO_BASE + 0x00201028))
#define UART0_LCRH ((volatile unsigned int *)(MMIO_BASE + 0x0020102C))
#define UART0_CR ((volatile unsigned int *)(MMIO_BASE + 0x00201030))
#define UART0_IMSC ((volatile unsigned int *)(MMIO_BASE + 0x00201038))
#define UART0_ICR ((volatile unsigned int *)(MMIO_BASE + 0x00201044))

/**
 * Set baud rate and characteristics (115200 8N1) and map to GPIO
 */
void uart_init()
{
    register unsigned int r;

    *UART0_CR = 0;

    mbox[0] = 9 * 4;
    mbox[1] = MBOX_REQUEST;
    mbox[2] = MBOX_TAG_SETCLKRATE;
    mbox[3] = 12;
    mbox[4] = 8;
    mbox[5] = 2;
    mbox[6] = 4000000;
    mbox[7] = 0;
    mbox[8] = MBOX_TAG_LAST;
    mbox_call(MBOX_CH_PROP);

    r = *GPFSEL1;
    r &= ~((7 << 12) | (7 << 15));
    r |= (4 << 12) | (4 << 15);
    *GPFSEL1 = r;
    *GPPUD = 0;
    r = 150;
    while (r--)
    {
        asm volatile("nop");
    }
    *GPPUDCLK0 = (1 << 14) | (1 << 15);
    r = 150;
    while (r--)
    {
        asm volatile("nop");
    }
    *GPPUDCLK0 = 0;

    *UART0_ICR = 0x7FF;
    *UART0_IBRD = 2;
    *UART0_FBRD = 0xB;
    *UART0_LCRH = 0x7 << 4;
    *UART0_CR = 0x301;
}

/**
 * Send a character
 */
void uart_send(unsigned int c)
{
    do
    {
        asm volatile("nop");
    } while (*UART0_FR & 0x20);
    *UART0_DR = c;
}

/**
 * Receive a character
 */
char uart_getc()
{
    char r;
    do
    {
        asm volatile("nop");
    } while (*UART0_FR & 0x10);

    r = (char)(*UART0_DR);

    return r == '\r' ? '\n' : r;
}

char check_uart_getc()
{
    if (*UART0_FR & 0x10)
        return 0;
    return (char)(*UART0_DR);
}

/**
 * Display a string
 */
void uart_puts(char *s)
{
    while (*s)
    {
        if (*s == '\n')
            uart_send('\r');
        uart_send(*s++);
    }
}

void uart_hex(unsigned int d)
{
    unsigned int n;
    int c;
    for (c = 28; c >= 0; c -= 4)
    {
        n = (d >> c) & 0xF;
        n += n > 9 ? 0x37 : 0x30;
        uart_send(n);
    }
}