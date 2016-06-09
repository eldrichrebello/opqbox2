export

DEVICE_FAMILY = STM32F3xx
DEVICE_TYPE = STM32F373xC
STARTUP_FILE = stm32f373xc
SYSTEM_FILE = stm32f3xx

CMSIS = Drivers/CMSIS
CMSIS_DEVSUP = $(CMSIS)/Device/ST/$(DEVICE_FAMILY)/
CMSIS_OPT = -DSTM32F373xC -DUSE_HAL_DRIVER
OTHER_OPT = "-D__weak=__attribute__((weak))" "-D__packed=__attribute__((__packed__))" 
CPU = -mthumb -mcpu=cortex-m4 -mfloat-abi=softfp -mfpu=fpv4-sp-d16
SYSTEM = arm-none-eabi

LDSCRIPT = "STM32F373CC_FLASH.ld"

SRCDIR := Src/
INCDIR := Inc/

LIBDIR := Drivers/


LIBINC := -IInc
LIBINC += -IDrivers/STM32F3xx_HAL_Driver/Inc
LIBINC += -IDrivers/CMSIS/Include
LIBINC += -IDrivers/CMSIS/Device/ST/STM32F3xx/Include


LIBS := ./$(LIBDIR)/STM32F3xx_HAL_Driver/libstm32fw.a	   
	   
LIBS += -lm 
CC      = $(SYSTEM)-gcc
CCDEP   = $(SYSTEM)-gcc
LD      = $(SYSTEM)-gcc
AR      = $(SYSTEM)-ar
AS      = $(SYSTEM)-gcc
OBJCOPY = $(SYSTEM)-objcopy
OBJDUMP	= $(SYSTEM)-objdump
GDB		= $(SYSTEM)-gdb
SIZE	= $(SYSTEM)-size
  
# INCLUDES = -I$(SRCDIR) $(LIBINC)
INCLUDES = $(LIBINC)
CFLAGS  = $(CPU) $(CMSIS_OPT) $(OTHER_OPT) -Wall -fno-common -fno-strict-aliasing -O2 $(INCLUDES) -g -Wfatal-errors -g 
ASFLAGS = $(CFLAGS) -x assembler-with-cpp
LDFLAGS = -Wl,--gc-sections,-Map=$*.map,-cref -T $(LDSCRIPT) $(CPU)
ARFLAGS = cr
OBJCOPYFLAGS = -Obinary
OBJDUMPFLAGS = -S

STARTUP_OBJ = $(CMSIS_DEVSUP)/Source/startup_$(STARTUP_FILE).o
SYSTEM_OBJ = $(CMSIS_DEVSUP)/Source/system_$(SYSTEM_FILE).o

BIN = opq.bin

OBJS = $(sort \
 $(patsubst %.c,%.o,$(wildcard Src/*.c)) \
 $(patsubst %.s,%.o,$(wildcard Src/*.s)) \
 $(STARTUP_OBJ) \
 $(SYSTEM_OBJ))

all: $(BIN)

	
$(BIN): opq.out
	$(OBJCOPY) $(OBJCOPYFLAGS) opq.out $(BIN)
	$(OBJDUMP) $(OBJDUMPFLAGS) opq.out > opq.list
	$(SIZE) opq.out
	@echo Make finished

opq.out: $(LIBS) $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

$(LIBS): libs

libs:
	@$(MAKE) -C $(LIBDIR)

libclean: clean
	@$(MAKE) -C $(LIBDIR) clean

clean:
	-rm -f $(OBJS)
	-rm -f opq.list opq.out opq.hex opq.map opq.bin .depend

depend dep: .depend

include .depend

.depend: Src/*.c
	$(CCDEP) $(CFLAGS) -MM $^ | sed -e 's@.*.o:@Src/&@' > .depend 

.c.o:
	@echo cc $<
	@$(CC) $(CFLAGS) -c -o $@ $<

.s.o:
	@echo as $<
	@$(AS) $(ASFLAGS) -c -o $@ $<
