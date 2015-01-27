/*
 * timebase.h
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


#include <avr/io.h>
#include <avr/interrupt.h>

#define GATE_LENGTH 12 // [ms] maximum is 12 (=>240, more will not fit into OCR0A and a 16 bit timer must be used)
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







