#!/bin/bash
ESPTOOL=/home/jeroen/esp8266/esp32/esp-idf/components/esptool_py/esptool/esptool.py
#python /home/jeroen/esp8266/esp32/esp-idf/bin/esptool.py --chip esp32 --port "/dev/ttyUSB0" --baud 115200 write_flash -z -fs 32m 0x100000 "$1"
python $ESPTOOL --chip esp32 --port "/dev/ttyUSB1" --baud $((230400*4)) write_flash -fs 32m 0x100000 "$1"
