MODULE := fscaler0
VERSION := v1.13

EXTRA_CFLAGS += -DPLATFORM_VERSION=8126
PLATFORM_OBJ += fscaler_gm8126.o

EXTRA_CFLAGS += -DMPFX=$(MODULE)
EXTRA_CFLAGS += -DMVER=$(VERSION)

## Interface support configurations
CONFIG_FSCDRV_VG=y
CONFIG_FSCDRV_IOCTL=n
CONFIG_FSCDRV_OSD=y
CONFIG_FSCDRV_PROC=y

## Hardware limitations, do not modify
DEFINES += -DFSCIP_MAX=1
DEFINES += -DFSCIP_INIT=1
DEFINES += -DFSCOSD_WIN_MAX=4
DEFINES += -DFSCOSD_PAL_MAX=7
DEFINES += -DFSCOSD_TRANS_MAX=4
DEFINES += -DFSCOSD_FONT_W_MIN=7
DEFINES += -DFSCOSD_FONT_W_MAX=12
DEFINES += -DFSCOSD_FONT_H_MIN=7
DEFINES += -DFSCOSD_FONT_H_MAX=18
DEFINES += -DFSCOSD_FONT_ROW_MAX=15
DEFINES += -DFSCOSD_FONT_COL_MAX=15
DEFINES += -DFSCOSD_FONT_MAX=64
DEFINES += -DFSCOSD_DISP_MAX=128

CONFIG_FSCDRV_DEBUG=n
CONFIG_FSCDRV_KPI=n

#################################################
#
# Configure rules
#
#################################################
ifeq ($(CONFIG_FSCDRV_PROC),y)
DEFINES += -DFSCDRV_PROC=1
else
DEFINES += -DFSCDRV_PROC=0
endif

ifeq ($(CONFIG_FSCDRV_DEBUG),y)
DEFINES += -DFSCDRV_DEBUG=1
else
DEFINES += -DFSCDRV_DEBUG=0
endif

ifeq ($(CONFIG_FSCDRV_KPI),y)
DEFINES += -DFSCDRV_KPI=1
else
DEFINES += -DFSCDRV_KPI=0
endif

## Software limitations
DEFINES += -DFSCFD_MAX=512
DEFINES += -DFSCFD_INIT=128
DEFINES += -DFSCDRV_TIMEOUT=40
DEFINES += -DFSCDRV_MAX_WIDTH=720
DEFINES += -DFSCDRV_MAX_HEIGHT=576
DEFINES += -DFSCDRV_JOBGROUP_MAX=4
DEFINES += -DFSCDRV_PMU_CLOCK=150
DEFINES += -DFSCDRV_MAX_RETRY=2

ifeq ($(CONFIG_FSCDRV_VG),y)
DEFINES += -DFSCDRV_VG=1
else
DEFINES += -DFSCDRV_VG=0
endif

ifeq ($(CONFIG_FSCDRV_IOCTL),y)
DEFINES += -DFSCDRV_IOCTL=1
else
DEFINES += -DFSCDRV_IOCTL=0
endif
DEFINES += -DFSCDRV_FRAMMAP=1

ifeq ($(CONFIG_FSCDRV_OSD),y)
DEFINES += -DFSCDRV_OSD=1
else
DEFINES += -DFSCDRV_OSD=0
endif

EXTRA_CFLAGS += $(DEFINES)

common-objs := fscaler_table.o fscaler_property.o fscaler_job.o fscaler_entity.o fscaler_device.o fscaler_module.o fscaler_driver.o $(PLATFORM_OBJ)
#interface-objs := fscaler_interface.o videograph_device.o  fscaler_vg.o fscaler_osd.o fscaler_proc.o fscaler_io.o
interface-objs := fscaler_interface.o videograph_device.o  fscaler_vg.o fscaler_osd.o fscaler_proc.o

$(MODULE)-objs-yy := $(common-objs)
$(MODULE)-objs-yy += $(interface-objs)
$(MODULE)-objs := $($(MODULE)-objs-yy)

modules-yy := $(MODULE).o

obj-m += $(modules-yy)
obj-m := $(sort $(obj-m))

ifeq ($(MAKING_MODULES),1)
-include $(TOPDIR)/Rules.make
endif
