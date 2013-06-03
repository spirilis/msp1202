/* chargen_nofb.c - Character display manager for STE2007 96x68 LCD display
 * This edition foregoes the use of a framebuffer in preference for lower RAM usage.
 *
    Copyright (c) 2013 Eric Brundick <spirilis [at] linux dot com>

    Permission is hereby granted, free of charge, to any person
    obtaining a copy of this software and associated documentation
    files (the "Software"), to deal in the Software without
    restriction, including without limitation the rights to use, copy,
    modify, merge, publish, distribute, sublicense, and/or sell copies
    of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be
    included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.
 */

#include <msp430.h>
#include <string.h>
#include <stdint.h>
#include "font_5x7.h"
#include "chargen_nofb.h"

uint8_t msp1202_x, msp1202_y;

void msp1202_init()
{
	msp1202_x = 0;
	msp1202_y = 0;
	ste2007_init();

#ifdef MSP1202_USE_CURSOR
	msp1202_writechar(MSP1202_CURSOR);
	ste2007_setxy(msp1202_x * MSP1202_CHAR_WIDTH, msp1202_y);
#endif
}

/* This is for library-internal use only. */
void msp1202_writechar(uint8_t ch)
{
	ste2007_chipselect(0);
	ste2007_write(font_5x7[ch-' '], 6);
	ste2007_chipselect(1);
}

void msp1202_putc(uint8_t c)
{
	// Process the character as is
	if (c > 0x80)  // High-bit characters treated as spaces (except 0x80 which is the cursor)
		c = 0x20;

	if (c >= 0x20) {
		msp1202_writechar(c);
		msp1202_x++;
	} else {
		// Process control character
		switch (c) {
			case '\n':
#ifdef MSP1202_USE_CURSOR
				// Erase the cursor presently at msp1202_x,msp1202_y before moving it
				msp1202_writechar(' ');
#endif
				msp1202_x = 0;
				msp1202_y++;
				ste2007_setxy(msp1202_x * MSP1202_CHAR_WIDTH, msp1202_y);
				break;
			case '\t':
#ifdef MSP1202_USE_CURSOR
				// Erase the cursor presently at msp1202_x,msp1202_y before moving it
				msp1202_writechar(' ');
#endif
				if (msp1202_x % MSP1202_TAB_SPACING == 0) {
					msp1202_x += MSP1202_TAB_SPACING;
				} else {
					msp1202_x += msp1202_x % MSP1202_TAB_SPACING;
				}
				ste2007_setxy(msp1202_x * MSP1202_CHAR_WIDTH, msp1202_y);
				break;
			case '\b':
				if (msp1202_x > 0) {  // Nothing happens if @ beginning of line
					msp1202_x--;
					// Otherwise, the previous character gets erased.
					ste2007_setxy(msp1202_x * MSP1202_CHAR_WIDTH, msp1202_y);
					msp1202_writechar(' ');
					ste2007_setxy(msp1202_x * MSP1202_CHAR_WIDTH, msp1202_y);
				}
				break;
			// No default section; any other ctrl char is ignored
		}
	}

	if (msp1202_x >= MSP1202_COLUMNS) {
		// Shift down one row
		msp1202_y++;
		msp1202_x = 0;
	}

	if (msp1202_y >= MSP1202_LINES) {
		// Without a framebuffer to store the rest of the screen, there is no scrolling;
		// we just reset the Y counter to the top of the display so it overwrites.
		msp1202_y = 0;
	}
	ste2007_setxy(msp1202_x * MSP1202_CHAR_WIDTH, msp1202_y);

#ifdef MSP1202_USE_CURSOR
	msp1202_writechar(MSP1202_CURSOR);
	ste2007_setxy(msp1202_x * MSP1202_CHAR_WIDTH, msp1202_y);
#endif
}

void msp1202_move(uint8_t x, uint8_t y)
{
#ifdef MSP1202_USE_CURSOR
	// Erase the cursor presently at msp1202_x,msp1202_y before moving it
	msp1202_writechar(' ');
#endif
	msp1202_x = x;
	msp1202_y = y;
	ste2007_setxy(msp1202_x * MSP1202_CHAR_WIDTH, msp1202_y);
#ifdef MSP1202_USE_CURSOR
	msp1202_writechar(MSP1202_CURSOR);
	ste2007_setxy(msp1202_x * MSP1202_CHAR_WIDTH, msp1202_y);
#endif
}

void msp1202_puts(const char *str)
{
	uint16_t i, j;

	j = strlen(str);
	for (i=0; i < j; i++)
		msp1202_putc((uint8_t)str[i]);
}
