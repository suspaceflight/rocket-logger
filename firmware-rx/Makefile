# Name: Makefile
# Project: Digital Test Bed
# Author: Jon Sowman <jon@hexoc.com> 

# DEVICE ....... The AVR device you compile for
# CLOCK ........ Target AVR clock rate in Hertz
# OBJECTS ...... The object files created from your source files. This list is
#                usually the same as the list of source files with suffix ".o".
# PROGRAMMER ... Options to avrdude which define the hardware you use for
#                uploading to the AVR and the interface where this hardware
#                is connected.
# FUSES ........ Parameters for avrdude to flash the fuses appropriately.

TARGET     = rocket_rx
DEVICE     = atmega644p
CLOCK      = 12000000
PROG_ISP   = -c avrispmkII -P usb -B 5
PROG_ASP   = -c usbasp
PROG_232   = -c c232hm -B 5
SOURCES	   = $(wildcard *.c)

# Fuses for 12MHz external osc, no ckdiv, BOD disable
FUSES      = -U hfuse:w:0x9c:m -U lfuse:w:0xff:m

# Binaries for manipulating AVR binaries and sources
CC = avr-gcc
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
DUDE = avrdude
SIZE = avr-size
RM = rm -f

# End configuration

# Find all the source files and convert to a list of objects
OBJECTS = $(SOURCES:.c=.o)
AVRDUDE_ASP = $(DUDE) $(PROG_ASP) -p $(DEVICE)
AVRDUDE_ISP = $(DUDE) $(PROG_ISP) -p $(DEVICE)
AVRDUDE_232 = $(DUDE) $(PROG_232) -p $(DEVICE)

CFLAGS = -Wall -Wextra -pedantic -ansi -Os -gdwarf-2 -std=gnu99
COMPILE = $(CC) $(CFLAGS) -DF_CPU=$(CLOCK) -mmcu=atmega644

# Symbolic targets:
all:	$(TARGET).hex

%.o: %.c
	$(COMPILE) -c $< -o $@

%.o: %.S
	$(COMPILE) -x assembler-with-cpp -c $< -o $@

%.s: %.c
	$(COMPILE) -S $< -o $@

flash_asp: all
	$(AVRDUDE_ASP) -U flash:w:$(TARGET).hex:i

flash_isp: all
	$(AVRDUDE_ISP) -U flash:w:$(TARGET).hex:i
	
flash_232: all
	$(AVRDUDE_232) -U flash:w:$(TARGET).hex:i

eeprom: all
	$(AVRDUDE) -U eeprom:w:$(TARGET).eep:i

fuse:
	$(AVRDUDE) $(FUSES)

clean:
	$(RM) $(TARGET).hex $(TARGET).elf $(TARGET).eep $(OBJECTS)

# File targets:
$(TARGET).elf: $(OBJECTS)
	$(COMPILE) -o $(TARGET).elf $(OBJECTS)
	$(SIZE) -C --mcu=${DEVICE} $@

$(TARGET).hex: $(TARGET).elf
	rm -f $(TARGET).hex
	$(OBJCOPY) -j .text -j .data -O ihex $(TARGET).elf $(TARGET).hex
	$(OBJCOPY) -j .eeprom -O ihex $(TARGET).elf $(TARGET).eep
# If you have an EEPROM section, you must also create a hex file for the
# EEPROM and add it to the "flash" target.

# Targets for code debugging and analysis:
disasm:	$(TARGET).elf
	$(OBJDUMP) -d $(TARGET).elf

