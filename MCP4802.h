/*
 * MCP4802.h
 *
 * Created: 14.06.2013 15:57:23
 *  Author: Julian
 */ 
#include <avr/io.h>

#ifndef MCP4802_H_
#define MCP4802_H_

#define MCP_CS_PORT		PORTB
#define MCP_CS_DDR		DDRB
#define MCP_CS_PIN		PB0
/*
By bringing down the LDACpin to a low state, the contents stored in the DAC’s input registers are transferred
into the DAC’s output registers (VOUT), and both VOUTA
and VOUTB are updated at the same time.
*/
#define MCP_LDAC_PIN	PB1

void mcp4802_init();
void mcp4802_outputData(const uint8_t out1, const uint8_t out2);


#endif /* MCP4802_H_ */