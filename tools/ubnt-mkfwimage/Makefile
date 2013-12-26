# 
# Copyright (C) 2007 Ubiquiti Networks, Inc
#
include $(TOPDIR)/rules.mk

PKG_NAME:=ubnt-mkfwimage

include $(INCLUDE_DIR)/host-build.mk

define Host/Compile
	$(CC) -O src/ubnt-mkfwimage.c -o $(HOST_BUILD_DIR)/ubnt-mkfwimage -lz
	$(CC) -O src/fwsplit.c -o $(HOST_BUILD_DIR)/fwsplit -lz
endef

define Host/Install
	$(INSTALL_BIN) $(HOST_BUILD_DIR)/ubnt-mkfwimage $(STAGING_DIR_HOST)/bin/
	# mkdir -p $(STAGING_DIR)/usr/include/$(PKG_NAME)
	# $(CP) src/*.h $(STAGING_DIR)/usr/include/$(PKG_NAME)/
endef

define Host/Clean
	rm -f $(STAGING_DIR)/bin/ubnt-mkfwimage
endef

$(eval $(call HostBuild))
