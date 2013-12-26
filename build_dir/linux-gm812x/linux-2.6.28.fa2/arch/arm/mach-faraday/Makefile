#
# Makefile for the linux kernel.

# Object file lists.

obj-$(CONFIG_FTINTC010)		+= ftintc010.o
obj-$(CONFIG_CPU_FA_IDLE)	+= idle.o
obj-$(CONFIG_FTPCI100) 		+= ftpci100.o
obj-$(CONFIG_FTPMU010)		+= ftpmu010.o
obj-$(CONFIG_FTPWMTMR010)	+= ftpwmtmr010.o
obj-$(CONFIG_FTSCU000)		+= ftscu000.o
obj-$(CONFIG_FTSCU010)		+= ftscu010.o
obj-$(CONFIG_FTTMR010)		+= fttmr010.o

#
# All supported Faraday platforms are listed here
# reference to arch/arm/mach-faraday/Kconfig
#
faraday-platform-$(CONFIG_PLATFORM_A320)	:= platform-a320
faraday-platform-$(CONFIG_PLATFORM_A360)	:= platform-a360
faraday-platform-$(CONFIG_PLATFORM_A369)	:= platform-a369
faraday-platform-$(CONFIG_PLATFORM_AXI)		:= platform-axi
faraday-platform-$(CONFIG_PLATFORM_FMP626A320)	:= platform-fmp626a320

#
# Default platform directory set to A320
# TODO: Make this an error, should never happen unless the Kconfig or Makefile is wrong
ifeq ($(faraday-platform-y),)
faraday-platform-y := platform-a320
endif
PLATFORM_DIR   := $(faraday-platform-y)

core-y		+=  arch/arm/mach-faraday/$(PLATFORM_DIR)/

define create-platform-symlink
	mkdir -p arch/arm/mach-faraday/include/mach;					\
	if [ -L $@ ]; then								\
		platformlink=`readlink $@`;						\
	fi;										\
	if [ ! -e $@ ] || [ $$platformlink != $(PLATFORM_DIR) ]; then			\
		touch arch/arm/mach-faraday/include/mach/$(PLATFORM_DIR)/*;		\
	fi;										\
	echo '  SYMLINK $@ -> arch/arm/mach-faraday/include/mach/$(PLATFORM_DIR)';	\
	ln -fsn $(PLATFORM_DIR) $@;
endef

arch/arm/mach-faraday/include/mach/platform: FORCE
	$(Q)$(create-platform-symlink)

prepare: arch/arm/mach-faraday/include/mach/platform

