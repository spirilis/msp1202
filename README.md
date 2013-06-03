msp1202
=======

MSP430 library to support Nokia 1202 / STE2007 LCD displays

SYNOPSIS
  msp1202 includes the STE2007 library for primitive I/O to the Nokia 1202 LCD
  STE2007 library utilizes an SPI layer provided by usci_spi.cpp (MSP430 Value Line G2xx3 series) but can
    be custom-tailored to your application by editing ste2007.c and redefining the STE2007_SPI_TRANSFER9() and
    STE2007_CHIPSELECT() macros.

  Advanced support for framebuffer-based text console and non-framebuffer-based text console
    is included in the terminal/ and terminal_lite/ subdirectories.
  Example main() programs included in each directory as test_*.c

