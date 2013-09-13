#stellarisware dir
SW_DIR=../../stellarisware
LM4TOOLS_DIR=../../lm4tools

CFLAGS=-g -mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -Os -ffunction-sections -fdata-sections -MD -std=c99 -Wall -pedantic -DPART_LM4F120H5QR -I$(SW_DIR) -DTARGET_IS_BLIZZARD_RA1 -Dgcc
PREFIX=arm-none-eabi
CC=$(PREFIX)-gcc
LD=$(PREFIX)-ld
OBJCOPY=$(PREFIX)-objcopy

OBJ=startup_gcc.o ir-gadget.o
TARGET=ir-gadget

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

a.out: $(OBJ)
	$(LD) -T $(TARGET).ld  --entry ResetISR -o $@ $^ --gc-sections -L$(SW_DIR)/driverlib/gcc-cm4f/ -ldriver-cm4f

%.bin: a.out
	$(OBJCOPY) -O binary $< $@

burn: $(TARGET).bin
	sudo $(LM4TOOLS_DIR)/lm4flash/lm4flash $(TARGET).bin

clean:
	rm -f *.o *.d *.bin a.out
