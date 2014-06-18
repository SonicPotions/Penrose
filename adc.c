/*
 * adc.c
 *
 * Created: 14.06.2013 18:55:08
 *  Author: Julian
 */ 

#include "adc.h"
#include "IoMatrix.h"
#include "eeprom.h"
#include <avr/interrupt.h>  

// for a 5V range with 10bit adc resolution
// we get 5V/1024 = 0,0048828125V per step
// that is 1V/0,0048828125V = 204,8 Steps per Volt
// so the calculated ideal values for the ADC mesurement are
#define IDEAL_1V_CODE 205 // 204,8 * 1V = 204,8
#define IDEAL_3V_CODE 614 // 204,8 * 3V = 614,4

static int16_t adc_offset;
static uint16_t adc_gain;
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
	
	
	adc_offset = eeprom_getCalibrationOffset();
	adc_gain = eeprom_getCalibrationGain();
};
//-----------------------------------------------------------
uint16_t adc_getCalibratedValue(uint16_t input)
{
  //adc_gain is a 15-bit unsigned int value
  //adc_offset is int16_t value
  //so we have a uint16_t/uint15_t divission
  return (((uint32_t)(input-adc_offset))<<15) / adc_gain;
}
//-----------------------------------------------------------
uint16_t adc_read_direct()
{
	ADCSRA |= (1<<ADSC);            // single readout
	while (ADCSRA & (1<<ADSC) ) {}  // wait to finish
	return (ADCW);	        // read result 
	//if(val>=1)return val;
	//return val;            
};
//-----------------------------------------------------------
uint16_t adc_read()
{
	ADCSRA |= (1<<ADSC);            // single readout
	while (ADCSRA & (1<<ADSC) ) {}  // wait to finish
	return ADCW;//adc_getCalibratedValue(ADCW);	        // read result 
	//if(val>=1)return val;
	//return val;            
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
/* CV input calibration Routine (1V/Oct)
 * 
 * The user has to input a 1V signal to the CV jack (From midi2CV interface or precission voltage source)
 * push button 1 to confirm
 * Then input a 3V signal to the CV jack
 * push button 1 again
 * push button 1 again to confirm
 * finished.
 * */
void adc_calibration()
{
  uint16_t value_1V=0;
  uint16_t value_3V=0;
  uint8_t toggle = 0;
  uint16_t gain;
  int16_t offset;
  //light 1st LED yellow to indicate 1V calibration mode and wait for button 1
  while(!io_isButton1Pushed(0))
  {
    //toggle 1st LED between red/green
    if(toggle)
    {
      io_setActiveSteps(1);
      io_setCurrentQuantizedValue(99); //no led
    }else{
      io_setActiveSteps(0);
      io_setCurrentQuantizedValue(0); //1st led
    }
    toggle = 1-toggle; 
    
    io_processButtons();
    io_processLed();
  }
  
  //store 1V calibration value
  value_1V = adc_read_direct();
  
  //light 3rd LED yellow to indicate 3V calibration mode and wait for button 3
  while(!io_isButton1Pushed(2))
  {
    //toggle 3rd LED between red/green
    if(toggle)
    {
      io_setActiveSteps(0x4);//3rd led
      io_setCurrentQuantizedValue(99); //no led
    }else{
      io_setActiveSteps(0); //no led
      io_setCurrentQuantizedValue(2); //3rd led
    }
    toggle = 1-toggle; 
    
    io_processButtons();
    io_processLed();
  }

  //store 3V calibration value
  value_3V = adc_read_direct();
  
  //calculate offset and gain - too lazy for integer math today :-p
  float fracGain = (value_3V - value_1V) / (float)(IDEAL_3V_CODE-IDEAL_1V_CODE);
  gain = fracGain * 0x7fff; //gain is 15-bit uint
  offset = value_3V - fracGain*IDEAL_3V_CODE;
  
  //save gain and offset to eeprom
  eeprom_saveCalibrationData(gain, offset);
 
}
//-----------------------------------------------------------
