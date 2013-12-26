# 
# Copyright (C) 2010 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/image.mk

define Image/Prepare
	cp $(LINUX_DIR)/arch/arm/boot/zImage $(KDIR)/zImage
endef

define Image/BuildKernel
	cp $(KDIR)/zImage $(BIN_DIR)/openwrt-$(BOARD)-zImage
endef

define Image/Build
	$(call Image/Build/$(1),$(1))
	cp ./files/fw-layout.txt $(BIN_DIR)
	[ -e ./files/u-boot.bin ] && cp ./files/u-boot.bin $(BIN_DIR) || echo "u-boot.bin not found. IGNORING..."
	[ -e ./files/rom.bin ] && cp ./files/rom.bin $(BIN_DIR) || echo "rom.bin not found. IGNORING..."
	[ -e ./files/nsboot.bin ] && cp ./files/nsboot.bin $(BIN_DIR) || echo "nsboot.bin not found. IGNORING..."
	cd $(BIN_DIR) && ubnt-mkfwimage -v `cat $(TARGET_DIR)/usr/lib/version` -o `sed -e 's/\([^\.]*\)\.[^\.]*\.\(.*\)/\1.\2/' $(TARGET_DIR)/usr/lib/version`.bin -i $(BIN_DIR)/fw-layout.txt
endef

define Image/Build/jffs2-64k
	dd if=$(KDIR)/root.$(1) of=$(BIN_DIR)/openwrt-$(BOARD)-$(1).img bs=65536 conv=sync
endef

define Image/Build/jffs2-128k
	dd if=$(KDIR)/root.$(1) of=$(BIN_DIR)/openwrt-$(BOARD)-$(1).img bs=131072 conv=sync
endef

define Image/Build/squashfs
	$(call prepare_generic_squashfs,$(KDIR)/root.squashfs)
	dd if=$(KDIR)/root.$(1) of=$(BIN_DIR)/openwrt-$(BOARD)-$(1).img bs=131072 conv=sync
endef

$(eval $(call BuildImage))
