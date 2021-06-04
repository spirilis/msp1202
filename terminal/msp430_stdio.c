/* msp430_stdio.c - Implementation of STDIO driver-level functions for
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

#include "msp430-stdio.h"

/* Implemented functions for Nokia 1202 LCD output using printf() et al */

static int msp1202_filedes = 0;

/// Register the MSP1202 driver as "msp1202"
void MSP1202_register_driver()
{
   add_device("msp1202", _MSA,
      MSP1202_open,
      MSP1202_close,
      MSP1202_read,
      MSP1202_write,
      MSP1202_lseek,
      MSP1202_unlink,
      MSP1202_rename);
}

/// Helper function to register the msp1202 driver & freopen stdout
/// @param[out] Returns 0 if successful, -1 if failed
int MSP1202_use_as_stdout()
{
   MSP1202_register_driver();
   if (!freopen("msp1202:", "w", stdout)) {
      return -1;
   }
   // Attempt to configure stdout for line-buffering.
   // If this fails, we don't really care.
   setvbuf(stdout, NULL, _IOLBF, 0);

   return 0;
}

/// Initialize the LCD and clear the screen
int MSP1202_open(const char *path, unsigned flags, int llv_fd)
{
   if (flags == O_RDONLY) {
      return -1;  // Invalid mode for this device
   }
   
   spi_init();
   msp1202_init();

   if (strlen(path) > 0) {
      int i;
      i = atoi(path);
      if (i >= 0 && i < 32) {
         // Interpret the "path" as a sharpness/contrast specification for the display
         ste2007_contrast(i);
      }
   }
   
   msp1202_filedes = 1;  // stdout
   return msp1202_filedes;
}

/// There isn't really anything to do here; just keep track that the user has
/// provided a valid file descriptor.
int MSP1202_close(int dev_fd)
{
   if (dev_fd != msp1202_filedes)
      return -1;
   return 0;
}

/// Re-implement msp1202_puts() using the supplied count variable
int MSP1202_write(int dev_fd, const char *buf, unsigned count)
{
   if (dev_fd == msp1202_filedes) {
   	unsigned i;

      for (i=0; i < count; i++)
         msp1202_putc((unsigned char)buf[i], 0);
      msp1202_flush();
      return count;
   }
   return -1;  // Invalid file descriptor specified
}

/* Unimplementable functions below - all return -1 */
int MSP1202_read(int dev_fd, char *buf, unsigned count)
{
   return -1;
}

off_t MSP1202_lseek(int dev_fd, off_t offset, int origin)
{
   return -1;
}

int MSP1202_unlink(const char *path)
{
   return -1;
}

int MSP1202_rename(const char *old_name, const char *new_name)
{
   return -1;
}
