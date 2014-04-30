/* Clockinit - MSP430 F5xxx series UCS clock initialization
 * Tailored for the F51xx series chips
 */
#ifndef CLOCKINIT_H
#define CLOCKINIT_H

#include <stdint.h>

uint16_t ucs_clockinit(unsigned long freq, uint16_t use_xt1, uint16_t vlo_as_aclk);

#endif
