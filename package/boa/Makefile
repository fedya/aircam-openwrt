# 
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_CVS_USER:=anonymous
PKG_CVS_PASS:=
PKG_CVS_ROOT:=":pserver:$(PKG_CVS_USER):$(PKG_CVS_PASS)@cvs.uclinux.org:/var/cvs"
PKG_CVS_MODULE:=uClinux-dist/user/boa
PKG_CVS_TAG:=uClinux-dist-20051014

PKG_NAME:=boa
PKG_VERSION:=0.93.17-$(PKG_CVS_TAG)
PKG_RELEASE:=1
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)
PKG_INSTALL_DIR:=$(PKG_BUILD_DIR)/ipkg-install

#make single dependency from matrixssl for now because of problems with || style alternative dependencies
#PKG_BUILD_DEPENDS:=+libmatrixssl +libopenssl

include $(INCLUDE_DIR)/package.mk

#this is workaround for cvs checkout...
#TODO: consider replacing it with $(PKG_UNPACK) variable...
download: $(DL_DIR)/$(PKG_SOURCE)

$(DL_DIR)/$(PKG_SOURCE):	
	mkdir -p $(DL_DIR)
	cvs -d$(PKG_CVS_ROOT) login
	(cd $(DL_DIR); \
		cvs -d$(PKG_CVS_ROOT) -z5 export -r $(PKG_CVS_TAG) \
			-d $(PKG_NAME)-$(PKG_VERSION) $(PKG_CVS_MODULE))
	cvs -d$(PKG_CVS_ROOT) logout
	tar -C $(DL_DIR) -zcvf $(DL_DIR)/$(PKG_SOURCE) $(PKG_NAME)-$(PKG_VERSION)
	rm -rf $(DL_DIR)/$(PKG_NAME)-$(PKG_VERSION)

$(STAMP_PREPARED): $(DL_DIR)/$(PKG_SOURCE)

define Package/boa
  SECTION:=base
  CATEGORY:=Base system
  TITLE:=Boa Webserver
  DEPENDS:=+libmatrixssl
  #DEPENDS:=@PACKAGE_libmatrixssl||PACKAGE_libopenssl
  URL:=http://www.boa.org/
endef

define Package/boa/description
	Boa is a single-tasking HTTP server.
endef

define Build/Configure
endef


MAKE_FLAGS += CONFIG_USER_BOA_WITH_SSL=1 CONFIG_USER_FORCE_REDIRECT_SSL=1
MAKE_PATH:=src
#ifeq ($(CONFIG_PACKAGE_libopenssl),)
MAKE_FLAGS += CONFIG_USER_BOA_WITH_MATRIX_SSL_COMPAT=1
#endif

define Build/Compile
	$(call Build/Compile/Default, )
	$(STRIP) $(PKG_BUILD_DIR)/$(MAKE_PATH)/boa
endef

define Package/boa/install
	$(INSTALL_DIR) $(1)/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/boa $(1)/bin/boa
endef	

$(eval $(call BuildPackage,boa))

$(eval $(call RequireCommand,cvs, \
        $(PKG_NAME) requires cvs. \
))

