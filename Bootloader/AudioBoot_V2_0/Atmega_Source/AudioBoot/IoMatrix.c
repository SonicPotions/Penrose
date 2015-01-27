/*
 *  Sonic Potions Quantizer Module 
 *  LED Matrix Functions
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

 

#include "IoMatrix.h"
#include <util/delay.h>
#include <avr/pgmspace.h> 

//-----------------------------------------------------------
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
//-----------------------------------------------------------
void io_init()
{
  //all LED pins as inputs => off
  LED_DDR_13 &= ~(  (1<<LED_1_PIN) | (1<<LED_2_PIN) | (1<<LED_3_PIN)  );
  LED_DDR_46 &= ~(  (1<<LED_4_PIN) | (1<<LED_5_PIN) | (1<<LED_6_PIN)  );
  
  //all buttons rows as inputs (attention SS pin used!)
  SWITCH_DDR_12 &= ~(  (1<<SWITCH_ROW1_PIN) | (1<<SWITCH_ROW2_PIN) );
  SWITCH_DDR_3 &= ~(1<<SWITCH_ROW3_PIN);
  //pullup on
  SWITCH_PORT_12 |= (1<<SWITCH_ROW1_PIN) | (1<<SWITCH_ROW2_PIN);
  SWITCH_PORT_3 |= (1<<SWITCH_ROW3_PIN);
  
  //all buttown columns as outs, state high
  COL_DDR |= (1<<COL1_PIN) | (1<<COL2_PIN) | (1<<COL3_PIN) | (1<<COL4_PIN);
  COL_PORT |= ((1<<COL1_PIN) | (1<<COL2_PIN) | (1<<COL3_PIN) | (1<<COL4_PIN));
  
  DDRB |= (1<<PB1); //gate = output
  PORTB |= (1<<PB1);
};

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
void turnLedOn(uint8_t colour)
{
  uint8_t pinA = LED_1_PIN;
  uint8_t pinB = LED_2_PIN;
  
  if(colour)
  {
      LED_DDR_13	|= (1<<pinA);
      LED_PORT_13	&= ~(1<<pinA);
    
      LED_DDR_13	|= (1<<pinB);
      LED_PORT_13	|= (1<<pinB);
  }
  else
  {
    //colour 1 -> A out, B in
      LED_DDR_13	|= (1<<pinA);
      LED_PORT_13	|= (1<<pinA);

      LED_DDR_13	|= (1<<pinB);
      LED_PORT_13	&= ~(1<<pinB);
   
  }
}
//-----------------------------------------------------------
uint8_t io_isButtonPushed() //checks button 0
{
  const uint8_t col = 0;

  //all columns on
  COL_PORT |=  ((1<<COL2_PIN) | (1<<COL3_PIN) | (1<<COL4_PIN) );
  //pin low for active column
  COL_PORT &= ~(1<<col);
  
  //read active row input
  uint8_t val = 0;

  val = (SWITCH_INPUT_12 & (1<<SWITCH_ROW1_PIN) ) == 0;
  
  return val;
}
//-----------------------------------------------------------

