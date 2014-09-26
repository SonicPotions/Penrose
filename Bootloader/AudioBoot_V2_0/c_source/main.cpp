/* ##################################################################################################################################
    AVR Audio Bootloader
    Copyright (C) 2014  Julian Schmidt / Sonic Potions / julian@sonic-potions.com

    This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110, USA

##################################################################################################################################
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
