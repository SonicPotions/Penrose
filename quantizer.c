/*
 * quantizer.c
 *
 * Created: 14.06.2013 15:53:19
 *  Author: Julian Schmidt
 */ 

#include <avr/io.h>

#include "MCP4802.h"
#include "adc.h"
#include "IoMatrix.h"
#include "eeprom.h"
#include "timebase.h"
#include <util/delay.h> 
#include <avr/interrupt.h>  


uint8_t quantizeValue(uint16_t input);
void gateOut(uint8_t onOff);

volatile uint8_t lastQuantValue = 0;
volatile uint8_t gateTimer = 0;

#define TRIGGER_INPUT_PIN		PD7
#define TRIGGER_INPUT_IN_PORT		PIND
#define TRIGGER_INPUT_PORT		PORTD

#define SWITCH_PIN			PC4
#define SWITCH_PORT			PORTC
#define SWITCH_IN_PORT			PINC
#define SWITCH_DDR			DDRC

#define INPUT_VOLTAGE			5.f	//Volt
#define OCTAVES				12.f	//octaves
#define VOLT_PER_OCTAVE			(INPUT_VOLTAGE/OCTAVES)
#define VOLT_PER_NOTE			(VOLT_PER_OCTAVE/12.f)
#define VOLT_PER_ADC_STEP		(INPUT_VOLTAGE/1024.f)
#define ADC_STEPS_PER_NOTE		(VOLT_PER_NOTE/VOLT_PER_ADC_STEP)


#define GATE_IN_CONNECTED (SWITCH_IN_PORT & (1<<SWITCH_PIN))
//-----------------------------------------------------------
void init()
{
    //switch is input with pullup
    SWITCH_DDR &= ~(1<<SWITCH_PIN);
    SWITCH_PORT |= (1<<SWITCH_PIN);

    //trigger is input with no pullup
    TRIGGER_INPUT_PORT &= ~(1<<TRIGGER_INPUT_PIN);

    timer_init();
    mcp4802_init();
    adc_init();
    io_init();

    /*
    Set up Interrupt for trigger input 

    PCINT0 to PCINT7 refer to the PCINT0 interrupt, PCINT8 to PCINT14 refer to the PCINT1 interrupt 
    and PCINT15 to PCINT23 refer to the PCINT2 interrupt
    -->
    TRIGGER_INPUT_PIN = PD7 = PCINT23 = pcint2 pin change interrupt for trigger
    */
    //interrupt trigger	(pin change)

    PCICR |= (1<<PCIE2);   //Enable PCINT2
    PCMSK2 |= (1<<PCINT23); //Trigger on change of PCINT23 (PD7)
    
    
    //read last button state from eeprom
    io_setActiveSteps( eeprom_ReadBuffer());
    
    sei();
}
//-----------------------------------------------------------
void process()
{
	const uint8_t quantValue = quantizeValue(adc_readAvg(0, 1));
	//if the value changed
	if(lastQuantValue != quantValue)
	{
		lastQuantValue = quantValue;
		mcp4802_outputData(quantValue,0);
		//start gate off timer
		timer0_start();
	}
}
//-----------------------------------------------------------
ISR(PCINT2_vect)
{
    if(bit_is_clear(PIND,7)) //only rising edge
    {
	process();	
    }	
    return;	
};
//-----------------------------------------------------------
uint8_t quantizeValue(uint16_t input)
{
	//quantize input value to all steps
	uint8_t quantValue = input/ADC_STEPS_PER_NOTE;
	//calculate the current active step
	const uint8_t octave = quantValue/12;
	uint8_t note = quantValue-(octave*12);
	
	//quantize to active steps
	//search for the lowest matching activated note (lit led)
	int i=0;
	for(;i<13;i++)
	{
	  if( ((1<<  ((note+i)%12) ) & io_getActiveSteps()) != 0) break;
	}
	
	if(i!=12)
	{
	  note = note+i;
	  note %= 12;
	}	
	
	quantValue = octave*12+note;
	
	//store to matrix
	io_setCurrentQuantizedValue(note);
	return quantValue*2;
	
}
//-----------------------------------------------------------
int main(void)
{
    init();

    while(1)
    {
	if( !GATE_IN_CONNECTED )
	{
	  //no gate cable plugged in
	  //continuous mode
	  process();
	}		
		
	//handle IOs (buttons + LED)		
	io_processButtons();
	io_processLed();	
	checkAutosave();

    }
}
//-----------------------------------------------------------
