#!/bin/bash
# set fuses
avrdude   -B100 -c usbasp -p atmega168 -U lfuse:w:0xe2:m -U hfuse:w:0xd4:m -U efuse:w:0x02:m
# flash bootloader
avrdude  -B1  -c usbasp -p atmega168 -U flash:w:audioBootloader.hex:i
# lock bootloader - BLB1 Mode 2 = LPM and SPM is not allowed to write to the Boot Loader Section
avrdude  -B1  -c usbasp -p atmega168 -U lock:w:0xCF:m 