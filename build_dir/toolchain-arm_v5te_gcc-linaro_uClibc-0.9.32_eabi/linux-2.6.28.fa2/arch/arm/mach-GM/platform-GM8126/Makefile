#
# Makefile for the GM platform dependent files

# Object file lists.

obj-y		:= spec.o pmu.o timer_fixup.o

ifeq ($(CONFIG_SECOND_SPI_FLASH_COMBINED), y) 
obj-y		+= platform_2ed_spi_flash_com.o 
else
obj-y		+= platform.o
endif

ifeq ($(CONFIG_SPI_FTSSP010), y)
obj-y		+= spi_adapter.o
endif
