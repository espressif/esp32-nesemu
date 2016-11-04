#
# This is a project Makefile. It is assumed the directory this Makefile resides in is a
# project subdirectory.
#

PROJECT_NAME := nesemu

include $(IDF_PATH)/make/project.mk

monitor:
	$(Q) miniterm.py --rts 0 --dtr 0 --raw $(ESPPORT) 115200

%.nes:
	$(Q) $(ESPTOOLPY_SERIAL) write_flash -fs 4MB 0x100000 $@
