# 
# Copyright (C) 2006-2010 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=frammap
PKG_VERSION:=1.1
PKG_RELEASE:=2

include $(INCLUDE_DIR)/package.mk

define KernelPackage/frammap
  SUBMENU:=Device drivers
  TITLE:=GM812x Frame Mapper
  DEPENDS:= @(TARGET_gm812x)
  FILES:=$(PKG_BUILD_DIR)/frammap.ko
  #AUTOLOAD:=$(call AutoLoad,10,frammap)
endef

include $(INCLUDE_DIR)/kernel-defaults.mk

define KernelPackage/frammap/description
 GM812x frame mapper
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	cp src/*.[ch] src/Makefile $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) $(KERNEL_MAKEOPTS) SUBDIRS="$(PKG_BUILD_DIR)" modules
endef

$(eval $(call KernelPackage,frammap))
