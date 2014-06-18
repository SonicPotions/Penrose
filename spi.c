/*
 * spi.c
 *
 * Created: 14.06.2013 18:19:44
 *  Author: Julian
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
