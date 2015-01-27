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

#ifndef IOMATRIX_H_
#define IOMATRIX_H_
#include <avr/io.h>

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

//-----------------------------------------------------------
void io_init();
void turnLedOn(uint8_t colour);
void turnAllLedsOff();
uint8_t io_isButtonPushed();


#endif /* IOMATRIX_H_ */