/*
 * MCP4802.c
 *
 * Driver works for MCP4801 
 * the prototype used a 4802, but channel 2 is not implemented yet 
 * since the final hardware version never used it and a 4801 was used instead
 * needs minor mods for propper 2 channel output
 * 
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
 
#include "MCP4802.h"
#include "spi.h"
#include "IoMatrix.h"

#define DAC_A				0
#define DAC_B				1

#define GAIN_X2				0
#define GAIN_X1				1

#define CHANNEL_ACTIVE			1
#define CHANNEL_SHUTDOWN		0
//-----------------------------------------------------------
void mcp4802_init()
{
	//init SPI port
	spi_init();
	
	//pin init 
	MCP_CS_DDR  |= (1<<MCP_CS_PIN) | (1<<MCP_LDAC_PIN); //CS pin as output, LDAC pin as output
	
	// then set high (no write)
	MCP_CS_PORT |= (1<<MCP_CS_PIN) | (1<<MCP_LDAC_PIN);
};
//-----------------------------------------------------------
/*
The write command is initiated by driving the CS pin
low, followed by clocking the four Configuration bits and
the 12 data bits into the SDI pin on the rising edge of
SCK. The CSpin is then raised, causing the data to be
latched into the selected DACs input registers.

bit 15 A/B:DAC A or DAC B Selection bit
1= Write to DACB
0= Write to DACA

bit 14  Don't Care 

bit 13 GA:Output Gain Selection bit
1=1x (VOUT= VREF* D/4096)
0=2x (VOUT= 2 * VREF* D/4096), where internal VREF= 2.048V.

bit 12 SHDN:Output Shutdown Control bit
1= Active mode operation. VOUTis available. ?
0= Shutdown the selected DAC channel. Analog output is not available at the channel that was shut down. 
VOUTpin is connected to 500 k???typical)?

bit 11-0 D11:D0:DAC Input Data bits. Bit x is ignored.
*/
void mcp4802_outputData(const uint8_t out1, const uint8_t out2)
{
	//LDAC HIGH (no dac update, gate low)
	MCP_CS_PORT |= (1<<MCP_LDAC_PIN);
	//CS low -> start write cmd
	MCP_CS_PORT &= ~(1<<MCP_CS_PIN);
	
	//data out B (not implemented)
	
	//data for output A
	uint16_t data = (DAC_A<<15) | (GAIN_X2<<13) | (CHANNEL_ACTIVE<<12) | (out1<<4);
	
	//save COL_PORT
	uint8_t port = COL_PORT;
	COL_PORT |= ( (1<<COL1_PIN) | (1<<COL2_PIN) | (1<<COL3_PIN) | (1<<COL4_PIN) );
	spi_enable(1);
	SPI_transmit(data>>8);		//transmit MSB
	SPI_transmit(data&0xff);	//transmit LSB
	spi_enable(0);
	COL_PORT = port;
	
	// CS high (end write)
	MCP_CS_PORT |= (1<<MCP_CS_PIN);
	
	//LDAC low (update dac outputs, gate high)
	MCP_CS_PORT &= ~(1<<MCP_LDAC_PIN);
};
