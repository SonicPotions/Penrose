/*
 * IoMatrix.c
 *
 * Created: 14.06.2013 21:07:52
 *  Author: Julian
 */ 
#include "IoMatrix.h"
#include <util/delay.h>
#include <avr/pgmspace.h> 


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
#define SWITCH_PORT_12		PORTB
#define SWITCH_INPUT_3		PINC
#define SWITCH_DDR_3		DDRC
#define SWITCH_PORT_3		PORTC


// ------------ LEDs ------------

#define LED_1_PIN		PC1
#define LED_2_PIN		PC2
#define	LED_3_PIN		PC3
#define LED_4_PIN		PD4
#define LED_5_PIN		PD5
#define LED_6_PIN		PD6

#define LED_PORT_13		PORTC
#define LED_DDR_13		DDRC

#define LED_PORT_46		PORTD
#define LED_DDR_46		DDRD

#define nop() \
   asm volatile ("nop")

/*
led	pin A	Pin B
0	1	2
1	1	3
2	1	4
3	2	3
4	2	4
5	2	5
6	3	4
7	3	5
8	3	6
9 	4	5
10	4	6
11	5	6
*/
static const uint8_t ledPinArray[12][2] PROGMEM = {
  {LED_1_PIN,LED_2_PIN},
  {LED_1_PIN,LED_3_PIN},
  {LED_1_PIN,LED_4_PIN},
  {LED_2_PIN,LED_3_PIN},
  {LED_2_PIN,LED_4_PIN},
  {LED_2_PIN,LED_5_PIN},
  {LED_3_PIN,LED_4_PIN},
  {LED_3_PIN,LED_5_PIN},
  {LED_3_PIN,LED_6_PIN},
  {LED_4_PIN,LED_5_PIN},
  {LED_4_PIN,LED_6_PIN},
  {LED_5_PIN,LED_6_PIN},
};
   
static uint16_t io_ledState=0x00;		//state of the 12 LEDs == activated notes
static uint8_t io_activeStep=0;			//current active quantisation step == currently played note
static uint16_t io_lastButtonState=0;
//-----------------------------------------------------------
void io_init()
{
  //all LED pins as inputs => off
  LED_DDR_13 &= ~(  (1<<LED_1_PIN) | (1<<LED_2_PIN) | (1<<LED_3_PIN)  );
  LED_DDR_46 &= ~(  (1<<LED_4_PIN) | (1<<LED_5_PIN) | (1<<LED_6_PIN)  );
  
  //all buttons rows as inputs
  SWITCH_DDR_12 &= ~(  (1<<SWITCH_ROW1_PIN) | (1<<SWITCH_ROW2_PIN) );
  SWITCH_DDR_3 &= ~(1<<SWITCH_ROW3_PIN);
  //row pullups on
  SWITCH_PORT_12 |= (1<<SWITCH_ROW1_PIN) | (1<<SWITCH_ROW2_PIN);
  SWITCH_PORT_3 |= (1<<SWITCH_ROW3_PIN);
  
  //all buttown columns as outputs, state low
  COL_DDR |= (1<<COL1_PIN) | (1<<COL2_PIN) | (1<<COL3_PIN) | (1<<COL4_PIN);
  COL_PORT &= ~((1<<COL1_PIN) | (1<<COL2_PIN) | (1<<COL3_PIN) | (1<<COL4_PIN));
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
void turnAllLedsOff()
{
  //all pins low / no pullup
  LED_PORT_13 &= ~( (1<<LED_1_PIN) | (1<<LED_2_PIN) | (1<<LED_3_PIN) );
  LED_PORT_46 &= ~( (1<<LED_4_PIN) | (1<<LED_5_PIN) | (1<<LED_6_PIN) );
  
  //all LED pins as inputs => off
  LED_DDR_13 &= ~(  (1<<LED_1_PIN) | (1<<LED_2_PIN) | (1<<LED_3_PIN)  );
  LED_DDR_46 &= ~(  (1<<LED_4_PIN) | (1<<LED_5_PIN) | (1<<LED_6_PIN)  );
}
//-----------------------------------------------------------
void turnLedOn(uint16_t ledNr, uint8_t colour)
{
  //get the needed pins from the LED array
  uint8_t pinA = pgm_read_byte(&ledPinArray[ledNr][0]);
  uint8_t pinB = pgm_read_byte(&ledPinArray[ledNr][1]);
  
  if(colour)
  {
    //colour 2 -> A=0, B=1
    if(pinA > LED_3_PIN)
    {
      LED_DDR_46 	|= (1<<pinA);	//pin as output
      LED_PORT_46	&= ~(1<<pinA);	//pin low
    }
    else
    {
      LED_DDR_13	|= (1<<pinA);
      LED_PORT_13	&= ~(1<<pinA);
    }
    
    if(pinB > LED_3_PIN)
    {
      LED_DDR_46	|= (1<<pinB);	//pin as output
      LED_PORT_46 	|= (1<<pinB);	//pin high
    }
    else
    {
      LED_DDR_13	|= (1<<pinB);
      LED_PORT_13	|= (1<<pinB);
    }
  }
  else
  {
    //colour 1 -> A out, B in
    if(pinA > LED_3_PIN)
    {
      LED_DDR_46 	|= (1<<pinA);	//pin as output
      LED_PORT_46	|= (1<<pinA);	//pin high
    }
    else
    {
      LED_DDR_13	|= (1<<pinA);
      LED_PORT_13	|= (1<<pinA);
    }
    
    if(pinB > LED_3_PIN)
    {
      LED_DDR_46	|= (1<<pinB);	//pin as output
      LED_PORT_46 	&= ~(1<<pinB);	//pin low
    }
    else
    {
      LED_DDR_13	|= (1<<pinB);
      LED_PORT_13	&= ~(1<<pinB);
    }    
  }
}
//-----------------------------------------------------------
//one circle trough the whole LED matrix
void io_processLed()
{
  for(int i=0; i<12; i++)
  {
    turnAllLedsOff();
    
    if(i==io_activeStep)
    {
      //this step is currently played => set color 1
      turnLedOn(i,0);
    } 
    else if ( (io_ledState & (1<<i)) > 0)
    {
      //step is active => colour 2
      turnLedOn(i,1);
    }
  }
};
//-----------------------------------------------------------
//read button matrix 
void io_processButtons()
{
	uint8_t col;
	uint8_t row;
	uint8_t i=0;
	uint16_t val;
	
	for(row=0;row<3;row++)
	{
	  for(col=0;col<4;col++)
	  {
		//all columns on
		COL_PORT |= ( (1<<COL1_PIN) | (1<<COL2_PIN) | (1<<COL3_PIN) | (1<<COL4_PIN) );
		//pin low for active column
		COL_PORT &= ~(1<<col);
		
		//read active row input
		switch(row)
		{
		  case 0:
		      val = (SWITCH_INPUT_12 & (1<<SWITCH_ROW1_PIN) ) != 0;
		      break;
		  
		  case 1:
		      val = (SWITCH_INPUT_12 & (1<<SWITCH_ROW2_PIN) ) != 0;
		      break;
		    
		  case 2:
		      val = (SWITCH_INPUT_3 & (1<<SWITCH_ROW3_PIN) ) != 0;
		      break;
		}
	    
		//check if the button changed its state since the last call
		if(   (io_lastButtonState&(1<<i))   != (val<<i)   )
		{
			//update state memory
			io_lastButtonState &= ~(1<<i);
			io_lastButtonState |=val<<i;
			//toggle LED
			if(val)
			{
			  if((io_ledState&(1<<i)))
			  {
			    io_ledState |= 1<<i;
			  } else 
			  {
			    io_ledState &= ~(1<<i);
			  }
			}
		}
		i++;
	  }
	}	
};
//-----------------------------------------------------------
