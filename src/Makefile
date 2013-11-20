#stellarisware dir
TW_DIR=../../tivaware
LM4TOOLS_DIR=../../lm4tools
CFLAGS=-g -mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -Os -ffunction-sections -fdata-sections -MD -Wall -pedantic -DPART_TM4C123GH6PM -I$(TW_DIR) -DTARGET_IS_BLIZZARD_RA1 -Dgcc
CPPFLAGS=-fno-exceptions -fno-rtti
PREFIX=arm-none-eabi
CC=$(PREFIX)-gcc
CPP=$(PREFIX)-g++
LD=$(PREFIX)-ld
OBJCOPY=$(PREFIX)-objcopy

OBJ=startup_gcc.o ir-gadget.o $(TW_DIR)/utils/uartstdio.o
TARGET=ir-gadget

%.o: %.c
	$(CC) -c -std=c99 -o $@ $< $(CFLAGS)

%.o: %.cpp
	$(CPP) -c -std=c++0x -o $@ $< $(CPPFLAGS) $(CFLAGS)

a.out: $(OBJ)
	$(LD) -T $(TARGET).ld  --entry=ResetISR -o $@ $^ --gc-sections -L$(TW_DIR)/driverlib/gcc/ -ldriver

%.bin: a.out
	$(OBJCOPY) -O binary $< $@

burn: $(TARGET).bin
	sudo $(LM4TOOLS_DIR)/lm4flash/lm4flash $(TARGET).bin

tags:
	ctags -R . ../../tivaware/

clean:
	rm -f *.o *.d *.bin a.out tags
