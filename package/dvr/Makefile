include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=dvr
PKG_VERSION:=1.1
PKG_RELEASE:=2

include $(INCLUDE_DIR)/package.mk

define KernelPackage/dvr
  SUBMENU:=Device drivers
  TITLE:=GM812x DVR
  DEPENDS:= @(TARGET_gm812x)
  FILES:=$(PKG_BUILD_DIR)/dvr_common.ko $(PKG_BUILD_DIR)/dvr_enc.ko
  #AUTOLOAD:=$(call AutoLoad,50,dvr)
endef

include $(INCLUDE_DIR)/kernel-defaults.mk

define KernelPackage/dvr/description
 GM812x DVR
endef

DVR_INC = -I$(PKG_BUILD_DIR)/inc/videograph
DVR_INC+= -I$(PKG_BUILD_DIR)/inc/dvr
DVR_INC+= -I$(PKG_BUILD_DIR)/inc/lcd200_v2

define Build/InstallDev
	$(CP) include/* $(STAGING_DIR)/include/
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	cp src/Makefile src/*.[ch] $(PKG_BUILD_DIR)/
	mkdir -p $(PKG_BUILD_DIR)/inc
	$(CP) include/* $(PKG_BUILD_DIR)/inc
endef

define Build/Compile
	$(MAKE) $(KERNEL_MAKEOPTS) EXTRA_CFLAGS="$(DVR_INC)" \
		SUBDIRS="$(PKG_BUILD_DIR)" modules
endef

$(eval $(call KernelPackage,dvr))
