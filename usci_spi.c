#include <msp430.h>
#include <stdint.h>
#include "usci_spi.h"

// USCI for F2xxx and G2xxx devices
#if defined(__MSP430_HAS_USCI__)
void spi_init()
{
	/* Configure ports on MSP430 device for USCI_B */
#ifndef __MSP430_HAS_TB3__
	// G2xx3 devices
	P1DIR |= BIT5 | BIT7;
	P1DIR &= ~BIT6;
	P1SEL |= BIT5 | BIT6 | BIT7;
	P1SEL2 |= BIT5 | BIT6 | BIT7;
#else
	// New G2xx4/G2xx5 devices
	P3DIR |= BIT1 | BIT3;
	P3DIR &= ~BIT2;
	P3SEL |= BIT1 | BIT2 | BIT3;
	P3SEL2 &= ~(BIT1 | BIT2 | BIT3);
#endif

	/* USCI-B specific SPI setup */
	UCB0CTL1 |= UCSWRST;
	UCB0CTL0 = UCCKPH | UCMSB | UCMST | UCMODE_0 | UCSYNC;  // SPI mode 0, master
	UCB0BR0 = 0x01;  // SPI clocked at same speed as SMCLK
	UCB0BR1 = 0x00;
	UCB0CTL1 = UCSSEL_2;  // Clock = SMCLK, clear UCSWRST and enables USCI_B module.
}

uint8_t spi_transfer(uint8_t inb)
{
	UCB0TXBUF = inb;
	while ( !(IFG2 & UCB0RXIFG) )  // Wait for RXIFG indicating remote byte received via SOMI
		;
	return UCB0RXBUF;
}

uint16_t spi_transfer16(uint16_t inw)
{
	uint16_t retw;

	UCB0TXBUF = (inw >> 8) & 0xFF;
	while ( !(IFG2 & UCB0RXIFG) )
		;
	retw = UCB0RXBUF << 8;
	UCB0TXBUF = inw & 0xFF;
	while ( !(IFG2 & UCB0RXIFG) )
		;
	retw |= UCB0RXBUF;
	return retw;
}

#ifndef __MSP430_HAS_TB3__
// G2xx3 devices
uint16_t spi_transfer9(uint16_t inw)
{
	uint16_t retw = 0;
	uint8_t p1dir_save, p1out_save, p1ren_save;

	// SPI mode 0, MSB first, bit #9 sent manually first
	p1dir_save = P1DIR;
	p1out_save = P1OUT;
	p1ren_save = P1REN;

	P1REN &= ~(BIT5 | BIT6 | BIT7);
	P1OUT &= ~(BIT5 | BIT6 | BIT7);
	P1DIR = (p1dir_save & ~(BIT5 | BIT6 | BIT7)) | BIT5 | BIT7;  // Maybe save a memory-read by reusing p1dir_save

	P1SEL2 &= ~(BIT5 | BIT6 | BIT7);
	P1SEL &= ~(BIT5 | BIT6 | BIT7);

	if (inw & 0x0100)
		P1OUT |= BIT7;  // Set MOSI before initial SCLK edge
	P1OUT |= BIT5;  // SCLK high
	// Sample MISO
	if (P1IN & BIT6)
		retw |= 0x0100;
	P1OUT &= ~BIT5; // SCLK low
	// Return port state over to USCI
	P1SEL |= BIT5 | BIT6 | BIT7;
	P1SEL2 |= BIT5 | BIT6 | BIT7;
	P1DIR = p1dir_save;
	P1OUT = p1out_save;
	P1REN = p1ren_save;

	// Transfer the remaining 8 bits
	retw |= spi_transfer((uint8_t)(inw & 0x00FF));

	return retw;
}

#else

// New G2xx4/G2xx5 devices
uint16_t spi_transfer9(uint16_t inw)
{
	uint16_t retw = 0;
	uint8_t p3dir_save, p3out_save, p3ren_save;

	// SPI mode 0, MSB first, bit #9 sent manually first
	p3dir_save = P3DIR;
	p3out_save = P3OUT;
	p3ren_save = P3REN;

	P3REN &= ~(BIT1 | BIT2 | BIT3);
	P3OUT &= ~(BIT1 | BIT2 | BIT3);
	P3DIR = (p3dir_save & ~(BIT1 | BIT2 | BIT3)) | BIT1 | BIT3;  // Maybe save a memory-read by reusing p3dir_save

	P3SEL2 &= ~(BIT1 | BIT2 | BIT3);
	P3SEL &= ~(BIT1 | BIT2 | BIT3);

	if (inw & 0x0100)
		P3OUT |= BIT1;  // Set MOSI before initial SCLK edge
	P3OUT |= BIT3;  // SCLK high
	// Sample MISO
	if (P3IN & BIT2)
		retw |= 0x0100;
	P3OUT &= ~BIT3; // SCLK low
	// Return port state over to USCI
	P3SEL |= BIT1 | BIT2 | BIT3;
	//P3SEL2 |= BIT1 | BIT2 | BIT3;
	P3DIR = p3dir_save;
	P3OUT = p3out_save;
	P3REN = p3ren_save;

	// Transfer the remaining 8 bits
	retw |= spi_transfer((uint8_t)(inw & 0x00FF));

	return retw;
}
#endif

#endif
