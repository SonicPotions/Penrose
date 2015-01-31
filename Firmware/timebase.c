/*
 * timebase.c
 *
 * contains timer functions for autosave and gate length
 * 
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


#include "timebase.h"
#include "MCP4802.h"
#include "eeprom.h"
#include "IoMatrix.h"

static volatile uint8_t autosave_counter = 0;
static volatile uint8_t autosave_flag = 1; //0 save needed, 1 no save needed
//-----------------------------------------------------------
ISR (TIMER0_COMPA_vect)
{
  //stop timer
  TCCR0B &= ~((1<<CS02) | (1<<CS00));
  //reset gate -> LDAC HIGH (no dac update, gate low)
  MCP_CS_PORT |= (1<<MCP_LDAC_PIN);
}
//-----------------------------------------------------------
void timer_init()
{
  // Timer 0 configuration
  TCCR0A = (1<<WGM01); // CTC Mode
  OCR0A = 20 * GATE_LENGTH;
  // Enabel Compare Interrupt 
  TIMSK0 |= (1<<OCIE0A);
  
  // Timer 1 (16-bit) configuration
  TCCR1B |= (1<<CS12) | (1<<CS10); //prescaler 1024
  // 20.000.000 / 1024 = 19531,25 = 1 sec
  TCCR1B |= (1<<WGM12); // CTC Modus
  OCR1A = 19531L; //timer => 999,9872 sec
  TIMSK1 |= (1<<OCIE1A);
}
//-----------------------------------------------------------
void timer0_start()
{
  //reset timer
  TCNT0 = 0; 
  //start timer
  TCCR0B |= (1<<CS02) | (1<<CS00); // Prescaler 1024
}
//-----------------------------------------------------------
ISR (TIMER1_COMPA_vect)
{
  if(autosave_counter < AUTOSAVE_TIME)
  {
    autosave_counter++;
  }
}
//-----------------------------------------------------------
void timer_touchAutosave()
{
  //arm autosave
  autosave_flag = 0;
  //reset save counter
  autosave_counter = 0;
}
//-----------------------------------------------------------
void checkAutosave()
{
  //only if autosave is needed
  if(autosave_flag==0)
  {
    if(autosave_counter >= AUTOSAVE_TIME)
    {
      eeprom_WriteBuffer(io_getActiveSteps());    
      autosave_flag = 1;
    }
  }
}
//-----------------------------------------------------------
