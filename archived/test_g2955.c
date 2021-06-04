#include <msp430.h>
#include <stdint.h>
#include "chargen.h"
#include "msp430_spi.h"

int main()
{
	WDTCTL = WDTPW | WDTHOLD;

	DCOCTL = CALDCO_16MHZ;
	BCSCTL1 = CALBC1_16MHZ;
	BCSCTL2 = DIVS_3;  // SMCLK = DCO/8 (2MHz)

	//__delay_cycles(8000);  // Short delay to let the LCD wake up -- turns out this isn't necessary
	// Chip select
	P2SEL &= ~BIT4;
	P2SEL2 &= ~BIT4;
	P2REN &= ~BIT4;
	P2DIR |= BIT4;
	P2OUT |= BIT4;  // Drive it high to disable LCD
	// Backlight
	P3SEL &= ~BIT6;
	P3SEL2 &= ~BIT6;
	P3REN &= ~BIT6;
	P3DIR |= BIT6;
	P3OUT |= BIT6; // Turn on backlight

	spi_init();
	// 250ms delay to let the Nokia display wake up out of RESET
	__delay_cycles(16000000 / 4);

	msp1202_init();
	ste2007_contrast(8);
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
	if (onoff)
		P2OUT |= BIT4;
	else
		P2OUT &= ~BIT4;
}
