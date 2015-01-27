/*
 * eeprom.c
 *
 * EEPROM routines with wear leveling
 * see atmel app note AVR101: High Endurance EEPROM Storage
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

#include "eeprom.h"

#define EEPROM_DATASIZE 2		// 12 notes = 12 bit = 2 bytes
#define EEPROM_SIZE 512 		// Atmega168 = 512 byte
#define BUFFER_MASK 0x7f

#define BUFFER_SIZE (EEPROM_SIZE/2/EEPROM_DATASIZE)	// Atmega168 = 128 bytes. 
							// using 2 bytes for the 12 bit note value results in 128 * 100.000cycles 
							// => 12.800.000 cycles
							// thats about 35 years with 1000 saves per day
							
//-----------------------------------------------------------
static uint16_t dataBuffer[BUFFER_SIZE] EEMEM;		// place to store the note data -> 2x128 = 256 bytes
static uint8_t statusBuffer[BUFFER_SIZE] EEMEM;		// place to keep the status info (next write position) -> 128 bytes
static uint8_t currentEepromAddress = 0;

//-----------------------------------------------------------
uint8_t findCurrentEepromAddr()
{
  uint8_t val1, val2, addr1, addr2;
  addr1 = 0x00;
  
  for(int i=0; i<BUFFER_SIZE; i++)
  {
    addr2 = ((addr1+1)&BUFFER_MASK);
    val1 = eeprom_read_byte(&statusBuffer[addr1]);
    val2 = eeprom_read_byte(&statusBuffer[addr2]);
    
     if( ((val1+1)&0xff) == val2)
     {
       //move on
       addr1++;
       
     } else
     {
       //this is the last used address
       return addr1;
     }
  }
  //we should never get here...
  return addr1;
}
//-----------------------------------------------------------
/*
 * This function is called once after power up.
 * First the current active eeprom address is searched 
 * then it's content is read from the dataBuffer
 */
uint16_t eeprom_ReadBuffer()
{
  currentEepromAddress = findCurrentEepromAddr();
  return eeprom_read_word(&dataBuffer[currentEepromAddress])&0xfff;
}
//-----------------------------------------------------------
void eeprom_WriteBuffer(uint16_t data)
{
  //check if current value differs from new value
  uint16_t storedData = eeprom_read_word(&dataBuffer[currentEepromAddress]);
  if(storedData == data)
  {
    //we have nothing to do -> return
    return;
  }
  
  //we need to write a new value
  
  //get current stausBuffer value
  uint8_t status = eeprom_read_byte(&statusBuffer[currentEepromAddress]);
  
  // increment current dataBuffer address
  currentEepromAddress = (currentEepromAddress+1)&BUFFER_MASK;
  
  // store parameter to dataBuffer
  eeprom_write_word(&dataBuffer[currentEepromAddress], data);
  
  //increment and store status buffer value
  status += 1;//(status+1)&BUFFER_MASK;
   
  eeprom_write_byte(&statusBuffer[currentEepromAddress],status);
}
//-----------------------------------------------------------
