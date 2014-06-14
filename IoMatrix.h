/*
 * IoMatrix.h
 *
 * Created: 14.06.2013 21:04:23
 *  Author: Julian
 */ 


#ifndef IOMATRIX_H_
#define IOMATRIX_H_
#include <avr/io.h>

//extern uint16_t io_buttonState;	//state of the 12 IO buttons
extern uint16_t io_ledState;		//state of the 13 LEDs
extern uint16_t io_lastButtonState;
extern uint8_t io_activeStep;		//current active quantisation step

void io_init();
//one circle trough the whole LED matrix
void io_tickLed();
//scan all buttons
void io_tickButtons();


#endif /* IOMATRIX_H_ */