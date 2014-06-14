/*
 * IoMatrix.c
 *
 * Created: 14.06.2013 21:07:52
 *  Author: Julian
 */ 
#include "IoMatrix.h"
#include <util/delay.h>


// ---------- Switches ----------
#define COL1_PIN		PD0
#define COL2_PIN		PD1
#define COL3_PIN		PD2
#define COL4_PIN		PD3

#define COL_PORT		PORTD
#define COL_DDR			DDRD

#define SWITCH_ROW1_PIN		PB2
#define SWITCH_ROW2_PIN		PB4
#define SWITCH_ROW3_PIN		PC5

#define SWITCH_INPUT_12		PINB
#define SWITCH_DDR_12		DDRB
#define SWITCH_INPUT_3		PINC
#define SWITCH_DDR_3		DDRC


// ------------ LEDs ------------

#define LED_1_PIN		PC1
#define LED_2_PIN		PC2
#define LED_3_PIN		PC3
#define LED_4_PIN		PD4
#define LED_5_PIN		PD5
#define LED_6_PIN		PD6

#define LED_PORT_13		PORTC
#define LED_DDR_13		DDRC

#define LED_PORT_46		PORTD
#define LED_DDR_46		DDRD

#define nop() \
   asm volatile ("nop")

static uint16_t io_ledState=0x00;		//state of the 12 LEDs == activated notes
static uint8_t io_activeStep=0;			//current active quantisation step == currently played note
static uint16_t io_lastButtonState=0;
//-----------------------------------------------------------
void io_init()
{
  LED_DDR |= (  (1<<LED_ROW1_PIN) | (1<<LED_ROW2_PIN) | (1<<LED_ROW3_PIN)  );
};
//-----------------------------------------------------------
uint16_t io_getActiveSteps()
{
  return io_ledState;
}
//-----------------------------------------------------------
void io_setActiveSteps(uint16_t val)
{
  io_ledState = val;
}
//-----------------------------------------------------------
void io_setCurrentQuantizedValue(uint8_t value)
{
  io_activeStep = value;
}
//-----------------------------------------------------------
void io_setAllLedPins(uint8_t onOff)
{
	if(onOff)
	//if(0)
	{
		//all pins on
		LED_PORT |= ( (1<<LED_ROW1_PIN) | (1<<LED_ROW2_PIN) | (1<<LED_ROW3_PIN) );
	} 
	else
	{
		//all pins off
		LED_PORT &= ~( (1<<LED_ROW1_PIN) | (1<<LED_ROW2_PIN) | (1<<LED_ROW3_PIN) );
	}		
}
//-----------------------------------------------------------
void io_setAllColPins(uint8_t onOff)
{
	if(onOff)
	{
		//all pins on
		COL_PORT |= ( (1<<COL1_PIN) | (1<<COL2_PIN) | (1<<COL3_PIN) | (1<<COL4_PIN) ); //set all col pins
	} 
	else
	{
		//all pins off
		COL_PORT &= ~( (1<<COL1_PIN) | (1<<COL2_PIN) | (1<<COL3_PIN) | (1<<COL4_PIN) ); //clear all col pins
	}		
}
//-----------------------------------------------------------
void io_setAllRowsHighZ()
{
	//DDR as input
	LED_DDR &= ~(  (1<<LED_ROW1_PIN) | (1<<LED_ROW2_PIN) | (1<<LED_ROW3_PIN) );
	//No Pullup
	LED_PORT &= ~( (1<<LED_ROW1_PIN) | (1<<LED_ROW2_PIN) | (1<<LED_ROW3_PIN) );
}
//-----------------------------------------------------------
void io_setAllColHighZ()
{
	//DDR as input
	COL_DDR &= ~( (1<<COL1_PIN) | (1<<COL2_PIN) | (1<<COL3_PIN) | (1<<COL4_PIN) );
	//No Pullup
	COL_PORT &= ~( (1<<COL1_PIN) | (1<<COL2_PIN) | (1<<COL3_PIN) | (1<<COL4_PIN) );
}
//-----------------------------------------------------------
//one circle trough the whole LED matrix
void io_processLed()
{
	uint8_t col;
	uint8_t row;
	//--- current quantized note color 1 ---
	
	/*
	led	row	col
	0	0	0
	1	1	0
	2	2	0
	3	0	1
	4	1	1
	5	2	1
	6	0	2
	7	1	2
	8	2	2
	9 	0	3
	10	1	3
	11	2	3
	*/
	
	row = io_activeStep%3;
	col = io_activeStep/3;


#if 1	
	//LED_DDR |= (  (1<<LED_ROW1_PIN) | (1<<LED_ROW2_PIN) | (1<<LED_ROW3_PIN)  );
	io_setAllRowsHighZ();
	io_setAllColHighZ();
	//io_setAllLedPins(1);
	//selected column is low
	COL_DDR |= (1<<(COL1_PIN+col));		//selected col pin as output
	COL_PORT &= ~(1<<(COL1_PIN+col));	//selected col is high
	
	//selected row is hi
	LED_DDR |=   (1<<(LED_ROW1_PIN+row));
	LED_PORT |= (1<<(LED_ROW1_PIN + row));
	_delay_ms(1);
	
#endif
	//--- active notes color 2 ---
	io_setAllRowsHighZ();
	io_setAllLedPins(0);
	//LED_DDR |= (  (1<<LED_ROW1_PIN) | (1<<LED_ROW2_PIN) | (1<<LED_ROW3_PIN)  );
	//io_setAllLedPins(0);
	
	
	//alle cols output
	//COL_DDR |= ( (1<<COL1_PIN) | (1<<COL2_PIN) | (1<<COL3_PIN) | (1<<COL4_PIN) );
	io_setAllColHighZ();
	io_setAllColPins(0);
	
	
	for(int i=0;i<12;i++)
	{
		//if step is activated
		if(io_ledState & (1<<i))
		{
			row = i%3;
			col = i/3;
	
			

			//LED_DDR |= (  (1<<LED_ROW1_PIN) | (1<<LED_ROW2_PIN) | (1<<LED_ROW3_PIN)  );
			
			//selected column is high
			COL_DDR |=  (1<<(COL1_PIN+col));
			COL_PORT |= (1<<(COL1_PIN+col));	//selected col is low
			//selected row is low
			LED_DDR |= 1<<(LED_ROW1_PIN + row);
			LED_PORT &= ~(1<<(LED_ROW1_PIN + row));
			_delay_ms(1);
			
			//io_setAllColPins(0);
			io_setAllColHighZ();
			
			io_setAllRowsHighZ();
			io_setAllLedPins(0);
			//_delay_ms(500);
		}
	
				
	}		
	
		io_setAllColHighZ();
			
			io_setAllRowsHighZ();
			io_setAllLedPins(0);
//	LED_DDR |= (  (1<<LED_ROW1_PIN) | (1<<LED_ROW2_PIN) | (1<<LED_ROW3_PIN)  );
//	COL_DDR |= (1<<(COL1_PIN)) | (1<<(COL2_PIN)) | (1<<(COL3_PIN)) | (1<<(COL4_PIN)) ;		//selected col pin as output
	
//	io_setAllLedPins(1);
//	io_setAllColPins(0);
};
//-----------------------------------------------------------
//read button matrix 
void io_processButtons()
{
	
	uint8_t col;
	uint8_t row;
	
	SWITCH_DDR &= ~(1<<SWITCH_ROW1_PIN) | (1<<SWITCH_ROW2_PIN) | (1<<SWITCH_ROW3_PIN);
	
	io_setAllColHighZ();
	//COL_DDR |= (1<<(COL1_PIN)) | (1<<(COL2_PIN)) | (1<<(COL3_PIN)) | (1<<(COL4_PIN)) ;
	io_setAllColPins(1);
	
	io_setAllRowsHighZ();
	
	
	for(int i=0;i<12;i++)
	{
		row = i%3;
		col = i/3;
		//selected column low
		COL_DDR |=  (1<<(COL1_PIN+col));
		COL_PORT &= ~(1<<(COL1_PIN+col));
		
		
		nop();
		nop();
		nop();
	//	_delay_us(1);
		//read selected row
		uint16_t val = (  SWITCH_INPUT&(1<<(SWITCH_ROW1_PIN+row))  ) == 0;
				//SWITCH_DDR |= (1<<SWITCH_ROW1_PIN) | (1<<SWITCH_ROW2_PIN) | (1<<SWITCH_ROW3_PIN);
		//io_setAllColHighZ();
			//COL_DDR |= (1<<(COL1_PIN)) | (1<<(COL2_PIN)) | (1<<(COL3_PIN)) | (1<<(COL4_PIN)) ;
	io_setAllColPins(1);
	
	
		//val = 1-val;
		//val = val << i;
		
		//continue;
		
		if(   (io_lastButtonState&(1<<i))   != val<<i   )
		{
			io_lastButtonState &= ~(1<<i);
			io_lastButtonState |=val<<i;
			//toggle state
			if(val)
			{
			  if(!(io_ledState&(1<<i)))
			  {
				  io_ledState |= 1<<i;
			  } else {
				  io_ledState &= ~(1<<i);
			  }
			}
		}	
		//	_delay_ms(1);
		

	
				
	}		
};
