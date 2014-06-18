/*
 * timebase.c
 *
 * Created: 15.06.2013 8:47
 *  Author: Julian Schmidt
 * 
 * contains timer functions for autosave and gate length
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
  // TCCR0B |= (1<<CS02) | (1<<CS00); // Prescaler 1024
  // ((20.000.000/1024)/1000) = 19,53125
  OCR0A = 20 * GATE_LENGTH; // => 0,9765625ms
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
