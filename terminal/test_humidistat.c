#include <msp430.h>
#include <stdint.h>
#include "chargen.h"
#include "sw_spi.h"
#include "lcd_printf.h"

int main()
{
	WDTCTL = WDTPW | WDTHOLD;

	DCOCTL = CALDCO_16MHZ;
	BCSCTL1 = CALBC1_16MHZ;
	BCSCTL2 = DIVS_1;  // SMCLK = DCO/2 (8MHz)

	__delay_cycles(80000);  // Short delay to let the LCD wake up -- turns out this isn't necessary
	// Chip select
	P1DIR |= BIT1;
	P1OUT |= BIT1;  // Drive it high to disable LCD
	// Backlight
	P1DIR |= BIT3;
	P1OUT &= ~BIT3; // Turn on backlight

	spi_init();
	msp1202_init();
	lcd_printf("Hi there my\n");
	lcd_printf("name is Eric.\n");
	msp1202_move(8, 0);

	while(1) {
		__delay_cycles(16000000);
		lcd_printf("txt %d", 1);
		__delay_cycles(16000000);
		lcd_printf("txt %d", 2);
	}

	LPM4;

	return 0;
}

void test_chipselect(uint8_t onoff)
{
	if (onoff)
		P1OUT |= BIT1;
	else
		P1OUT &= ~BIT1;
}
