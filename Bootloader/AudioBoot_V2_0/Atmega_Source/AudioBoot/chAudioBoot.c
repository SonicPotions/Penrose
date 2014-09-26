/*
 * Source code simplified and adapted to Sonic Potions Quantizer Module by Julian Schmidt 2014 
 * Original code by Christoph Haberer 2011 (see below)
 * 
##################################################################################################################################
    AVR Audio Bootloader
    Copyright (C) 2014  Julian Schmidt / Sonic Potions / julian@sonic-potions.com

    This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110, USA

##################################################################################################################################


#define ATMEGA168_MICROCONTROLLER
//#define ATMEGA8_MICROCONTROLLER



//***************************************************************************************
/*
	AudioBoot - flashing a microcontroller by PC audio line out 
				This version is with differential manchester coding

	necessary setup

	1. Project->ConfigurationOptions->Processortype
	2. Project->ConfigurationOptions->Programming Modell 'Os'
	3. Project->ConfigurationOptions->CustomOptions->LinkerOptions->see further down
	5. BootresetvectorFuseBit enable Bootloader
	6. compile and flash Bootloader


	ATMEGA168, 1K bootloader: -Wl,--section-start=.text=0x3c00
	Fuses Atmega168:
	Extended: 0xFA
	HIGH: 0xDF
	LOW: E2


	to protect the bootloader section from beeing overwritten, set the memory protection
	flags as follows:

	BOOTSz1=0;BOOTSz0=0;
	( lockbits: 0xCF ==> LPM and SPM prohibited in bootloader section )

	************************************************************************************

	(c) -C-H-R-I-S-T-O-P-H-   -H-A-B-E-R-E-R- 2011

	v0.1	19.6.2008	C. -H-A-B-E-R-E-R- 	Bootloader for IR-Interface
	v1.0	03.9.2011	C. -H-A-B-E-R-E-R-	Bootloader for audio signal
	v1.1	05.9.2011	C. -H-A-B-E-R-E-R-	changing pin setup, comments, and exitcounter=3 
	v1.2	12.5.2012	C. -H-A-B-E-R-E-R-	Atmega8 Support added, java programm has to be addeptet too 
	v1.3	20.5.2012	C. -H-A-B-E-R-E-R-	now interrupts of user programm are working 
	v1.4	05.6.2012	C. -H-A-B-E-R-E-R-  signal coding changed to differential manchester code
	v2.0	13.6.2012	C. -H-A-B-E-R-E-R-	setup for various MCs

	This program is free software; you can redistribute it and/or modify
 	it under the terms of the GNU General Public License as published by
 	the Free Software Foundation; either version 2 of the License, or
 	(at your option) any later version.
*/
//***************************************************************************************




	#include <avr/io.h>
	#include <avr/interrupt.h>
	#include <avr/interrupt.h>
	#include <stdlib.h>
	#include <avr/boot.h>
	#include <util/delay.h>
	
	#include "IoMatrix.h"

	// use TxD ( PD7 ) as audio input	
	#define INPUTAUDIOPIN (1<<PD7)
	#define PINVALUE (PIND&INPUTAUDIOPIN)

	//#define INITPORT {PORTC|=INPUTAUDIOPIN;} //turn on pull up 

	#define PINLOW (PINVALUE==0)
	#define PINHIGH (!PINLOW)


	#define true (1==1)
	#define false !true


//***************************************************************************************
// main loop
//***************************************************************************************

#define TIMER TCNT2 // we use timer2 for measuring time

// frame format definition
#define COMMAND         0
#define PAGEINDEXLOW 	1  // page address lower part
#define PAGEINDEXHIGH 	2  // page address higher part
#define CRCLOW          3  // checksum lower part 
#define CRCHIGH 		4  // checksum higher part 
#define DATAPAGESTART   5  // start of data
#define PAGESIZE 		128
#define FRAMESIZE       (PAGESIZE+DATAPAGESTART)// size of the data block to be received

// bootloader commands
#define NOCOMMAND       0
#define TESTCOMMAND     1
#define PROGCOMMAND     2
#define RUNCOMMAND      3

uint8_t FrameData[FRAMESIZE];

//***************************************************************************************
// receiveFrame()
//
// This routine receives a differential manchester coded signal at the input pin.
// The routine waits for a toggling voltage level. 
// It automatically detects the transmission speed.
//
// output: 		uint8_t flag: true: checksum ok
//				Data // global variable
// 
//***************************************************************************************
uint8_t receiveFrame()
{
  uint16_t store[16];

  uint16_t counter=0;
  volatile uint16_t time=0;
  volatile uint16_t delayTime;
  uint8_t p,t;
  uint8_t k=8;
  uint8_t dataPointer=0;
  uint16_t n;

  //*** synchronisation and bit rate estimation **************************
  time=0;
  // wait for edge
  p=PINVALUE;
  while(p==PINVALUE);

  p=PINVALUE;
  
  TIMER=0; // reset timer
  for(n=0;n<16;n++)
  {
    // wait for edge
	while(p==PINVALUE);
	t=TIMER;
	TIMER=0; // reset timer
    p=PINVALUE;

	store[counter++]=t;

    if(n>=8)time+=t; // time accumulator for mean period calculation only the last 8 times are used 
  }
  
  delayTime=time*3/4/8;
  // delay 3/4 bit
  while(TIMER<delayTime);

  //p=1;
  
  //****************** wait for start bit ***************************
  while(p==PINVALUE) // while not startbit ( no change of pinValue means 0 bit )
  {
    // wait for edge
	while(p==PINVALUE);
    p=PINVALUE;
	TIMER=0;

    // delay 3/4 bit
    while(TIMER<delayTime);
	TIMER=0;

    counter++;
  }
  p=PINVALUE;
  //****************************************************************
  //receive data bits
  k=8;
  for(n=0;n<(FRAMESIZE*8);n++)
  {
	// wait for edge
	while(p==PINVALUE);
    TIMER=0;
	p=PINVALUE;
    
    // delay 3/4 bit
    while(TIMER<delayTime);
	
	t=PINVALUE;

    counter++;
    
    FrameData[dataPointer]=FrameData[dataPointer]<<1;
    if(p!=t) FrameData[dataPointer]|=1;
	p=t;
    k--;
    if(k==0){dataPointer++;k=8;};
  }
    uint16_t crc=(uint16_t)FrameData[CRCLOW]+FrameData[CRCHIGH]*256;

  if(crc==0x55AA) return true;
  else return false;
}

//***************************************************************************************
//	void boot_program_page (uint32_t page, uint8_t *buf)
//
//  Erase and flash one page.
//
//  inputt: 		page address and data to be programmed
// 
//***************************************************************************************
void boot_program_page (uint32_t page, uint8_t *buf)
{
	uint16_t i;
  
    cli(); // disable interrupts

    boot_page_erase (page);
    boot_spm_busy_wait ();      // Wait until the memory is erased.

    for (i=0; i<SPM_PAGESIZE; i+=2)
    {
        uint16_t w = *buf++;
        w += (*buf++) << 8;
        
        boot_page_fill (page + i, w);
    	boot_spm_busy_wait();       // Wait until the memory is written.
	}

    boot_page_write (page);     // Store buffer in flash page.
    boot_spm_busy_wait();       // Wait until the memory is written.
	
    boot_rww_enable ();
}
//***************************************************************************************
void initstart()
{
   // Timer 2 normal mode, clk/8, count up from 0 to 255  
   // ==> frequency @16MHz= 16MHz/8/256=7812.5Hz

#ifdef ATMEGA8_MICROCONTROLLER
   	TCCR2= _BV(CS21);
#endif
#ifdef ATMEGA168_MICROCONTROLLER
   TCCR2B= _BV(CS21);
#endif
}
//***************************************************************************************
//jump to address 0x0000
void (*start)( void ) = 0x0000;
//***************************************************************************************
void runProgramm(void)
{
	// reintialize registers to default
	DDRB=0;
	DDRC=0;
	DDRD=0;
	cli();
#ifdef ATMEGA8_MICROCONTROLLER
	TCCR2=0; // turn off timer2
	// make sure that interrupt vectors point to user space
	GICR=(1<<IVSEL);
	GICR=0;
#endif
#ifdef ATMEGA168_MICROCONTROLLER
	TCCR2B=0; // turn off timer2
#endif

/*
	// start user programm
	asm volatile(		
	"clr r30	\n\t"
	"clr r31	\n\t"	// z Register mit Adresse laden
	"ijmp		\n\t"	// z Register mit Adresse laden
	);
	*/
start();
}

/* **************************************************************************************
 * LED Functions
 *
 * Red blinking => waiting for audio files
 * Green blinking => update running
 * Red => update failed
 * Green => update succeed
 * 
 * 
 ***************************************************************************************/

#define RED 0
#define GREEN 1

void ledOn(uint8_t colour)
{
  turnLedOn(colour);
}

void ledOff()
{
  turnAllLedsOff();
}

static uint8_t ledState=0;

void ledToggle(uint8_t colour)
{
  if(ledState)
  {
    turnAllLedsOff();
  } else {
    turnLedOn(colour);
  }
  ledState = 1-ledState;
}

//***************************************************************************************
// main loop
//***************************************************************************************
void a_main()
{
  uint8_t timeout = 10;
  
  while(1)
  {
    if(io_isButtonPushed() )
    {
      break;
    }
    
    timeout--;
    if(timeout==0)
    {
	runProgramm();
    }
    
  }
  

  
  
  //button is pushed, start bootloader
  

  initstart();
  


  uint8_t p=0;
  #define WAITBLINKTIME 1000
  uint16_t time=WAITBLINKTIME;
  //uint8_t timeout=6;
 
  //*************** wait for toggling input pin or timeout ******************************
  uint8_t exitcounter=3;
  while(1)
  {  

    if(TIMER>100)  // timedelay ==> frequency @16MHz= 16MHz/8/100=20kHz
    {
       TIMER=0;
       time--;
       if(time==0)
       {
         ledToggle(RED);
	 time=WAITBLINKTIME;
    /*
	 timeout--;
         if(timeout==0)
         {

           LEDOFF; // timeout,
           // leave bootloader and run program
	   runProgramm();
         }
         */
       }
    }
    if(p!=PINVALUE)
    {
      p=PINVALUE;
      exitcounter--;
    }
    if(exitcounter==0) break; // signal received, leave this loop and go on
  }
  //*************** start command interpreter *************************************  
  ledOff();
  
  while(1)
  {
    if(!receiveFrame())
    {
      //*****  error: blink fast, press reset to restart *******************
      while(1)
      {   
	ledOn(RED); //=> error, RED LED
	/*
        if(TIMER>100)  // timerstop ==> frequency @16MHz= 16MHz/8/100=20kHz
        {
           TIMER=0;
           time--;
           if(time==0)
           {
             TOGGLELED;
             time=1000;
           }
        }
        */
      }
    }
    else // succeed
    {
      ledToggle(GREEN);;
      switch(FrameData[COMMAND])
      {
        case TESTCOMMAND: // not used yet
        {

        }
        break;
        case RUNCOMMAND:
        {
		//setExternalPort(2);
          // leave bootloader and run program
		runProgramm();
        }
        break;
        case PROGCOMMAND:
        { 
  #ifdef ATMEGA168_MICROCONTROLLER
  			// Atmega168 Pagesize=64 Worte=128 Byte
			uint16_t k;
			k=(((uint16_t)FrameData[PAGEINDEXHIGH])<<8)+FrameData[PAGEINDEXLOW];
			boot_program_page (SPM_PAGESIZE*k, FrameData+DATAPAGESTART);	// erase and programm page
  #endif
  #ifdef ATMEGA8_MICROCONTROLLER
  			// Atmega8 Pagesize=32 Worte=64 Byte
			uint16_t k;
			k=(((uint16_t)FrameData[PAGEINDEXHIGH])<<8)+FrameData[PAGEINDEXLOW];
			boot_program_page (SPM_PAGESIZE*k*2, FrameData+DATAPAGESTART);	// erase and programm page
			boot_program_page (SPM_PAGESIZE*(k*2+1), FrameData+SPM_PAGESIZE+DATAPAGESTART);	// erase and programm page

  #endif
        }
        break;
      }
      FrameData[COMMAND]=NOCOMMAND; // delete command
    }
  }
}

int main()
{
//		INITPORT; 
	DDRD &= ~(1<<PD7); //input
	io_init();
	a_main(); // start the main function
}
