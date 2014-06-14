/*
 * quantizer.c
 *
 * Created: 14.06.2013 15:53:19
 *  Author: Julian
 */ 

#include <avr/io.h>

#include "MCP4802.h"
#include "adc.h"
#include "IoMatrix.h"
#include <util/delay.h> 
#include <avr/interrupt.h>  


uint8_t quantizeValue(uint16_t input);
void gateOut(uint8_t onOff);

volatile uint8_t lastQuantValue = 0;
volatile uint8_t gateTimer = 0;
uint8_t lastTriggerValue = 0;

#define TRIGGER_INPUT_PIN		PD7
#define TRIGGER_INPUT_IN_PORT		PIND
#define TRIGGER_INPUT_PORT		PORTD

#define SWITCH_PIN			PC4
#define SWITCH_PORT			PORTC
#define SWITCH_IN_PORT			PINC
#define SWITCH_DDR			DDRC

//#define GET_SWITCH			(SWITCH_IN_PORT & (1<<SWITCH_PIN))

#define TRIGGER_ACTIVE			(  (TRIGGER_INPUT_IN_PORT & (1<<TRIGGER_INPUT_PIN))==0 )

//#define GATE_OUT_PIN			PC5
//#define GATE_OUT_PORT			PORTC


#define INPUT_VOLTAGE			5.f	//Volt
#define OCTAVES				12.f	//octaves
#define VOLT_PER_OCTAVE			(INPUT_VOLTAGE/OCTAVES)
#define VOLT_PER_NOTE			(VOLT_PER_OCTAVE/12.f)
#define VOLT_PER_ADC_STEP		(INPUT_VOLTAGE/1024.f)
#define ADC_STEPS_PER_NOTE		(VOLT_PER_NOTE/VOLT_PER_ADC_STEP)


uint8_t readSwitch()
{
	uint8_t ret =0;
	
	
	SPCR &= ~(1<<SPE); //disable SPI
	
		//switch input
	SWITCH_DDR &= ~(1<<SWITCH_PIN);
	//pull-up an
	SWITCH_PORT |= (1<<SWITCH_PIN);
	
	_delay_ms(1);
	
	ret = (SWITCH_IN_PORT & (1<<SWITCH_PIN)) ;
	
	
	//output to zero!!! (because switch can connect it to gnd)
	SWITCH_PORT &= ~(1<<SWITCH_PIN);
	//switch output (for dumb avr spi master problem with SS pin)
	SWITCH_DDR |= (1<<SWITCH_PIN);
	
	//spi_init();
	SPCR |= (1<<SPE);
	
	return ret;
	
}

void init()
{
	
  /*
	//switch 
	//output to zero!!! (because switch can connect it to gnd)
	SWITCH_PORT &= ~(1<<SWITCH_PIN);
	//switch output (for dumb avr spi master problem with SS pin)
	SWITCH_DDR |= (1<<SWITCH_PIN);
	*/
  
  	//switch input
	SWITCH_DDR &= ~(1<<SWITCH_PIN);
	//pull-up an
	SWITCH_PORT |= (1<<SWITCH_PIN);
	
	mcp4802_init();
	//SPCR &= ~(1<<SPE); //disable SPI -> switch ss input problem
	
	adc_init();
	io_init();
	
	//gate out
	DDRC |= (1<<PC5); //output
	PORTC &= ~(1<<PC5);
	
	
	
	//trigger in
	TRIGGER_INPUT_PORT &= ~(1<<TRIGGER_INPUT_PIN);
	//TRIGGER_INPUT_PORT |= (1<<TRIGGER_INPUT_PIN);
	

	
	/*
	PCINT0 to PCINT7 refer to the PCINT0 interrupt, PCINT8 to PCINT14 refer to the PCINT1 interrupt 
	and PCINT15 to PCINT23 refer to the PCINT2 interrupt
	-->
	TRIGGER_INPUT_PIN = PD7 = PCINT23 = pcint2 pin change interrupt for trigger
	*/
	//interrupt trigger	(pin change)
	
	PCICR |= (1<<PCIE2);   //Enable PCINT2
    PCMSK2 |= (1<<PCINT23); //Trigger on change of PCINT23 (PD7)
    sei();
	


	

}

void process()
{
	const uint8_t quantValue = quantizeValue(adc_readAvg(0, 1));
	//if the value changed
	if(lastQuantValue != quantValue)
	{
		lastQuantValue = quantValue;
		mcp4802_outputData(0,quantValue);
		gateOut(1);
		gateTimer=0;
	}
}

ISR(SIG_PIN_CHANGE2)
{
	//if(switchValue) return;
	if(bit_is_clear(PIND,7)) //only rising edge
    {
		process();	
	//	gateOut(1);
	//	gateTimer=0;
	}	
	return;	
};





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
		if( ((1<<  ((note+i)%12) ) & io_ledState) != 0) break;
	}
	
	if(i!=12)
	{
		note = note+i;
		note %= 12;
	}	
	
	
	
	quantValue = octave*12+note;
	
	//store to matrix
	io_activeStep = note;
	return quantValue*2;
	
}

//controls the gate out jack
void gateOut(uint8_t onOff)
{
	//onOff?
	if(onOff)
	{
		PORTC |= (1<<PC5);
	} else {
		PORTC &= ~(1<<PC5);
	}
	
}





int main(void)
{
	init();

    while(1)
    {
		uint8_t switchValue = 0;//readSwitch();
		
		for(int i=0;i<20;i++)
		{
			
			//output new value if enable pin is high
			uint8_t triggerValue = TRIGGER_ACTIVE;
		
		/*
			if( ((switchValue) && (lastTriggerValue != triggerValue)) || (!switchValue)   )
			{
				lastTriggerValue = triggerValue;
				if(!triggerValue)
				{
					process();

				}	
			}		
			*/
				
			//handle IOs (buttons + LED)		
			io_tickButtons();
			io_tickLed();
		
			//turn off gate again
			//would be better in a fixed timer routine for fixed gate length
			if(gateTimer>=10)
			{
				gateOut(0);
			} else
			{
				gateTimer++;
			}						
		}			
    }
}