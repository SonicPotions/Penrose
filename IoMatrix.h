/*
 * IoMatrix.h
 *
 * Created: 14.06.2013 21:04:23
 *  Author: Julian
 */ 


#ifndef IOMATRIX_H_
#define IOMATRIX_H_
#include <avr/io.h>

void io_init();
//one circle trough the whole LED matrix
void io_processLed();
//scan all buttons
void io_processButtons();

uint16_t io_getActiveSteps();
void io_setActiveSteps(uint16_t val);
void io_setCurrentQuantizedValue(uint8_t value);


#endif /* IOMATRIX_H_ */