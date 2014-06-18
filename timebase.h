/*
 * timebase.h
 *
 * Created: 15.06.2013 8:47
 *  Author: Julian Schmidt
 * 
 * contains timer functions for autosave and gate length
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>

#define GATE_LENGTH 10 // [ms] maximum is 12 (=>240, more will not fit into OCR0A and a 16 bit timer must be used)
#define AUTOSAVE_TIME 15 // [sec]

void timer_init();

/* Gate Timer
 * This function will turn off the gate signal after GATE_LENGTH [ms]
 */
void timer0_start();

/* called whenever a button is pressed.
 * autosave timer will be started and reset to zero.
 * this has 2 functions
 * - after a change is made, an autosave to eeprom is necessary to keep buttons state after power down
 * - autosave should only happen AUTOSAVE_TIME seconds after the last change
 */
void timer_touchAutosave();

/* checks timer variables if an autosave is needed */
void checkAutosave();







