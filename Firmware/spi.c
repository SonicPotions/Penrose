/*
 * spi.c
 *
 *  Copyright 2015 Julian Schmidt, Sonic Potions <julian@sonic-potions.com>
 *  Web: www.sonic-potions.com/penrose
 * 
 *  This file is part of the Penrose Quantizer Firmware.
 *
 *  The Penrose Quantizer Firmware is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  The Penrose Quantizer Firmware is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with the Penrose Quantizer Firmware.  If not, see <http://www.gnu.org/licenses/>.
 */ 

#include "spi.h"

#define SPI_PORT	PORTB
#define SPI_DDR		DDRB
#define MOSI_PIN	PB3
#define SCK_PIN		PB5

//-----------------------------------------------------------
void spi_init(void)
{
	//init port
	SPI_DDR  |= (1<<MOSI_PIN) | (1<<SCK_PIN) ;//MOSI+SCK as output
	SPCR =  (1<<MSTR) | (1<<SPR1); //clock FCPU/64 = 20000000/64= 312.5kHz
	SPSR = 0x00;
	/* SPI is not enabled here
	 * it is just enabled before sending out data to the DAC
	 * since the MISO and SS pins are also used for the button matrix
	 * a continuous activated SPI port would conflic with the matrix and break functionality
	 * */
};
//-----------------------------------------------------------
unsigned char SPI_transmit(unsigned char data)
{
	// Start transmission
	SPDR = data;

	// Wait for transmission complete
	while(!(SPSR & (1<<SPIF)));
	data = SPDR;

	return(data);
};
//-----------------------------------------------------------
void spi_enable(uint8_t onOff)
{
  if(onOff)
  {
    SPCR |= (1<<SPE);
  } else {
    SPCR &= ~(1<<SPE);    
  }
}
//-----------------------------------------------------------
