TARGETMCU	?= msp430g2553

CROSS		:= msp430-
CC		:= $(CROSS)gcc
MSPDEBUG	:= mspdebug
CFLAGS		:= -Os -Wall -Werror -g -mmcu=$(TARGETMCU)
CFLAGS += -fdata-sections -ffunction-sections -Wl,--gc-sections

SRCS			:= ste2007.c chargen.c lcd_printf.c
USCI_SPI_G2		:= usci_spi.c
USCI_SPI_F5		:= msp430_spi.c
TEST_TERMINAL		:= test_terminal.c
TEST_PRINTF		:= test_printf.c
TEST_PUSHBTN		:= test_pushbtn.c
TEST_G2955		:= test_g2955.c
TEST_F5529		:= test_f5529.c clockinit.c vcore.c

all:			test_terminal.elf

test_terminal.elf:	$(OBJS)
	$(CC) $(CFLAGS) -o test_terminal.elf $(SRCS) $(USCI_SPI_G2) $(TEST_TERMINAL)

test_printf.elf:	$(OBJS)
	$(CC) $(CFLAGS) -o test_printf.elf $(SRCS) $(USCI_SPI_G2) $(TEST_PRINTF)

test_pushbtn.elf:	$(OBJS)
	$(CC) $(CFLAGS) -o test_pushbtn.elf $(SRCS) $(USCI_SPI_G2) $(TEST_PUSHBTN)

test_g2955.elf:		$(OBJS)
	$(CC) $(CFLAGS) -mmcu=msp430g2955 -o test_g2955.elf $(SRCS) $(USCI_SPI_G2) $(TEST_G2955)

test_f5529.elf:		$(OBJS)
	$(CC) $(CFLAGS) -mmcu=msp430f5529 -mmemory-model=huge -o test_f5529.elf $(SRCS) $(USCI_SPI_F5) $(TEST_F5529)

install:	test_terminal.elf
	mspdebug rf2500 "prog test_terminal.elf"

clean:
	-rm -f *.elf
