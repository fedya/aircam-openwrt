#
# Copyright (C) 2009 Ubiquiti Networks, Inc.
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=nuttcp
PKG_VERSION:=6.1.2
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2
PKG_SOURCE_URL:=ftp://ftp.lcp.nrl.navy.mil/pub/nuttcp/
PKG_MD5SUM:=a16d4d6d5def02cea980e57feaf30500

include $(INCLUDE_DIR)/package.mk

define Package/nuttcp/Default
  SUBMENU:=Monitoring
  SECTION:=net
  CATEGORY:=Network
  URL:=ftp://ftp.lcp.nrl.navy.mil/pub/nuttcp/
endef

define Package/nuttcp
  $(call Package/nuttcp/Default)
  MENU:=1
  TITLE:=Bandwidth measuring tool
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) CC="$(TARGET_CC)" OPT="$(TARGET_CFLAGS)"
endef

define Package/nuttcp/install
	$(INSTALL_DIR) $(1)/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/nuttcp-$(PKG_VERSION) $(1)/sbin/nuttcp
endef

$(eval $(call BuildPackage,nuttcp))
