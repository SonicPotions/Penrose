
#############################################################################################
    AVR Audio Bootloader
    
    Copyright 2015 Julian Schmidt, Sonic Potions <julian@sonic-potions.com>
    based on code by Christoph Haberer copyright 2008-2012 AudioBoot_V2_0
    
   This file is part of the Penrose Quantizer Firmware.
 
   The Penrose Quantizer Firmware is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
 
   The Penrose Quantizer Firmware is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
 
   You should have received a copy of the GNU General Public License
   along with the Penrose Quantizer Firmware.  If not, see <http://www.gnu.org/licenses/>.


##############################################################################################


This package consists of 2 parts:
- AVR sourcecode (tested on atmega168) of the audio bootloader
- a C++ command line tool to generate .wav files from itel hex files.

Using these tools firmware updates for the AVR can be made with an audio cable and an audio player using manchester coding.

This bootloader is based on the AudioBoot_V2_0 bootloader by Christoph Haberer from www.roboterclub-freiburg.de
His original post can be found here: http://www.hobby-roboter.de/forum/viewtopic.php?f=4&t=127

The original java tools to generate the wav file were discarded and ported to C++ (I don't like java)
The AVR code was adapted to work with a modified hardware (Sonic Potions Penrose Quantizer module)

Here is the original README text from AudioBoot_V2_0:



1. java-programm:
There are two java programms. The slow one is suitable for mp3 compression with 320kBit/s

2. Bootloader

	AudioBoot - flashing a microcontroller by PC audio line out 
				This version is with differential manchester coding

	Hardware: 	Atmega8 
				Atmega168

	input pin: 	should be connected to a voltage divider.

				The input pin is also connected by a 10nF capacitor to the PC line out
				
				The Atmega168 seems to have the switching voltage level at 2.2V
				The Atmega8 at 1.4V
				The switching levels of the input pins may vary a little bit from one
				MC to another.	If you to be able to adjust the voltages, 
				use a 10k poti as voltage divider.

	outuput: 	status led connected to a pin with a 470 Ohm resistor


	As developement platform an Arduino Diecimilla was used. Therefore you
	will find many #ifdefs for the Arduino in this code.
	If you want to optimize the bootloader further you may use an Arduino 
	as developement platform.


	necessary setup

	1. Project->ConfigurationOptions->Processortype
	2. Project->ConfigurationOptions->Programming Modell 'Os'
	3. Project->ConfigurationOptions->CustomOptions->LinkerOptions->see further down
	5. BootresetvectorFuseBit enable Bootloader
	6. compile and flash Bootloader


	ATMEGA8, 1K byte bootloader: -Wl,--section-start=.text=0x1c00
	HIGH:0xDA
	LOW:0xE4

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
	v2.0	13.6.2012	C. -H-A-B-E-R-E-R-	setup for Atmega8 and Atmega168

	This program is free software; you can redistribute it and/or modify
 	it under the terms of the GNU General Public License as published by
 	the Free Software Foundation; either version 2 of the License, or
 	(at your option) any later version.




14.6.2012
chris
www.roboterclub-freiburg.de