/*
 * adc.c
 *
 * Created: 14.06.2013 18:55:08
 *  Author: Julian
 */ 

#include "adc.h"
#include <avr/interrupt.h>  
//-----------------------------------------------------------
void adc_init(void)
{
  
  DDRC &= ~(1<<PC0); //pin input
  PORTC &= ~(1<<PC0); //no pullup
  
	uint16_t result;
 	// AVCC as ref voltage
	ADMUX =  (1<<REFS0);
	ADMUX = (ADMUX & ~(0x1F)) | (0 & 0x1F);
 	// single conversion
	ADCSRA = (1<<ADPS2) |(1<<ADPS1)| (1<<ADPS0);     // adc prescaler (must be between 50 and 200kHz)
	ADCSRA |= (1<<ADEN);                  // ADC enable
 	// dummy readout
	ADCSRA |= (1<<ADSC);                  // single readout
	while (ADCSRA & (1<<ADSC) ) {}        // wait to finish
	// read result 
	result = ADCW;
};
//-----------------------------------------------------------
uint16_t adc_read( uint8_t channel )
{

	//select channel
//	ADMUX = (ADMUX & ~(0x1F)) | (channel & 0x1F);
	ADCSRA |= (1<<ADSC);            // single readout
	while (ADCSRA & (1<<ADSC) ) {}  // wait to finish
	return ADCW;                    // read and return result

};
//-----------------------------------------------------------
uint16_t adc_readAvg( uint8_t channel, uint8_t nsamples )
{
	uint32_t sum = 0;
	for (uint8_t i = 0; i < nsamples; ++i ) {
	sum += adc_read( channel );
	}
  	return (uint16_t)( sum / nsamples );
};
//-----------------------------------------------------------
