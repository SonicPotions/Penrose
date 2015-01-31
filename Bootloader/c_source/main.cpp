/*
 *  AVR Audio Bootloader
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

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>   
using namespace std;
#include "WaveCodeGenerator.h"
 
int main(int argc,char *argv[]){

  //check if arguments are valid
  if (argc < 3)
  {
    cout << "not enough arguments!" << endl << "you need to call 'hex2wav input.hex output.wav'" << endl;
    exit(1);
  }
  
  WavCodeGenerator waveGenerator;
  waveGenerator.convertHex2Wav(argv[1], argv[2]);

  exit(0);
}
