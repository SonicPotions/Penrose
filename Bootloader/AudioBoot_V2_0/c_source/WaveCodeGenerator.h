/*
 * 
	wave generator for audio bootloader
		
	Original JAVA code by (c) -C-H-R-I-S-T-O-P-H-   -H-A-B-E-R-E-R- 2011
	Ported to C++ by julian Schmidt 2014

	This program is free software; you can redistribute it and/or modify
 	it under the terms of the GNU General Public License as published by
 	the Free Software Foundation; either version 2 of the License, or
 	(at your option) any later version.
 	 	
*/

#include "hex2bin.h"
#include "hex2signal.h"
#include "BootFrame.h"
#include "wave.h"

#include <vector>

#include <stdlib.h>   
using namespace std;

static int sampleRate = 44100;		// Samples per second


 
class WavCodeGenerator {
  
public:
  WavCodeGenerator()
  {

  };
  ~WavCodeGenerator()
  {

  };
  
public:
  void generatePageSignal(std::vector<int> &data, std::vector<double> &outPtr)
  {
	  HexToSignal h2s;

	  std::vector<int> frameData;
	  frameData.resize(frameSetup.getFrameSize());
	  
	  // copy data into frame data
	  for(size_t n=0;n<frameSetup.getPageSize();n++)
	  {
		  if(n<data.size()) frameData[n+frameSetup.getPageStart()]=data[n];
		  else frameData[n+frameSetup.getPageStart()]=0xFF;
	  }
	  frameSetup.addFrameParameters(frameData);
	  h2s.manchesterCoding(frameData, frameSetup.getFrameSize(), outPtr);
  }
  // duration in seconds
  void silence(double duration, std::vector<double> &outPtr)
  {
	  int size = (int)(duration * sampleRate);
	  
	  for(int i=0;i<size;i++)
	  {
	    outPtr.push_back(0);
	  }
  }
  void makeRunCommand(std::vector<double> &output)
  {
	  HexToSignal h2s;
	  std::vector<int> frameData;
	  frameData.resize(frameSetup.getFrameSize());
	  
	  frameSetup.setRunCommand();
	  frameSetup.addFrameParameters(frameData);
	  
	  h2s.manchesterCoding(frameData, frameSetup.getFrameSize(), output);
  }
  void generateSignal(int data[], int size, std::vector<double> &output)
  {
	  frameSetup.setProgCommand(); // we want to programm the mc
	  int pl=frameSetup.getPageSize();
	  int total=size;
	  int sigPointer=0;
	  int pagePointer=0;
	  while(total>0)
	  {
		  cout << ".";
		  
		  frameSetup.setPageIndex(pagePointer++);
		  std::vector<int> partSig;
		  partSig.resize(pl);
		  
		  for(int n=0;n<pl;n++)
		  {
			  if(n+sigPointer>size-1) partSig[n]=0xFF;
			  else partSig[n]=data[n+sigPointer];
		  }
		  sigPointer+=pl;
		  
		  std::vector<double> sig;
		  generatePageSignal(partSig, sig);
		  //signal=appendSignal(signal,sig);
		  appendSignal(output,sig); //add sig to signal
		  
		  std::vector<double> silencePtr;
		  silence(frameSetup.getSilenceBetweenPages(), silencePtr);
		  appendSignal(output, silencePtr);
		  
		  total-=pl;
	  }
	  
	  std::vector<double> runCmd;;
	  makeRunCommand(runCmd);
	  appendSignal(output,   runCmd); // send mc "start the application"
  }

  
  bool convertHex2Wav(char* hexFilePath, char* wavFilePath)
  {
	  cout << "######## Intel HEX to .wav ########" << endl;
	  cout << "#    AVR audio bootloader tool    #" << endl;
	  cout << "###################################" << endl;


	  Hex2Bin hex2bin;
	  hex2bin.load_file(hexFilePath);

	  int *srcData = hex2bin.getData();

	  cout << "generating";
	  std::vector<double> signal;
	  generateSignal(srcData, hex2bin.getSize(),signal);
	  cout << endl;

	  cout << "saving wave file of size " << signal.size() << endl;
	  short data[signal.size()+1];
	  data[0]=0;
	  for(unsigned int i=0;i<signal.size();i++)
	  {
	    data[i+1] = signal[i]*32767;
	  }

	  writeWAVData(wavFilePath, data, signal.size()*2,sampleRate,1);

	  return true;
  }
  
private:
  BootFrame frameSetup;
  
  void appendSignal(std::vector<double> &sig1, std::vector<double> &sig2)
  {
	  int l2=sig2.size();;
 
	  for(int i=0;i<l2;i++)
	  {
	      sig1.push_back(sig2[i]);
	  }
  }
};
