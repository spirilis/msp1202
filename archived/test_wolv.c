#include <msp430.h>
#include <stdint.h>
#include "chargen.h"
#include "msp430_spi.h"
#include "lcd_printf.h"

int main()
{
	WDTCTL = WDTPW | WDTHOLD;

	//__delay_cycles(8000);  // Short delay to let the LCD wake up -- turns out this isn't necessary
	// Chip select
	P1DIR |= BIT5;
	P1OUT |= BIT5;  // Drive it high to disable LCD
	// Backlight
	P1SEL0 &= ~BIT2;
	P1SEL1 &= ~BIT2;
	P1DIR |= BIT2;
	P1OUT |= BIT2; // Turn on backlight
	// LFXT1 pins
	PJSEL0 = BIT4 | BIT5;
	PJSEL1 = 0x00;

	PM5CTL0 &= ~LOCKLPM5;  // Enable GPIO on Wolverine
	// Wolverine Clock System setup
	CSCTL0_H = CSKEY >> 8;
	FRCTL0 = FWPW | NACCESS_2;  // 24MHz (2 wait states)
	CSCTL2 = SELA__LFXTCLK | SELS__DCOCLK | SELM__DCOCLK;
	CSCTL3 = DIVA__1 | DIVS__4 | DIVM__1;
	CSCTL1 = DCORSEL | DCOFSEL_6;  // 24MHz
	CSCTL4 &= ~LFXTOFF;
	do {
		CSCTL5 &= ~LFXTOFFG;
		SFRIFG1 &= ~OFIFG;
	} while (SFRIFG1 & OFIFG);
	CSCTL0_H = 0;

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
		P1OUT |= BIT5;
	else
		P1OUT &= ~BIT5;
}
