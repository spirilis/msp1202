/* USCI SPI driver ripped from msprf24 and with spi_transfer9() added. */


#ifndef USCI_SPI_H
#define USCI_SPI_H


#include <stdint.h>

// SPI driver needs to provide these
void spi_init();
uint8_t spi_transfer(uint8_t);      // SPI xfer 1 byte
uint16_t spi_transfer16(uint16_t);  // SPI xfer 2 bytes
uint16_t spi_transfer9(uint16_t);   // SPI xfer 1 bit + 1 byte



#endif
