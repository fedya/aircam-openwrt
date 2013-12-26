#
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=udpxy
PKG_VERSION:=1.0-Chipmunk-16
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME).$(PKG_VERSION).tgz
PKG_SOURCE_URL:=@SF/$(PKG_NAME)
PKG_MD5SUM:=0bba6fd3e0b0065424b198abdf0acb7d

include $(INCLUDE_DIR)/package.mk

define Package/udpxy
  SECTION:=net
  CATEGORY:=Network
  TITLE:=Multicast Relay Daemon
  URL:=http://sourceforge.net/projects/udpxy
endef

define Package/udpxy/description
	small-footprint UNIX/Linux daemon to relay 
	multicast UDP traffic to client's TCP (HTTP) connection.
endef

#TARGET_CFLAGS += -Dlog=igmpproxy_log

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		CC="$(TARGET_CC)" \
		CFLAGS="$(TARGET_CFLAGS) -std=gnu99"
endef

define Package/udpxy/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/udpxy $(1)/usr/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/udpxrec $(1)/usr/sbin/
endef

$(eval $(call BuildPackage,udpxy))
