ARCH=avr32
SCRIPT_NAME=elf
TEMPLATE_NAME=elf32
EXTRA_EM_FILE=avr32elf
OUTPUT_FORMAT="elf32-avr32"
GENERATE_SHLIB_SCRIPT=yes
MAXPAGESIZE=0x1000
TEXT_START_ADDR=0x00001000
NOP=0xd703d703

# This appears to place the GOT before the data section, which is
# essential for uClinux.  We don't use those .s* sections on AVR32
# anyway, so it shouldn't hurt for regular Linux either...
NO_SMALL_DATA=yes
