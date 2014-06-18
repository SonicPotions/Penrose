/*
 * spi.h
 *
 * Created: 14.06.2013 18:19:33
 *  Author: Julian
 */ 


#ifndef SPI_H_
#define SPI_H_

#include <avr/io.h>

void spi_init(void);
unsigned char SPI_transmit(unsigned char data);
void spi_enable(uint8_t onOff);

#endif /* SPI_H_ */