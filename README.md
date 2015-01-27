# Penrose Quantizer Firmware

This is the sourcecode for the Sonic Potions Penrose Quantizer eurorack module.
The project page with more information about the module can be found here: http://www.sonic-potions.com/penrose

Penrose is a simple CV quantizer DIY kit for the eurorack format. 
It takes an incoming continuous CV voltage and converts it to the nearest note in a scale. 
Active notes can be selected with the 12 buttons. You can use it to tune the output of your analog sequencer, as a semi random melody generator, for fast chiptune arpeggios and much more.

The code has multiple separate parts:
- The root directory contains the Quantizer AVR main program
- The bootloader directory contains an audio bootloader for the AVR and a C++ program to generate .wav files from AVR .hex files

The code is released under the GPL:

Copyright 2015 Julian Schmidt, Sonic Potions <julian@sonic-potions.com>
Web: www.sonic-potions.com/penrose

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
