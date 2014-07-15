/*
 * IoMatrix.h
 *
 * Created: 14.06.2013 21:04:23
 *  Author: Julian
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
//one circle trough the whole LED matrix
void io_processLed();

void io_processLedPipelined();
//scan all buttons
void io_processButtons();
void io_processButtonsPipelined();

uint16_t io_getActiveSteps();
void io_setActiveSteps(uint16_t val);
void io_setCurrentQuantizedValue(uint8_t value);
uint8_t io_isButtonPushed(uint8_t buttonNr);


#endif /* IOMATRIX_H_ */