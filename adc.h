/*
 * adc.h
 *
 * Created: 14.06.2013 18:54:45
 *  Author: Julian
 */ 


#ifndef ADC_H_
#define ADC_H_

#include <avr/io.h>

void adc_init(void);
uint16_t adc_read( uint8_t channel );
uint16_t adc_readAvg( uint8_t channel, uint8_t nsamples );
//a routine to calibrate the ADC measurement to 1V/Oct
void adc_calibration();


#endif /* ADC_H_ */