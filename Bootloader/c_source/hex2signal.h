/*
 * 
	wave generator for audio bootloader
	
	hex values to audio signal converter  (manchester code)
	
	(c) -C-H-R-I-S-T-O-P-H-   -H-A-B-E-R-E-R- 2011

	This program is free software; you can redistribute it and/or modify
 	it under the terms of the GNU General Public License as published by
 	the Free Software Foundation; either version 2 of the License, or
 	(at your option) any later version.
 	
 	Ported to C++ by julian Schmidt 2014
*/

#include <vector>

#include <stdlib.h>   
using namespace std;

class HexToSignal {
  
public:
  HexToSignal()
  {
	lowNumberOfPulses=2;
	highNumberOfPulses=3;
	startSequencePulses=40;
	numStartBits=1;
	numStopBits=1;
	invertSignal=true; // correction of an inverted audio signal line
	manchesterPhase=1; // current phase for differential manchester coding
	
	manchesterNumberOfSamplesPerBit=4; // this value must be even
  }
  ~HexToSignal()
  {
  }

public:
	void manchesterCoding(std::vector<int> &hexdata, int inputSize, std::vector<double> &outPtr)
	{
	  

		int laenge=inputSize;
		int size = (1+startSequencePulses+laenge*8)*manchesterNumberOfSamplesPerBit;
		

		//outPtr=new double[size];
		//double[] signal=new double[(1+startSequencePulses+laenge*8)*manchesterNumberOfSamplesPerBit];
		outPtr.resize(size);

		
		int counter=0;
		/** generate synchronisation start sequence **/
		for (int n=0; n<startSequencePulses; n++)
		{
			manchesterEdge(false,counter,outPtr); // 0 bits: generate falling edges 
			counter+=manchesterNumberOfSamplesPerBit;
		}
		
		/** start bit **/
		manchesterEdge(true,counter,outPtr); //  1 bit:  rising edge 
		counter+=manchesterNumberOfSamplesPerBit;
		
		/** create data signal **/
		int count=0;
		for(count=0;count<laenge;count++)
		{
			int dat=hexdata[count];
			//System.out.println(dat);
			/** create one byte **/			
			for( int n=0;n<8;n++) // first bit to send: MSB
			{
				if((dat&0x80)==0) 	manchesterEdge(false,counter,outPtr); // generate falling edges ( 0 bits )
				else 				manchesterEdge(true,counter,outPtr); // rising edge ( 1 bit )
				counter+=manchesterNumberOfSamplesPerBit;	
				dat=dat<<1; // shift to next bit
			}
		}
	
	}
	
private:
	int lowNumberOfPulses;
	int highNumberOfPulses;
	int startSequencePulses;
	int numStartBits;
	int numStopBits;
	bool invertSignal; // correction of an inverted audio signal line
	double manchesterPhase; // current phase for differential manchester coding
	
	int manchesterNumberOfSamplesPerBit; // this value must be even
	/* flag=true: rising edge
	 * flag=false: falling edge
	 */
	void manchesterEdge(bool flag, int pointerIntoSignal,std::vector<double>  &signal )
	{
		//double sigpart[]=new double[manchesterNumberOfSamplesPerBit];
		int n;
		double value;

		if(false) // manchester code
		{
			if(flag) value=1;
			else value=-1;
			if(invertSignal)value=value*-1;  // correction of an inverted audio signal line
			for(n=0;n<manchesterNumberOfSamplesPerBit;n++)
			{
				if(n<manchesterNumberOfSamplesPerBit/2)signal[pointerIntoSignal]=-value;
				else signal[pointerIntoSignal]=value;
				pointerIntoSignal++;
			}
		}
		else // differential manchester code ( inverted )
		{
			if(flag) manchesterPhase=-manchesterPhase; // toggle phase
			for(n=0;n<manchesterNumberOfSamplesPerBit;n++)
			{
				if(n==(manchesterNumberOfSamplesPerBit/2))manchesterPhase=-manchesterPhase; // toggle phase
				signal[pointerIntoSignal]=manchesterPhase;
				pointerIntoSignal++;
			}		
		}
		//return sigpart;
	}

	
};
 
