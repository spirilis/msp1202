#include <msp430.h>
#include <stdint.h>
#include "chargen.h"
#include "usci_spi.h"

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
	msp1202_init();
	msp1202_puts("Hi there my\n");
	msp1202_puts("name is Eric.\n");
	msp1202_move(8, 0);

	while(1) {
		__delay_cycles(16000000);
		msp1202_puts("txt 1");
		__delay_cycles(16000000);
		msp1202_puts("txt 2");
	}

	LPM4;

	return 0;
}

void test_chipselect(uint8_t onoff)
{
	P2OUT = (P2OUT & ~BIT0) | (onoff & 0x01);
}
