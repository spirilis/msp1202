#ifndef LCD_PRINTF_H
#define LCD_PRINTF_H

/* Printf() derived from oPossum's code - http://forum.43oh.com/topic/1289-tiny-printf-c-version/
 * Doctored up to make GCC happy and function names converted to use an lcd_ prefix so they don't conflict
 * with MSPGCC's shipped libc.
 */
void lcd_printf(char *format, ...);

#endif
