# 
# Copyright (C) 2011 Ubiquiti Networks, Inc
#
include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=mac-telnet
PKG_VERSION:=0.1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/mac-telnet
  SECTION:=net
  CATEGORY:=Base system
  TITLE:=Kind of RouterOS Mac-Telnet application for Linux users
  URL:=http://lunatic.no/2010/10/routeros-mac-telnet-application-for-linux-users/
endef

define Package/mac-telnet/description
 It is intended to provide RouterOS Mac-Telnet client/server applications.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		CC="$(TARGET_CC)" \
		CCFLAGS="$(TARGET_CFLAGS)"
endef

define Package/mac-telnet/install
	$(INSTALL_DIR) $(1)/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/mactelnet $(1)/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/mactelnetd $(1)/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/macping $(1)/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/mndp $(1)/sbin/
endef

$(eval $(call BuildPackage,mac-telnet))

include $(TOPDIR)/rules.mk
