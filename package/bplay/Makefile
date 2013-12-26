# Makefile for bplay
# created by Dan Brown, 2009

include $(TOPDIR)/rules.mk

PKG_NAME:=bplay
PKG_VERSION:=0.991
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)_$(PKG_VERSION).orig.tar.gz
PKG_SOURCE_URL:=http://ftp.de.debian.org/debian/pool/main/b/bplay
PKG_MD5SUM:=7bb4c7bbd81b83f403b994f8faaad113

include $(INCLUDE_DIR)/package.mk

define Package/bplay
	SECTION:=sound
	CATEGORY:=Sound
	TITLE:=bplay is a buffered audio file player/recorder
endef

define Package/bplay/description
       The bplay package provides a simple command-line
       utility for playing and recording audio files in
       raw sample, VOC and WAV formats. To use this
       program you need a soundcard of some kind and the
       appropriate driver configured into your kernel.
       When run the program creates two processes which
       share a memory buffer. It does reading/writing on
       the disk and the sound device simultaneously, in
       order to be less liable to `pause' because the
       disk is too slow or too busy.
endef

define Package/bplay/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(CP) $(PKG_BUILD_DIR)/{bplay,brec} $(1)/usr/bin/
endef

$(eval $(call BuildPackage,bplay))
