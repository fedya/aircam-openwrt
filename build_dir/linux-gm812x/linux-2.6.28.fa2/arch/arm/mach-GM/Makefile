#
# Makefile for the linux kernel.

# Object file lists.

obj-y += ftintc010.o ftpmu010.o fttmr010.o ahb_dma.o memory.o fmem.o
obj-$(CONFIG_CPU_FA_IDLE)       += idle.o
obj-$(CONFIG_MFIQ)              += mfiq.o
#obj-$(CONFIG_ISOLATE_HIGHMEM)   += memory.o
#
# All supported GM platforms are listed here
# reference to arch/arm/mach-GM/Kconfig
#
GM-platform-$(CONFIG_PLATFORM_GM8181) 		:= platform-GM8181
GM-platform-$(CONFIG_PLATFORM_GM8126)		:= platform-GM8126
#
# Default platform directory set to GM8181
# TODO: Make this an error, should never happen unless the Kconfig or Makefile is wrong
ifeq ($(GM-platform-y),)
GM-platform-y := platform-GM8181
endif
PLATFORM_DIR   := $(GM-platform-y)

core-y		+=  arch/arm/mach-GM/$(PLATFORM_DIR)/

define create-platform-symlink
	mkdir -p arch/arm/mach-GM/include/mach;					\
	if [ -L $@ ]; then								\
		platformlink=`readlink $@`;						\
	fi;										\
	if [ ! -e $@ ] || [ $$platformlink != $(PLATFORM_DIR) ]; then			\
		touch arch/arm/mach-GM/include/mach/$(PLATFORM_DIR)/*;		\
	fi;										\
	echo '  SYMLINK $@ -> arch/arm/mach-GM/include/mach/$(PLATFORM_DIR)';	\
	ln -fsn $(PLATFORM_DIR) $@;
endef

arch/arm/mach-GM/include/mach/platform: FORCE
	$(Q)$(create-platform-symlink)

prepare: arch/arm/mach-GM/include/mach/platform

