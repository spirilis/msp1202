/* msp430_stdio.h - Implementation of STDIO driver-level functions for
 *   the MSP430 Optimizing C/C++ compiler for CCS's STDIO implementation.
 *
    Copyright (c) 2021 Eric Brundick <spirilis [at] linux dot com>

    Permission is hereby granted, free of charge, to any person
    obtaining a copy of this software and associated documentation
    files (the "Software"), to deal in the Software without
    restriction, including without limitation the rights to use, copy,
    modify, merge, publish, distribute, sublicense, and/or sell copies
    of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be
    included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.
 */

#ifndef MSP430_STDIO_H
#define MSP430_STDIO_H
#include <msp430.h>
#include <stdio.h>
#include <stdlib.h>
#include <file.h>
#include <string.h>
#include "ste2007.h"
#include "msp430_spi.h"
#include "chargen.h"

/* Function prototypes for user functions - setting up the driver */
int MSP1202_use_as_stdout();  // This will register the driver & freopen(stdout) for you.
void MSP1202_register_driver();  // DO NOT use this if using MSP1202_use_as_stdout !!

/* Function prototypes for the low-level driver */

int MSP1202_open(const char *, unsigned, int);
int MSP1202_close(int);
int MSP1202_read(int, char *, unsigned);
int MSP1202_write(int, const char *, unsigned);
off_t MSP1202_lseek(int, off_t, int);
int MSP1202_unlink(const char *);
int MSP1202_rename(const char *, const char *);

#endif /* MSP430_STDIO_H */
