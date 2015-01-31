/*
 * adc.c
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
	ADMUX = (ADMUX & ~(0x1F)) | (0 & 0x1F); //always channel 0
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
uint16_t adc_read()
{
	ADCSRA |= (1<<ADSC);            // single readout
	while (ADCSRA & (1<<ADSC) ) {}  // wait to finish
	return ADCW;
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
