msp1202
=======

MSP430 library to support Nokia 1202 / STE2007 LCD displays

## SYNOPSIS
- msp1202 includes the STE2007 library for primitive I/O to the Nokia 1202 LCD
- STE2007 library utilizes an SPI layer provided by msp430_spi.c (MSP430 Value Line G2xx3 series) but can be custom-tailored to your application by editing ste2007.c and redefining the STE2007_SPI_TRANSFER9() and STE2007_CHIPSELECT() macros.  You may need to adjust the #include's in ste2007.c to make your SPI functions available.
- Advanced support for framebuffer-based text console and non-framebuffer-based text console is included in the terminal/ and terminal_lite/ subdirectories.
- Example main() programs included in each directory as test_*.c
- Examples based on bluehash @ 43oh.com's Nokia 1202 BoosterPack with P2.0 for the LCD's SPI Chip Select and P2.5 for the backlight LED.  BoosterPack modified to connect to a USCI_B port on the G2xx3 chips.
