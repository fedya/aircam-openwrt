# 
# Copyright (C) 2009 Ubiquiti Networks Inc.
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=tdb
PKG_VERSION:=1.2.1
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)_$(PKG_VERSION).orig.tar.gz
PKG_SOURCE_URL:= \
	http://ftp.de.debian.org/debian/pool/main/t/tdb \
	http://ftp.debian.org/debian/pool/main/t/tdb \
        http://ftp.fr.debian.org/debian/pool/main/t/tdb \
        http://ftp.us.debian.org/debian/pool/main/t/tdb

PKG_MD5SUM:=73ea81282a82e5c959d9c082af2d0215

include $(INCLUDE_DIR)/package.mk

define Package/libtdb
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=Trivial Database
  URL:=http://tdb.samba.org
endef

TARGET_CFLAGS += $(FPIC)

define Build/Prepare
	$(call Build/Prepare/Default)
	(cd $(PKG_BUILD_DIR); make distclean; rm -rf config.cache)
endef

CONFIGURE_ARGS += \
	--with-shared-build-dir="$(PKG_INSTALL_DIR)/usr" \
	--disable-largefile --disable-python \

define Build/Configure
	$(call Build/Configure/Default)
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		DESTDIR="$(PKG_INSTALL_DIR)" \
		shared-build
endef

define Build/InstallDev
	mkdir -p $(1)/usr/include
	$(CP) $(PKG_INSTALL_DIR)/usr/include/tdb.h $(1)/usr/include/
	mkdir -p $(1)/usr/lib
	$(CP) -d $(PKG_INSTALL_DIR)/usr/lib/libtdb.so* $(1)/usr/lib/
endef

define Package/libtdb/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) -d $(PKG_INSTALL_DIR)/usr/lib/libtdb.so* $(1)/usr/lib/
	$(STRIP) $(1)/usr/lib/libtdb.so.*
endef

$(eval $(call BuildPackage,libtdb))
