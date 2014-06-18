/*
 * eeprom.h
 *
 * Created: 14.06.2014 11:46
 *  Author: Julian Schmidt
 * 
 * EEPROM routines with wear leveling
 * see atmel app note AVR101: High Endurance EEPROM Storage
 */ 

#include <avr/io.h>
#include <avr/eeprom.h>

uint16_t eeprom_ReadBuffer();
void eeprom_WriteBuffer(uint16_t data);
void eeprom_saveCalibrationData(uint8_t gain, uint8_t offset);
