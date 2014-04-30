#include <msp430.h>
#include <stdint.h>
#include "chargen.h"
#include "msp430_spi.h"
#include "clockinit.h"

int main()
{
	WDTCTL = WDTPW | WDTHOLD;

	P4DIR |= BIT7;
	P4OUT |= BIT7;
	if (!ucs_clockinit(16000000, 1, 0))
		LPM4;
	P4OUT &= ~BIT7;
	UCSCTL5 = DIVS__4;

	// Chip select
	P2SEL &= ~BIT7;
	P2REN &= ~BIT7;
	P2DIR |= BIT7;
	P2OUT |= BIT7;  // Drive it high to disable LCD
	// Backlight
	P2SEL &= ~BIT6;
	P2REN &= ~BIT6;
	P2DIR |= BIT6;
	P2OUT &= ~BIT6; // Turn off backlight

	spi_init();
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
		P2OUT |= BIT7;
	else
		P2OUT &= ~BIT7;
}
