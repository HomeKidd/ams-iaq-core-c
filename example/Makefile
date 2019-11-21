PROGRAM = main

EXTRA_COMPONENTS = \
		 extras/i2c \
	 	 extras/iaq_core \


FLASH_SIZE ?= 32

include $(SDK_PATH)/common.mk


monitor:
	$(FILTEROUTPUT) --port $(ESPPORT) --baud 115200 --elf $(PROGRAM_OUT)
