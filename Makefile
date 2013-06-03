TARGETMCU	?= msp430g2553

CROSS		:= msp430-
CC		:= $(CROSS)gcc
MSPDEBUG	:= mspdebug
CFLAGS		:= -Os -Wall -Werror -g -mmcu=$(TARGETMCU)
CFLAGS += -fdata-sections -ffunction-sections -Wl,--gc-sections

SRCS			:= ste2007.c usci_spi.c
TEST_SIMPLE		:= test_simple.c

all:			test_simple.elf

test_simple.elf:	$(OBJS)
	$(CC) $(CFLAGS) -o test_simple.elf $(SRCS) $(TEST_SIMPLE)

install:	test_simple.elf
	mspdebug rf2500 "prog test_simple.elf"

clean:
	-rm -f *.elf
