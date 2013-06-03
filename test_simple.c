#include <msp430.h>
#include <stdint.h>
#include "usci_spi.h"
#include "ste2007.h"
#include "font_5x7.h"

int main()
{
	WDTCTL = WDTPW | WDTHOLD;

	DCOCTL = CALDCO_16MHZ;
	BCSCTL1 = CALBC1_16MHZ;
	BCSCTL2 = DIVS_1;  // SMCLK = DCO/2 (8MHz)

	//__delay_cycles(8000);  // Short delay to let the LCD wake up -- turns out this isn't necessary
	// Chip select
	P2DIR |= BIT0;
	P2OUT |= BIT0;  // Drive it high to disable LCD
	// Backlight
	P2DIR |= BIT5;
	P2OUT |= BIT5; // Turn on backlight

	spi_init();
	ste2007_init();
	ste2007_invert(0);  // Normal display, not inverted.
	ste2007_putchar(0, 0, font_5x7['!'-32]);
	ste2007_putchar(-1, 0, font_5x7['A'-32]);
	ste2007_putchar(-1, 0, font_5x7['C'-32]);
	ste2007_putchar(-1, 0, font_5x7['D'-32]);
	ste2007_putchar(-1, 0, font_5x7['~'-32]);
	ste2007_putchar(0, 1, font_5x7['M'-32]);
	ste2007_putchar(-1, 1, font_5x7['y'-32]);
	ste2007_putchar(-1, 1, font_5x7[' '-32]);
	ste2007_putchar(-1, 1, font_5x7['n'-32]);
	ste2007_putchar(-1, 1, font_5x7['a'-32]);
	ste2007_putchar(-1, 1, font_5x7['m'-32]);
	ste2007_putchar(-1, 1, font_5x7['e'-32]);
	ste2007_putchar(-1, 1, font_5x7[' '-32]);
	ste2007_putchar(-1, 1, font_5x7['i'-32]);
	ste2007_putchar(-1, 1, font_5x7['s'-32]);
	ste2007_putchar(0, 2, font_5x7['E'-32]);
	ste2007_putchar(-1, 2, font_5x7['r'-32]);
	ste2007_putchar(-1, 2, font_5x7['i'-32]);
	ste2007_putchar(-1, 2, font_5x7['c'-32]);
	ste2007_putchar(-1, 2, font_5x7['.'-32]);

	ste2007_contrast(10);  // A little lighter than the default
	while(1) {
		__delay_cycles(16000000);
		ste2007_powersave(1);
		__delay_cycles(16000000);
		ste2007_powersave(0);
	}

	LPM4;

	return 0;
}

void test_chipselect(uint8_t onoff)
{
	P2OUT = (P2OUT & ~BIT0) | (onoff & 0x01);
}
