/*
 * 
	wave generator for audio bootloader
	
	frame format
	
	(c) -C-H-R-I-S-T-O-P-H-   -H-A-B-E-R-E-R- 2011

	This program is free software; you can redistribute it and/or modify
 	it under the terms of the GNU General Public License as published by
 	the Free Software Foundation; either version 2 of the License, or
 	(at your option) any later version.
 	
 	Ported to C++ by julian Schmidt 2014
*/
#include <vector>

class BootFrame {

	/*
	 * 	#define COMMAND         0
		#define PAGEINDEXLOW 	1  // page address lower part
		#define PAGEINDEXHIGH 	2  // page address higher part
		#define CRCLOW          3  // checksum lower part
		#define CRCHIGH 		4  // checksum higher part
		#define DATAPAGESTART   5  // start of data
		#define FRAMESIZE       (DATAPAGESTART+128) // size of the data block to be received
	 */

private:	
	int command;
	int pageIndex;
	int crc;
	int pageStart;
	int pageSize;
	int frameSize;
	
	//private double silenceBetweenPages=2; // 2 seconds for debugging purposes silence in seconds
	double silenceBetweenPages; // silence in seconds
	
public:
	BootFrame()
	{
		command=0;
		pageIndex=4;
		crc=0x55AA;
		
		pageStart=5;
		pageSize=128;
		frameSize=pageStart+pageSize;
		silenceBetweenPages=0.02;
	}
	void setProgCommand()
	{
		command=2;
	}
	void setRunCommand()
	{
		command=3;
	}
	void addFrameParameters(std::vector<int> &data)
	{
		data[0]=command;
		data[1]=pageIndex&0xFF;
		data[2]=(pageIndex>>8)&0xFF;
		data[3]=crc&0xFF;
		data[4]=(crc>>8)&0xFF;

	}
	void setFrameSize(int frameSize) {
		this->frameSize = frameSize;
	}
	int getFrameSize() {
		return frameSize;
	}
	void setCommand(int command) {
		this->command = command;
	}
	int getCommand() {
		return command;
	}
	void setPageIndex(int pageIndex) {
		this->pageIndex = pageIndex;
	}
	int getPageIndex() {
		return pageIndex;
	}
	void setCrc(int crc) {
		this->crc = crc;
	}
	int getCrc() {
		return crc;
	}
	void setPageStart(int pageStart) {
		this->pageStart = pageStart;
	}
	int getPageStart() {
		return pageStart;
	}
	void setPageSize(int pageSize) {
		this->pageSize = pageSize;
	}
	int getPageSize() {
		return pageSize;
	}
	void setSilenceBetweenPages(double silenceBetweenPages) {
		this->silenceBetweenPages = silenceBetweenPages;
	}
	double getSilenceBetweenPages() {
		return silenceBetweenPages;
	}
};

 
