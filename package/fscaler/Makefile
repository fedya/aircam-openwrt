include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=fscaler
PKG_VERSION:=1.0

PKG_BUILD_DEPENDS:=dvr

include $(INCLUDE_DIR)/package.mk

define KernelPackage/fscaler
  SUBMENU:=Device drivers
  TITLE:=GM8126 Scaler
  FILES:=$(PKG_BUILD_DIR)/fscaler0.$(LINUX_KMOD_SUFFIX)
  AUTOLOAD:=$(call AutoLoad,50,fscaler0)
endef

define KernelPackage/fscaler/description
 GM8126 Scaler
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	cp src/Makefile src/*.[ch] $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) -C $(LINUX_DIR) \
		CROSS_COMPILE="$(TARGET_CROSS)" \
		ARCH="$(LINUX_KARCH)" \
		SUBDIRS="$(PKG_BUILD_DIR)" \
		KERNELDIR=$(LINUX_DIR) \
		CC="$(TARGET_CC)" \
		EXTRA_CFLAGS="-I$(STAGING_DIR)/include/videograph -I$(STAGING_DIR)/include/scaler"
		modules
endef

$(eval $(call KernelPackage,fscaler))
