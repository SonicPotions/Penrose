#!/bin/bash

#abort on error
set -e
set -u


# set fuses
avrdude   -B100 -c usbasp -p atmega168 -U lfuse:w:0xff:m -U hfuse:w:0xd4:m -U efuse:w:0x02:m
# flash bootloader
#avrdude  -B1  -c usbasp -p atmega168 -U flash:w:audioBootloader.hex:i
avrdude  -B1  -c usbasp -p atmega168 -U flash:w:./FirmwareAndBootloader.hex:i
# lock bootloader - BLB1 Mode 2 = LPM and SPM is not allowed to write to the Boot Loader Section
avrdude  -B1  -c usbasp -p atmega168 -U lock:w:0x0F:m 

echo -e "\e[32m##### Success! #####"
echo -e "\e[39m"