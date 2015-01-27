/*
 * MCP4802.h
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

#ifndef MCP4802_H_
#define MCP4802_H_

#define MCP_CS_PORT		PORTB
#define MCP_CS_DDR		DDRB
#define MCP_CS_PIN		PB0
/*
By bringing down the LDAC pin to a low state, the contents stored in the DAC's input registers are transferred
into the DAC's output registers (VOUT), and both VOUTA
and VOUTB are updated at the same time.
*/
#define MCP_LDAC_PIN	PB1
//-----------------------------------------------------------
void mcp4802_init();
void mcp4802_outputData(const uint8_t out1, const uint8_t out2);


#endif /* MCP4802_H_ */