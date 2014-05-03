#include <msp430.h>
#include <stdint.h>
#include "chargen.h"
#include "msp430_spi.h"
#include "lcd_printf.h"

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
	// Pushbuttons
	P2DIR &= ~(BIT3 | BIT4);
	P2OUT |= BIT3 | BIT4;
	P2REN |= BIT3 | BIT4;
	P2IES |= BIT3 | BIT4;
	P2IFG &= ~(BIT3 | BIT4);
	P2IE |= BIT3 | BIT4;

	spi_init();
	msp1202_init();
	ste2007_contrast(8);
	lcd_printf("Hi there my\n");
	lcd_printf("name is Eric.\n");
	_EINT();

	while(1) {
		if ( (~P2IN) & (BIT3|BIT4) ) {
			if (!(P2IN & BIT3)) {
				lcd_printf("S1 pressed\n");
				P2OUT ^= BIT5;
			}
			if (!(P2IN & BIT4)) {
				lcd_printf("S2 pressed\n");
			}
		}
		LPM4;
	}

	return 0;
}

void test_chipselect(uint8_t onoff)
{
	P2OUT = (P2OUT & ~BIT0) | (onoff & 0x01);
}

#pragma vector = PORT2_VECTOR
__interrupt void P2_IRQ (void) {
	if (P2IFG & (BIT3|BIT4)) {
		P2IFG &= ~(BIT3 | BIT4);
		__bic_SR_register_on_exit(LPM4_bits);    // Wake up
	}
}
