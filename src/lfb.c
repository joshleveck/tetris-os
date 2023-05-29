#include "uart.h"
#include "mbox.h"
#include "delay.h"
#include "drawing.h"
#include "colour.h"

/* PC Screen Font as used by Linux Console */
typedef struct
{
    unsigned int magic;
    unsigned int version;
    unsigned int headersize;
    unsigned int flags;
    unsigned int numglyph;
    unsigned int bytesperglyph;
    unsigned int height;
    unsigned int width;
    unsigned char glyphs;
} __attribute__((packed)) psf_t;
extern volatile unsigned char _binary_lib_font_psf_start;

/* Scalable Screen Font (https://gitlab.com/bztsrc/scalable-font2) */
typedef struct
{
    unsigned char magic[4];
    unsigned int size;
    unsigned char type;
    unsigned char features;
    unsigned char width;
    unsigned char height;
    unsigned char baseline;
    unsigned char underline;
    unsigned short fragments_offs;
    unsigned int characters_offs;
    unsigned int ligature_offs;
    unsigned int kerning_offs;
    unsigned int cmap_offs;
} __attribute__((packed)) sfn_t;
extern volatile unsigned char _binary_lib_font_sfn_start;

unsigned int width, height, pitch, isrgb;
unsigned char *lfb;

void lfb_init()
{
    wait_us(100000); // smth about a segfault ?

    mbox[0] = 35 * 4;
    mbox[1] = MBOX_REQUEST;

    mbox[2] = 0x48003; // set physical wh
    mbox[3] = 8;
    mbox[4] = 8;
    mbox[5] = 1024; // FrameBufferInfo.width
    mbox[6] = 768;  // FrameBufferInfo.height

    mbox[7] = 0x48004; // set virt wh
    mbox[8] = 8;
    mbox[9] = 8;
    mbox[10] = 1024; // FrameBufferInfo.virtual_width
    mbox[11] = 768;  // FrameBufferInfo.virtual_height

    mbox[12] = 0x48009; // set virtual offset
    mbox[13] = 8;
    mbox[14] = 8;
    mbox[15] = 0; // FrameBufferInfo.offset_x
    mbox[16] = 0; // FrameBufferInfo.offset_y

    mbox[17] = 0x48005; // set depth
    mbox[18] = 4;
    mbox[19] = 4;
    mbox[20] = 32; // FrameBufferInfo.depth

    mbox[21] = 0x48006; // set pixel order
    mbox[22] = 4;
    mbox[23] = 4;
    mbox[24] = 1; // RGB, not BGR preferably

    mbox[25] = 0x40001; // get framebuffer, gets alignment on request
    mbox[26] = 8;
    mbox[27] = 8;
    mbox[28] = 4096; // FrameBufferInfo.pointer
    mbox[29] = 0;    // FrameBufferInfo.size

    mbox[30] = 0x40008; // get pitch
    mbox[31] = 4;
    mbox[32] = 4;
    mbox[33] = 0; // FrameBufferInfo.pitch

    mbox[34] = MBOX_TAG_LAST;

    if (mbox_call(MBOX_CH_PROP) && mbox[20] == 32 && mbox[28] != 0)
    {
        mbox[28] &= 0x3FFFFFFF; // convert GPU address to ARM address
        width = mbox[5];        // get actual physical width
        height = mbox[6];       // get actual physical height
        pitch = mbox[33];       // get number of bytes per line
        isrgb = mbox[24];       // get the actual channel order
        lfb = (void *)((unsigned long)mbox[28]);
    }
    else
    {
        uart_puts("Unable to set screen resolution to 1024x768x32\n");
    }
}

void lfb_print(int x, int y, char *s)
{
    psf_t *font = (psf_t *)&_binary_lib_font_psf_start;
    while (*s)
    {
        unsigned char *glyph = (unsigned char *)&_binary_lib_font_psf_start + font->headersize + (*((unsigned char *)s) < font->numglyph ? *s : 0) * font->bytesperglyph;

        int offs = (y * pitch) + (x * 4);

        int i, j, line, mask, bytesperline = (font->width + 7) / 8;
        if (*s == '\r')
        {
            x = 0;
        }
        else
        {
            for (j = 0; j < font->height; j++)
            {
                line = offs;
                mask = 1 << (font->width - 1);
                for (i = 0; i < font->width; i++)
                {
                    *((unsigned int *)(lfb + line)) = ((int)*glyph) & (mask) ? 0xFFFFFF : 0;
                    mask >>= 1;
                    line += 4;
                }

                glyph += bytesperline;
                offs += pitch;
            }

            x += font->width + 1;
        }

        s++;
    }
}

void lfb_proprint(int x, int y, char *s)
{
    sfn_t *font = (sfn_t *)&_binary_lib_font_sfn_start;
    unsigned char *ptr, *chr, *frg;
    unsigned int c;
    unsigned long o, p;
    int i, j, k, l, m, n;

    while (*s)
    {
        if ((*s & 128) != 0)
        {
            if (!(*s & 32))
            {
                c = ((*s & 0x1F) << 6) | (*(s + 1) & 0x3F);
                s += 1;
            }
            else if (!(*s & 16))
            {
                c = ((*s & 0xF) << 12) | ((*(s + 1) & 0x3F) << 6) | (*(s + 2) & 0x3F);
                s += 2;
            }
            else if (!(*s & 8))
            {
                c = ((*s & 0x7) << 18) | ((*(s + 1) & 0x3F) << 12) | ((*(s + 2) & 0x3F) << 6) | (*(s + 3) & 0x3F);
                s += 3;
            }
            else
                c = 0;
        }
        else
            c = *s;
        s++;

        // handle carrige return
        if (c == '\r')
        {
            x = 0;
            continue;
        }
        else
            // new line
            if (c == '\n')
            {
                x = 0;
                y += font->height;
                continue;
            }

        for (ptr = (unsigned char *)font + font->characters_offs, chr = 0, i = 0; i < 0x110000; i++)
        {
            if (ptr[0] == 0xFF)
            {
                i += 65535;
                ptr++;
            }
            else if ((ptr[0] & 0xC0) == 0xC0)
            {
                j = (((ptr[0] & 0x3F) << 8) | ptr[1]);
                i += j;
                ptr += 2;
            }
            else if ((ptr[0] & 0xC0) == 0x80)
            {
                j = (ptr[0] & 0x3F);
                i += j;
                ptr++;
            }
            else
            {
                if ((unsigned int)i == c)
                {
                    chr = ptr;
                    break;
                }
                ptr += 6 + ptr[1] * (ptr[0] & 0x40 ? 6 : 5);
            }
        }
        if (!chr)
            continue;

        ptr = chr + 6;
        o = (unsigned long)lfb + y * pitch + x * 4;
        for (i = n = 0; i < chr[1]; i++, ptr += chr[0] & 0x40 ? 6 : 5)
        {
            if (ptr[0] == 255 && ptr[1] == 255)
                continue;
            frg = (unsigned char *)font + (chr[0] & 0x40 ? ((ptr[5] << 24) | (ptr[4] << 16) | (ptr[3] << 8) | ptr[2]) : ((ptr[4] << 16) | (ptr[3] << 8) | ptr[2]));
            if ((frg[0] & 0xE0) != 0x80)
                continue;
            o += (int)(ptr[1] - n) * pitch;
            n = ptr[1];
            k = ((frg[0] & 0x1F) + 1) << 3;
            j = frg[1] + 1;
            frg += 2;
            for (m = 1; j; j--, n++, o += pitch)
                for (p = o, l = 0; l < k; l++, p += 4, m <<= 1)
                {
                    if (m > 0x80)
                    {
                        frg++;
                        m = 1;
                    }
                    if (*frg & m)
                        *((unsigned int *)p) = 0xFFFFFF;
                }
        }
        // add advances
        x += chr[4] + 1;
        y += chr[5];
    }
}

void draw_block_simple(int x, int y, unsigned int colour)
{
    unsigned char *ptr = lfb;
    colour = ((colour & 0xFF) << 16) | ((colour & 0xFF00)) | ((colour & 0xFF0000) >> 16);

    ptr += (y * pitch * SQUARE_LENGTH) + (x * 4 * SQUARE_LENGTH);
    for (int i = 0; i < SQUARE_LENGTH; i++)
    {
        for (int j = 0; j < SQUARE_LENGTH; j++)
        {
            *((unsigned int *)ptr) = colour;
            ptr += 4;
        }
        ptr += pitch - SQUARE_LENGTH * 4;
    }
}

void draw_x_line(int start_x, int start_y, int length, unsigned int colour)
{
    unsigned char *ptr1 = lfb;
    ptr1 += (start_y * pitch) + (start_x * 4);

    for (int i = 0; i < length; i++)
    {
        *((unsigned int *)ptr1) = colour;
        ptr1 += 4;
    }
}

void draw_y_line(int start_x, int start_y, int length, unsigned int colour)
{
    unsigned char *ptr1 = lfb;
    ptr1 += (start_y * pitch) + (start_x * 4);

    for (int i = 0; i < length; i++)
    {
        *((unsigned int *)ptr1) = colour;
        ptr1 += pitch;
    }
}

void draw_block(int x, int y, unsigned int colour)
{
    draw_block_simple(x, y, colour);
    // int width = 3;

    // for (int i = 0; i < width; i++)
    // {
    //     draw_x_line(x * SQUARE_LENGTH + i, y * SQUARE_LENGTH + i, SQUARE_LENGTH - i * 2, colour + UPPER_OFFSET);
    //     draw_y_line(x * SQUARE_LENGTH + i, y * SQUARE_LENGTH + i, SQUARE_LENGTH - i * 2, colour + UPPER_OFFSET);

    //     draw_x_line(x * SQUARE_LENGTH + i, y * SQUARE_LENGTH + SQUARE_LENGTH - i, SQUARE_LENGTH - i * 2, colour - LOWER_OFFSET);
    //     draw_y_line(x * SQUARE_LENGTH + SQUARE_LENGTH - i, y * SQUARE_LENGTH + i, SQUARE_LENGTH - i * 2, colour - LOWER_OFFSET);
    // }
}