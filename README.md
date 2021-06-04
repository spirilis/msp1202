msp1202
=======

MSP430 library to support Nokia 1202 / STE2007 LCD displays

## SYNOPSIS

- msp1202 includes the STE2007 library for primitive I/O to the Nokia 1202 LCD
- STE2007 library utilizes an SPI layer provided by msp430_spi.c (MSP430 Value Line G2xx3 series) but can be custom-tailored to your application by editing ste2007.c and redefining the STE2007_SPI_TRANSFER9() and STE2007_CHIPSELECT() macros.  You may need to adjust the #include's in ste2007.c to make your SPI functions available.
- Advanced support for framebuffer-based text console and non-framebuffer-based text console is included in the terminal/ and terminal_lite/ subdirectories.
- Example main() programs included in ``archived`` directory as test_*.c
- Examples based on bluehash @ 43oh.com's Nokia 1202 BoosterPack with P2.0 for the LCD's SPI Chip Select and P2.5 for the backlight LED.  BoosterPack modified to connect to a USCI_B port on the G2xx3 chips.
- Support for MSP430 Optimizing C/C++ compiler's stdio library is provided in
  ``terminal/msp430_stdio.c`` and ``terminal/msp430_stdio.h``.  General use-case specified below.

## Organization

The source code files are organized in the main folder and subfolders based on feature set-

Core files:

    msp430_spi.c
    msp430_spi.h
    ste2007.c
    ste2007.h
    font_5x7.h

- The msp430_spi.* files contain drivers for SPI, and you may need to edit ``msp430_spi.h`` to
  define which peripheral (USCI_A, B, A0, A1, B0, B1, etc) you're using.
- The implementation is highly specific to the chip, not just the family, so many chips are not
  implemented.
- One of the trickiest pieces of the implementation is ``spi_transfer9()``, a function to perform
  9-bit SPI.  As most peripherals only support 8-bit SPI, this is implemented using a quick
  deconfiguration of the SPI mode for the SPI CLK, MOSI/SIMO, MISO/SOMI pins and manually driving
  SPI in mode #0 for the most-significant-bit, followed by reconfiguring the SPI mode of the
  GPIOs and using the standard 8-bit SPI transfer function.
- This ``spi_transfer9()`` function just happens to be critical for the Nokia 1202 LCD
  implementation, as it does not break out the D/C line into a separate GPIO.  Thus, the
  9th bit is necessary to indicate data vs. control frames.
- For each chip I have implemented in ``msp430_spi.c``, I have tried to implement all of the
  available USCI or eUSCI peripherals on their default GPIO pins.  Things may get dicey and you
  may have to write some code in here, or trash the msp430_spi.* files altogether and write
  your own custom implementation of the SPI driver if you need
  to remap the SPI peripheral pins using a chip that supports a pin mapper.
- The only functions strictly necessary for *ste2007.c*'s operation are:

```c
void spi_init();
uint8_t spi_transfer(uint8_t byte_to_transmit);
uint16_t spi_transfer9(uint16_t word_to_transmit);
```

- *uint8_t* is an alias for *unsigned char* on the MSP430, although *char* would probably work
  fine, it just might throw some compiler warnings.
- Likewise *uint16_t* is an alias for *unsigned int*, and *int* would work but it may throw
  some compiler warnings.
- The ``spi_transfer16()`` function is not utilized by *ste2007.c* so you wouldn't need to
  implement it.
- The default ``font_5x7.h`` is used by the higher-level driver functions in ``terminal`` and
  ``terminal_lite``
- Easter Egg!  The ``font_5x7.h`` font set includes 2 characters - 0x81 and 0x82 - when printed
  side-by-side on the LCD, it will display the [Texas Instruments](https://ti.com) logo ;-)
- Using this library for doing bitmap graphics would require writing directly for the *ste2007.c*
  code library, and ignoring the *terminal* or *terminal_lite* higher-level functions, which
  are explained below.

### terminal implementation

The files under ``terminal/`` subfolder include:

    chargen.c
    chargen.h
    lcd_printf.c
    lcd_printf.h
    msp430_stdio.c
    msp430_stdio.h

- You may use the "lcd_printf" or the "msp430_stdio" implementation, both are not strictly
  required.  You can also use the msp1202_* functions by themselves without those libraries,
  only utilizing the functions found in ``chargen.c``
- *chargen.c* provides a concept of a framebuffer, and primitive functions for writing characters
  to the LCD screen, using a 16x8 display size.  Handling of font bit-writes, keeping track of
  dirty characters (characters whose contents have changed), maintaining the concept of a "cursor"
  is all handled with this code.
- The implementation of an on-screen cursor may be disabled - or the cursor itself modified -
  by editing ``chargen.h`` and tweaking the #define's near the top.
- *lcd_printf* provides a basic implementation of printf derived from some codebase I've long
  since forgotten where I found it.  I've tested this with MSPGCC and TI's proprietary compiler,
  it works fine but lacks some features.
- *msp430_stdio* is a new implementation, for the TI proprietary *cl430* compiler only, and it
  implements the C STDIO interface.  This is explained 2 sections below in _Using the stdio implementation_

### terminal_lite implementation

The files under ``terminal_lite/`` subfolder include:

    chargen_nofb.c
    chargen_nofb.h
    lcd_printf.c
    lcd_printf.h

- This is similar to the *terminal* implementation explained in the previous section, except
  no framebuffer concept exists - characters are written or re-written on the display as needed.
- A cursor exists here and can be turned off by editing ``chargen_nofb.h``
- The same *lcd_printf* implementation as *terminal* exists here, but no *msp430_stdio* 
  implementation.  I figured the basic memory requirements for STDIO are high enough that tiny
  chips won't use it - and if you're using a chip with plenty of SRAM for STDIO, you can
  probably fit the small (144) byte framebuffer+dirtybit bitfield in your SRAM as well.
- There is no ``msp1202_flush()`` function in the *terminal_lite* implementation because there is
  no framebuffer or "dirty-bits" concept.  Every write commits to the screen right away.
- This implementation may be chattier and less performant than the *terminal* implementation but
  it reduces memory requirements for chips sporting smaller SRAM footprint.

---

## Using the stdio implementation

Copy the following files into your CCS project:

    msp430_spi.c
    msp430_spi.h
    ste2007.c
    ste2007.h
    font_5x7.h
    terminal/chargen.c
    terminal/chargen.h
    terminal/msp430_stdio.c
    terminal/msp430_stdio.h

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
and set the "*Heap size for C/C++ dynamic memory allocation*" to 256 or more.  More may be safer. 
This allocates memory out of SRAM, so be sure your chip has enough SRAM.  I have found 768
works fine with the MSP430FR2433 - but mind you - I haven't written anything intense enough to
fill up the SRAM on this chip yet (it has 4KB).  The value needs to be at least 256.
