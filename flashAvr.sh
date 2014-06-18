#!/bin/bash
# set fuses
#avrdude   -B100 -c usbasp -p atmega168 -U lfuse:w:0xe2:m -U hfuse:w:0xd4:m
# flash bootloader
avrdude  -B1  -c usbasp -p atmega168 -U flash:w:quantizer.hex:i
# lock bootloader - BLB1 Mode 2 = SPM is not allowed to write to the Boot Loader Section
#avrdude  -B1  -c usbasp -p m644 -U lock:w:0x2F:m 