#stellarisware dir
TW_DIR=../../tivaware
LM4TOOLS_DIR=../../lm4tools
CFLAGS=-g -mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -Os -ffunction-sections -fdata-sections -MD -Wall -pedantic -DPART_TM4C123GH6PM -I$(TW_DIR) -DTARGET_IS_BLIZZARD_RA1 -Dgcc
CPPFLAGS=-Dgcc -DPART_TM4C123GH6PM -DTARGET_IS_BLIZZARD_RA1 -I../../tivaware -std=c++11 -Wno-int-to-pointer-cast -O2 -nostdlib -ffreestanding -emit-llvm -fno-exceptions -fno-rtti
LLCFLAGS=-march=thumb -mcpu=cortex-m4
PREFIX=arm-none-eabi
CC=$(PREFIX)-gcc
CPP=$(PREFIX)-g++
LD=$(PREFIX)-ld
AS=$(PREFIX)-as
OBJCOPY=$(PREFIX)-objcopy

#clang -DPART_TM4C123GH6PM -DTARGET_IS_BLIZZARD_RA1 -I../../tivaware -std=c++11 -Wno-int-to-pointer-cast -O2 -nostdlib -nostartfiles -ffreestanding -emit-llvm -c ir-gadget.cpp -o xxx.bc
#llvm-link xxx.bc -o yyy.bc
#llc -march=thumb -mcpu=cortex-m4 yyy.bc -o xxx.s
OBJ=startup_gcc.o ir-gadget.o $(TW_DIR)/utils/uartstdio.o
TARGET=ir-gadget

%.o: %.c
	$(CC) -c -std=c99 -o $@ $< $(CFLAGS)

%.o: %.cpp
	clang $(CPPFLAGS) -c $< -o $@.bc
	llc $(LLCFLAGS) $@.bc -o $@.s
	$(AS) $@.s -o $@

a.out: $(OBJ)
	$(LD) -T $(TARGET).ld  --entry=ResetISR -o $@ $^ --gc-sections -L$(TW_DIR)/driverlib/gcc/ -ldriver

%.bin: a.out
	$(OBJCOPY) -O binary $< $@

burn: $(TARGET).bin
	sudo $(LM4TOOLS_DIR)/lm4flash/lm4flash $(TARGET).bin

tags:
	ctags -R . ../../tivaware/

clean:
	rm -f *.o *.d *.bin *.o.bc *.o.s a.out tags
