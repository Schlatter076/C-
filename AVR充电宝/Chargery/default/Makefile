###############################################################################
# Makefile for the project train
###############################################################################

## General Flags
PROJECT = train
MCU = atmega32
TARGET = charger.elf
CC = avr-gcc

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2                                                                        -DF_CPU=16000000  -Os  -funsigned-char  -funsigned-bitfields  -fpack-struct  -fshort-enums
CFLAGS += -Wp,-M,-MP,-MT,$(*F).o,-MF,dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=charger.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0


## Include Directories
INCLUDES = -I"D:\Chargery\." -I"D:\Chargery\code" -I"D:\Chargery\code\config" -I"D:\Chargery\code\cpu" -I"D:\Chargery\code\info" -I"D:\Chargery\code\main" -I"D:\Chargery\code\misc" -I"D:\Chargery\code\rs485" -I"D:\Chargery\code\info\detect" -I"D:\Chargery\code\info\display" -I"C:\WinAVR\avr\include" 

## Objects that must be built in order to link
OBJECTS = battery.o comm.o adc.o dbus.o eeprom.o key.o port.o pwm.o timer0.o uart0.o lcd.o main.o menu.o misc.o cpuinit.o string.o Li.o Ni.o Pb.o items.o charger.o para.o sets.o limit.o balance.o scroll.o pmu.o 

## Build
all: $(TARGET) charger.hex charger.eep charger.lss

## Compile
battery.o: ../code/battery/battery.c
	$(CC) $(INCLUDES) $(CFLAGS) -c   $< -o $@

comm.o: ../code/comm/comm.c
	$(CC) $(INCLUDES) $(CFLAGS) -c   $< -o $@

adc.o: ../code/cpu/adc.c
	$(CC) $(INCLUDES) $(CFLAGS) -c   $< -o $@

dbus.o: ../code/cpu/dbus.c
	$(CC) $(INCLUDES) $(CFLAGS) -c   $< -o $@

eeprom.o: ../code/cpu/eeprom.c
	$(CC) $(INCLUDES) $(CFLAGS) -c   $< -o $@

key.o: ../code/cpu/key.c
	$(CC) $(INCLUDES) $(CFLAGS) -c   $< -o $@

port.o: ../code/cpu/port.c
	$(CC) $(INCLUDES) $(CFLAGS) -c   $< -o $@

pwm.o: ../code/cpu/pwm.c
	$(CC) $(INCLUDES) $(CFLAGS) -c   $< -o $@

timer0.o: ../code/cpu/timer0.c
	$(CC) $(INCLUDES) $(CFLAGS) -c   $< -o $@

uart0.o: ../code/cpu/uart0.c
	$(CC) $(INCLUDES) $(CFLAGS) -c   $< -o $@

lcd.o: ../code/lcd/lcd.c
	$(CC) $(INCLUDES) $(CFLAGS) -c   $< -o $@

main.o: ../code/main/main.c
	$(CC) $(INCLUDES) $(CFLAGS) -c   $< -o $@

menu.o: ../code/menu/menu.c
	$(CC) $(INCLUDES) $(CFLAGS) -c   $< -o $@

misc.o: ../code/misc/misc.c
	$(CC) $(INCLUDES) $(CFLAGS) -c   $< -o $@

cpuinit.o: ../code/cpu/cpuinit.c
	$(CC) $(INCLUDES) $(CFLAGS) -c   $< -o $@

string.o: ../code/misc/string.c
	$(CC) $(INCLUDES) $(CFLAGS) -c   $< -o $@

Li.o: ../code/battery/Li.c
	$(CC) $(INCLUDES) $(CFLAGS) -c   $< -o $@

Ni.o: ../code/battery/Ni.c
	$(CC) $(INCLUDES) $(CFLAGS) -c   $< -o $@

Pb.o: ../code/battery/Pb.c
	$(CC) $(INCLUDES) $(CFLAGS) -c   $< -o $@

items.o: ../code/battery/items.c
	$(CC) $(INCLUDES) $(CFLAGS) -c   $< -o $@

charger.o: ../code/charger/charger.c
	$(CC) $(INCLUDES) $(CFLAGS) -c   $< -o $@

para.o: ../code/charger/para.c
	$(CC) $(INCLUDES) $(CFLAGS) -c   $< -o $@

sets.o: ../code/battery/sets.c
	$(CC) $(INCLUDES) $(CFLAGS) -c   $< -o $@

limit.o: ../code/battery/limit.c
	$(CC) $(INCLUDES) $(CFLAGS) -c   $< -o $@

balance.o: ../code/charger/balance.c
	$(CC) $(INCLUDES) $(CFLAGS) -c   $< -o $@

scroll.o: ../code/battery/scroll.c
	$(CC) $(INCLUDES) $(CFLAGS) -c   $< -o $@

pmu.o: ../code/charger/pmu.c
	$(CC) $(INCLUDES) $(CFLAGS) -c   $< -o $@

##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) charger.elf dep/ charger.hex charger.eep charger.lss charger.map


## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)

