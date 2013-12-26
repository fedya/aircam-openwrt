#
# Copyright (C) 2010 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk

ARCH:=arm
BOARD:=gm812x
BOARDNAME:=Grain Media GM812x
FEATURES:=squashfs gpio audio
CFLAGS:=-O2 -pipe -march=armv5te -msoft-float -fno-caller-saves
MAINTAINER:=Jonas Majauskas <jmajau@ubnt.com>

LINUX_VERSION:=2.6.28.fa2
INITRAMFS_EXTRA_FILES:=${CURDIR}/image/initramfs-base-files.txt

include $(INCLUDE_DIR)/target.mk

define Target/Description
	Build images for Grain Media GM812x  based boards
endef

KERNELNAME:="zImage"

SVN_REV:=$(shell svn info $(TOPDIR)/.. | grep "^Last Changed Rev:" | awk '{print $$4}')
ifeq ($(SVN_REV),)
	SVN_REV:=$(shell cd $(TOPDIR)/..; git svn info | grep "^Last Changed Rev:" | awk '{print $$4}'; cd - >/dev/null)
endif
ifeq ($(SVN_REV),)
	SVN_REV:=19999
endif
FULL_VERSION:=$(CONFIG_UBNT_FIRMWARE_VERSION).$(SVN_REV).$(shell date +%y%m%d.%H%M)
DSTAMP:=$(shell date +%m%d%H%M%Y)

define Kernel/TransferDirectory
	mkdir -p $(1)/$(3)
	tar -C $(1)/$(2) -cf - . | tar -C $(1)/$(3) -xf -
	rm -rf $(1)/$(2)
	ln -sf $(patsubst %..,%../,$(subst $(space),/,$(patsubst %,..,$(subst /,$(space), $(subst .,,$(dir $(2)))))))$(3) $(1)/$(2)
endef

define Kernel/RemoveUnused
	mkdir -p $(TARGET_DIR)/usr/etc
	if [ -d $(TARGET_DIR)/etc/modules.d ]; then mv $(TARGET_DIR)/etc/modules.d $(TARGET_DIR)/usr/etc/; fi
	if [ -d $(TARGET_DIR)/etc/ssh ]; then mv $(TARGET_DIR)/etc/ssh $(TARGET_DIR)/usr/etc/; fi

	#remove everything that is not needed for us
	rm -rf $(TARGET_DIR)/usr/lib/ipkg \
		$(TARGET_DIR)/usr/lib/firewall.awk \
		$(TARGET_DIR)/usr/lib/opkg

	rm -rf $(TARGET_DIR)/lib/network \
		$(TARGET_DIR)/lib/functions \
		$(TARGET_DIR)/lib/firstboot \
		$(TARGET_DIR)/lib/preinit \
		$(TARGET_DIR)/lib/upgrade

	rm -rf $(TARGET_DIR)/sbin/firstboot \
		$(TARGET_DIR)/sbin/hotplug-call \
		$(TARGET_DIR)/sbin/ifdown \
		$(TARGET_DIR)/sbin/ifup \
		$(TARGET_DIR)/sbin/led.sh \
		$(TARGET_DIR)/sbin/mount_root \
		$(TARGET_DIR)/sbin/sysupgrade \
		$(TARGET_DIR)/sbin/wifi

	rm -rf $(TARGET_DIR)/etc
	ln -s var/etc $(TARGET_DIR)/etc

	rm -rf $(TARGET_DIR)/tmp
	ln -s var/tmp $(TARGET_DIR)/tmp
endef

define Kernel/UpdateTimestamp
	mkdir -p $(1)/usr/lib
	echo $(FULL_VERSION) > $(1)/usr/lib/version
	$(SED) 's/^BDATE=/BDATE=$$(DSTAMP)/' $(1)/init
endef

define Kernel/FixModuleDir
	mkdir -p $(1)/lib/modules
	ln -s 2.6.28.fa2 $(1)/lib/modules/2.6.28
	if [ -e $(1)/lib/modules/gm ]; then \
		mv $(1)/lib/modules/gm/*.ko $(1)/lib/modules/2.6.28/; \
		rm -r $(1)/lib/modules/gm; \
	fi
endef

define Kernel/FixAll
	$(call Kernel/RemoveUnused)
	$(call Kernel/TransferDirectory,$(TARGET_DIR),sbin,bin)
	$(call Kernel/TransferDirectory,$(TARGET_DIR),usr/bin,bin)
	$(call Kernel/TransferDirectory,$(TARGET_DIR),usr/sbin,bin)
	$(call Kernel/TransferDirectory,$(TARGET_DIR),usr/lib,lib)
	$(call Kernel/UpdateTimestamp,$(TARGET_DIR))
	$(call Kernel/FixModuleDir,$(TARGET_DIR))
endef

define Kernel/CompileImage
	$(call Kernel/FixAll)
	$(call Kernel/CompileImage/Default)
endef

$(eval $(call BuildTarget))
