msp1202
=======

MSP430 library to support Nokia 1202 / STE2007 LCD displays

## SYNOPSIS
- msp1202 includes the STE2007 library for primitive I/O to the Nokia 1202 LCD
- STE2007 library utilizes an SPI layer provided by msp430_spi.c (MSP430 Value Line G2xx3 series) but can be custom-tailored to your application by editing ste2007.c and redefining the STE2007_SPI_TRANSFER9() and STE2007_CHIPSELECT() macros.  You may need to adjust the #include's in ste2007.c to make your SPI functions available.
- Advanced support for framebuffer-based text console and non-framebuffer-based text console is included in the terminal/ and terminal_lite/ subdirectories.
- Example main() programs included in each directory as test_*.c
- Examples based on bluehash @ 43oh.com's Nokia 1202 BoosterPack with P2.0 for the LCD's SPI Chip Select and P2.5 for the backlight LED.  BoosterPack modified to connect to a USCI_B port on the G2xx3 chips.
- Support for MSP430 Optimizing C/C++ compiler's stdio library is provided in
  ``terminal/msp430_stdio.c`` and ``terminal/msp430_stdio.h``.  General use-case specified below.

---

## Using the stdio implementation

Copy the following files into your CCS project:

    msp430_spi.c
    msp430_spi.h
    ste2007.c
    ste2007.h
    terminal/chargen.c
    terminal/chargen.h
    terminal/msp430_stdio.c
    terminal/msp430_stdio.h
    terminal/font_5x7.h

Edit ste2007.h and provide a correct function name for the ChipSelect setting implementation; this
is critical to the MSP1202 library's implementation:

    // Chip Select line drive; takes 0 or 1 to set the CS line low or high
    #define STE2007_CHIPSELECT(x) test_chipselect(x)
    void test_chipselect(uint8_t);  // Declared in the main .c file we'll be using

Replace ``test_chipselect`` with a different function you supply in your code.  
This function needs to take a single unsigned char argument, which is 0 or 1.  
If it's 0, the requisite Chip Select GPIO for the LCD should be switched off.  
If it's 1, the Chip Select GPIO should be switched on.

See ``msp430_stdio.h`` for the function prototypes.  You will typically use
``MSP1202_use_as_stdio()`` in your code.  This will register the driver and *freopen()*
the *stdout* data stream.  As this occurs, the library will call:

    spi_init();
    msp1202_init();

The screen will go blank and a cursor will show up in the upper left corner of the LCD.
Note the contrast control will be set to 16 (range: 0-31).  If you wish to use a different
contrast, you can do this 2 ways:

1. Use the *ste2007_contrast()* function to change the contrast
2. Rerun *freopen()* like such:

``freopen("msp1202:<contrast value>", "w", stdout);``

e.g. ``freopen("msp1202:31", "w", stdout);``

Upon completion of the driver registration/initialization, you should be able to do:

``printf("Hello, world!\n");``

and see Hello, world! at the top of the display with the cursor at the left of the 2nd line.

## GOTCHAS when using stdio

The C linker's heap size setting needs to be extended - the printf() implementation et al
requires at least 256 bytes, while the default (for the MSP430FR2433, anyhow) is 160.

Go to the CCS project's
Properties > *Build* > *MSP430 Linker* > *Basic Options*
and set the "*Heap size for C/C++ dynamic memory allocation*" to 256 or more.  More may be safer.This allocates memory out of SRAM, so be sure your chip has enough SRAM.  I have found 768
works fine with the MSP430FR2433 - but mind you - I haven't written anything intense enough to
fill up the SRAM on this chip yet (it has 4KB).  The value needs to be at least 256.
