Source-Makefile: feeds/packages/net/asterisk-1.8.x/Makefile
Package: asterisk18
Menu: 1
Submenu: Telephony
Version: 1.8.3.2-2
Depends: +libopenssl +libncurses +libpopt +libpthread +zlib @!TARGET_avr32
Menu-Depends: 
Provides: 
Section: net
Category: Network
Title: Complete open source PBX, v1.8x
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.8.3.2.tar.gz
Type: ipkg
Description:  Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
http://www.asterisk.org/
Hans Zandbelt <hans.zandbelt@gmail.com>
@@

Package: asterisk18-voicemail
Submenu: Telephony
Version: 1.8.3.2-2
Depends: asterisk18
Menu-Depends: 
Provides: 
Section: net
Category: Network
Title: Voicemail support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.8.3.2.tar.gz
Type: ipkg
Description:  Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
 This package contains voicemail related modules for Asterisk.
http://www.asterisk.org/
Hans Zandbelt <hans.zandbelt@gmail.com>
@@

Package: asterisk18-sounds
Submenu: Telephony
Version: 1.8.3.2-2
Depends: asterisk18
Menu-Depends: 
Provides: 
Section: net
Category: Network
Title: Sound files
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.8.3.2.tar.gz
Type: ipkg
Description:  Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
 This package contains sound files for Asterisk.
http://www.asterisk.org/
Hans Zandbelt <hans.zandbelt@gmail.com>
@@

Package: asterisk18-chan-iax2
Submenu: Telephony
Version: 1.8.3.2-2
Depends: asterisk18 +asterisk18-res-crypto
Menu-Depends: 
Provides: 
Section: net
Category: Network
Title: IAX support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.8.3.2.tar.gz
Type: ipkg
Description:  Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
 This package provides IAX support to
 Asterisk.
http://www.asterisk.org/
Hans Zandbelt <hans.zandbelt@gmail.com>
@@

Package: asterisk18-cdr
Submenu: Telephony
Version: 1.8.3.2-2
Depends: asterisk18
Menu-Depends: 
Provides: 
Section: net
Category: Network
Title: CDR support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.8.3.2.tar.gz
Type: ipkg
Description:  Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
 This package provides Call Detail Record support to
 Asterisk.
http://www.asterisk.org/
Hans Zandbelt <hans.zandbelt@gmail.com>
@@

Package: asterisk18-res-musiconhold
Submenu: Telephony
Version: 1.8.3.2-2
Depends: asterisk18
Menu-Depends: 
Provides: 
Section: net
Category: Network
Title: MOH support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.8.3.2.tar.gz
Type: ipkg
Description:  Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
 This package provides Music On Hold support to
 Asterisk.
http://www.asterisk.org/
Hans Zandbelt <hans.zandbelt@gmail.com>
@@

Package: asterisk18-chan-gtalk
Submenu: Telephony
Version: 1.8.3.2-2
Depends: asterisk18 +libiksemel
Menu-Depends: 
Provides: 
Section: net
Category: Network
Title: GTalk support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.8.3.2.tar.gz
Type: ipkg
Description:  Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
 This package provides the channel chan_gtalk and res_jabber for GTalk
 support to Asterisk.
http://www.asterisk.org/
Hans Zandbelt <hans.zandbelt@gmail.com>
@@

Package: asterisk18-chan-mobile
Submenu: Telephony
Version: 1.8.3.2-2
Depends: asterisk18 +bluez-libs
Menu-Depends: 
Provides: 
Section: net
Category: Network
Title: Mobile channel support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.8.3.2.tar.gz
Type: ipkg
Description:  Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
 This package provides the channel chan_mobile support to Asterisk.
http://www.asterisk.org/
Hans Zandbelt <hans.zandbelt@gmail.com>
@@

Package: asterisk18-chan-mgcp
Submenu: Telephony
Version: 1.8.3.2-2
Depends: asterisk18
Menu-Depends: 
Provides: 
Section: net
Category: Network
Title: MGCP channel support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.8.3.2.tar.gz
Type: ipkg
Description:  Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
 This package provides the channel chan_mgcp support to Asterisk.
http://www.asterisk.org/
Hans Zandbelt <hans.zandbelt@gmail.com>
@@

Package: asterisk18-curl
Submenu: Telephony
Version: 1.8.3.2-2
Depends: asterisk18 +libcurl
Menu-Depends: 
Provides: 
Section: net
Category: Network
Title: CURL support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.8.3.2.tar.gz
Type: ipkg
Description:  Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
 This package provides CURL
 support to Asterisk.
http://www.asterisk.org/
Hans Zandbelt <hans.zandbelt@gmail.com>
@@

Package: asterisk18-mysql
Submenu: Telephony
Version: 1.8.3.2-2
Depends: asterisk18 +libmysqlclient
Menu-Depends: 
Provides: 
Section: net
Category: Network
Title: MySQL support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.8.3.2.tar.gz
Type: ipkg
Description:  Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
 This package provides MySQL
 support to Asterisk.
http://www.asterisk.org/
Hans Zandbelt <hans.zandbelt@gmail.com>
@@

Package: asterisk18-app-authenticate
Submenu: Telephony
Version: 1.8.3.2-2
Depends: asterisk18 
Menu-Depends: 
Provides: 
Section: net
Category: Network
Title: Authenticate support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.8.3.2.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support support for executing arbitrary authenticate commands in Asterisk.
http://www.asterisk.org/
Hans Zandbelt <hans.zandbelt@gmail.com>
@@

Package: asterisk18-app-chanisavail
Submenu: Telephony
Version: 1.8.3.2-2
Depends: asterisk18 
Menu-Depends: 
Provides: 
Section: net
Category: Network
Title: Channel availability check support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.8.3.2.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support support for checking if a channel is available in Asterisk.
http://www.asterisk.org/
Hans Zandbelt <hans.zandbelt@gmail.com>
@@

Package: asterisk18-app-chanspy
Submenu: Telephony
Version: 1.8.3.2-2
Depends: asterisk18 
Menu-Depends: 
Provides: 
Section: net
Category: Network
Title: Channel listen in support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.8.3.2.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support support for listening in on any channel in Asterisk.
http://www.asterisk.org/
Hans Zandbelt <hans.zandbelt@gmail.com>
@@

Package: asterisk18-app-directed-pickup
Submenu: Telephony
Version: 1.8.3.2-2
Depends: asterisk18 
Menu-Depends: 
Provides: 
Section: net
Category: Network
Title: Directed call pickup support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.8.3.2.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support support for directed call pickup in Asterisk.
http://www.asterisk.org/
Hans Zandbelt <hans.zandbelt@gmail.com>
@@

Package: asterisk18-app-exec
Submenu: Telephony
Version: 1.8.3.2-2
Depends: asterisk18 
Menu-Depends: 
Provides: 
Section: net
Category: Network
Title: Exec application support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.8.3.2.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support support for application execution in Asterisk.
http://www.asterisk.org/
Hans Zandbelt <hans.zandbelt@gmail.com>
@@

Package: asterisk18-app-minivm
Submenu: Telephony
Version: 1.8.3.2-2
Depends: asterisk18 
Menu-Depends: 
Provides: 
Section: net
Category: Network
Title: Minimal voicemail system support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.8.3.2.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support a voicemail system in small building blocks working together based on the Comedian Mail voicemail system in Asterisk.
http://www.asterisk.org/
Hans Zandbelt <hans.zandbelt@gmail.com>
@@

Package: asterisk18-app-readexten
Submenu: Telephony
Version: 1.8.3.2-2
Depends: asterisk18 
Menu-Depends: 
Provides: 
Section: net
Category: Network
Title: Extension to variable support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.8.3.2.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support a trivial application to read an extension into a variable in Asterisk.
http://www.asterisk.org/
Hans Zandbelt <hans.zandbelt@gmail.com>
@@

Package: asterisk18-app-read
Submenu: Telephony
Version: 1.8.3.2-2
Depends: asterisk18 
Menu-Depends: 
Provides: 
Section: net
Category: Network
Title: Variable read support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.8.3.2.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support a trivial application to read a variable in Asterisk.
http://www.asterisk.org/
Hans Zandbelt <hans.zandbelt@gmail.com>
@@

Package: asterisk18-app-sayunixtime
Submenu: Telephony
Version: 1.8.3.2-2
Depends: asterisk18 
Menu-Depends: 
Provides: 
Section: net
Category: Network
Title: Say Unix time support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.8.3.2.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support an application to say Unix time in Asterisk.
http://www.asterisk.org/
Hans Zandbelt <hans.zandbelt@gmail.com>
@@

Package: asterisk18-app-sms
Submenu: Telephony
Version: 1.8.3.2-2
Depends: asterisk18 
Menu-Depends: 
Provides: 
Section: net
Category: Network
Title: SMS support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.8.3.2.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support SMS support (ETSI ES 201 912 protocol 1) in Asterisk.
http://www.asterisk.org/
Hans Zandbelt <hans.zandbelt@gmail.com>
@@

Package: asterisk18-app-stack
Submenu: Telephony
Version: 1.8.3.2-2
Depends: asterisk18  +asterisk18-res-agi
Menu-Depends: 
Provides: 
Section: net
Category: Network
Title: Stack applications support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.8.3.2.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support  stack applications Gosub Return etc. in Asterisk.
http://www.asterisk.org/
Hans Zandbelt <hans.zandbelt@gmail.com>
@@

Package: asterisk18-app-system
Submenu: Telephony
Version: 1.8.3.2-2
Depends: asterisk18 
Menu-Depends: 
Provides: 
Section: net
Category: Network
Title: System exec support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.8.3.2.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support support for executing system commands in Asterisk.
http://www.asterisk.org/
Hans Zandbelt <hans.zandbelt@gmail.com>
@@

Package: asterisk18-app-talkdetect
Submenu: Telephony
Version: 1.8.3.2-2
Depends: asterisk18 
Menu-Depends: 
Provides: 
Section: net
Category: Network
Title: File playback with audio detect support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.8.3.2.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support for file playback with audio detect in Asterisk.
http://www.asterisk.org/
Hans Zandbelt <hans.zandbelt@gmail.com>
@@

Package: asterisk18-app-waituntil
Submenu: Telephony
Version: 1.8.3.2-2
Depends: asterisk18 
Menu-Depends: 
Provides: 
Section: net
Category: Network
Title: Sleep support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.8.3.2.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support support sleeping until the given epoch in Asterisk.
http://www.asterisk.org/
Hans Zandbelt <hans.zandbelt@gmail.com>
@@

Package: asterisk18-app-while
Submenu: Telephony
Version: 1.8.3.2-2
Depends: asterisk18 
Menu-Depends: 
Provides: 
Section: net
Category: Network
Title: While loop support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.8.3.2.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support a while loop implementation in Asterisk.
http://www.asterisk.org/
Hans Zandbelt <hans.zandbelt@gmail.com>
@@

Package: asterisk18-chan-agent
Submenu: Telephony
Version: 1.8.3.2-2
Depends: asterisk18 
Menu-Depends: 
Provides: 
Section: net
Category: Network
Title: Agents proxy channel support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.8.3.2.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support  an implementation of agents proxy channel in Asterisk.
http://www.asterisk.org/
Hans Zandbelt <hans.zandbelt@gmail.com>
@@

Package: asterisk18-chan-local
Submenu: Telephony
Version: 1.8.3.2-2
Depends: asterisk18 
Menu-Depends: 
Provides: 
Section: net
Category: Network
Title: Local proxy channel support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.8.3.2.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support  an implementation of local proxy channel in Asterisk.
http://www.asterisk.org/
Hans Zandbelt <hans.zandbelt@gmail.com>
@@

Package: asterisk18-codec-alaw
Submenu: Telephony
Version: 1.8.3.2-2
Depends: asterisk18 
Menu-Depends: 
Provides: 
Section: net
Category: Network
Title: Signed linear to alaw translation support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.8.3.2.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support translation between signed linear and alaw codecs in Asterisk.
http://www.asterisk.org/
Hans Zandbelt <hans.zandbelt@gmail.com>
@@

Package: asterisk18-codec-ulaw
Submenu: Telephony
Version: 1.8.3.2-2
Depends: asterisk18 
Menu-Depends: 
Provides: 
Section: net
Category: Network
Title: Signed linear to ulaw translation support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.8.3.2.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support translation between signed linear and ulaw codecs in Asterisk.
http://www.asterisk.org/
Hans Zandbelt <hans.zandbelt@gmail.com>
@@

Package: asterisk18-codec-a-mu
Submenu: Telephony
Version: 1.8.3.2-2
Depends: asterisk18 
Menu-Depends: 
Provides: 
Section: net
Category: Network
Title: Alaw to ulaw translation support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.8.3.2.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support translation between alaw and ulaw codecs in Asterisk.
http://www.asterisk.org/
Hans Zandbelt <hans.zandbelt@gmail.com>
@@

Package: asterisk18-codec-g722
Submenu: Telephony
Version: 1.8.3.2-2
Depends: asterisk18 
Menu-Depends: 
Provides: 
Section: net
Category: Network
Title: G.722 support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.8.3.2.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support a high bit rate 48/56/64Kbps ITU standard codec in Asterisk.
http://www.asterisk.org/
Hans Zandbelt <hans.zandbelt@gmail.com>
@@

Package: asterisk18-codec-g726
Submenu: Telephony
Version: 1.8.3.2-2
Depends: asterisk18 
Menu-Depends: 
Provides: 
Section: net
Category: Network
Title: Signed linear to G.726 translation support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.8.3.2.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support translation between signed linear and ITU G.726-32kbps codecs in Asterisk.
http://www.asterisk.org/
Hans Zandbelt <hans.zandbelt@gmail.com>
@@

Package: asterisk18-format-g726
Submenu: Telephony
Version: 1.8.3.2-2
Depends: asterisk18 
Menu-Depends: 
Provides: 
Section: net
Category: Network
Title: G.726 support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.8.3.2.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support support for headerless G.726 16/24/32/40kbps data format in Asterisk.
http://www.asterisk.org/
Hans Zandbelt <hans.zandbelt@gmail.com>
@@

Package: asterisk18-format-g729
Submenu: Telephony
Version: 1.8.3.2-2
Depends: asterisk18 
Menu-Depends: 
Provides: 
Section: net
Category: Network
Title: G.729 support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.8.3.2.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support support for raw headerless G729 data in Asterisk.
http://www.asterisk.org/
Hans Zandbelt <hans.zandbelt@gmail.com>
@@

Package: asterisk18-format-sln
Submenu: Telephony
Version: 1.8.3.2-2
Depends: asterisk18 
Menu-Depends: 
Provides: 
Section: net
Category: Network
Title: Raw slinear format support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.8.3.2.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support support for raw slinear format in Asterisk.
http://www.asterisk.org/
Hans Zandbelt <hans.zandbelt@gmail.com>
@@

Package: asterisk18-format-sln16
Submenu: Telephony
Version: 1.8.3.2-2
Depends: asterisk18 
Menu-Depends: 
Provides: 
Section: net
Category: Network
Title: Raw slinear 16 format support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.8.3.2.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support support for Raw slinear 16 format in Asterisk.
http://www.asterisk.org/
Hans Zandbelt <hans.zandbelt@gmail.com>
@@

Package: asterisk18-func-db
Submenu: Telephony
Version: 1.8.3.2-2
Depends: asterisk18 
Menu-Depends: 
Provides: 
Section: net
Category: Network
Title: Database interaction support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.8.3.2.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support functions for interaction with the database in Asterisk.
http://www.asterisk.org/
Hans Zandbelt <hans.zandbelt@gmail.com>
@@

Package: asterisk18-func-devstate
Submenu: Telephony
Version: 1.8.3.2-2
Depends: asterisk18 
Menu-Depends: 
Provides: 
Section: net
Category: Network
Title: Blinky lights control support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.8.3.2.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support functions for manually controlled blinky lights in Asterisk.
http://www.asterisk.org/
Hans Zandbelt <hans.zandbelt@gmail.com>
@@

Package: asterisk18-func-vmcount
Submenu: Telephony
Version: 1.8.3.2-2
Depends: asterisk18 
Menu-Depends: 
Provides: 
Section: net
Category: Network
Title: vmcount dialplan support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.8.3.2.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support a vmcount dialplan function in Asterisk.
http://www.asterisk.org/
Hans Zandbelt <hans.zandbelt@gmail.com>
@@

Package: asterisk18-func-extstate
Submenu: Telephony
Version: 1.8.3.2-2
Depends: asterisk18 
Menu-Depends: 
Provides: 
Section: net
Category: Network
Title: Hinted extension state support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.8.3.2.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support retrieving the state of a hinted extension for dialplan control in Asterisk.
http://www.asterisk.org/
Hans Zandbelt <hans.zandbelt@gmail.com>
@@

Package: asterisk18-func-global
Submenu: Telephony
Version: 1.8.3.2-2
Depends: asterisk18 
Menu-Depends: 
Provides: 
Section: net
Category: Network
Title: Global variable support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.8.3.2.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support global variable dialplan functions in Asterisk.
http://www.asterisk.org/
Hans Zandbelt <hans.zandbelt@gmail.com>
@@

Package: asterisk18-func-shell
Submenu: Telephony
Version: 1.8.3.2-2
Depends: asterisk18 
Menu-Depends: 
Provides: 
Section: net
Category: Network
Title: Shell support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.8.3.2.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support support for shell execution in Asterisk.
http://www.asterisk.org/
Hans Zandbelt <hans.zandbelt@gmail.com>
@@

Package: asterisk18-pbx-ael
Submenu: Telephony
Version: 1.8.3.2-2
Depends: asterisk18 
Menu-Depends: 
Provides: 
Section: net
Category: Network
Title: Asterisk Extension Logic support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.8.3.2.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support support for symbolic Asterisk Extension Logic in Asterisk.
http://www.asterisk.org/
Hans Zandbelt <hans.zandbelt@gmail.com>
@@

Package: asterisk18-res-ael-share
Submenu: Telephony
Version: 1.8.3.2-2
Depends: asterisk18 
Menu-Depends: 
Provides: 
Section: net
Category: Network
Title: Shareable AEL code support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.8.3.2.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support support for shareable AEL code mainly between internal and external modules in Asterisk.
http://www.asterisk.org/
Hans Zandbelt <hans.zandbelt@gmail.com>
@@

Package: asterisk18-pbx-spool
Submenu: Telephony
Version: 1.8.3.2-2
Depends: asterisk18 
Menu-Depends: 
Provides: 
Section: net
Category: Network
Title: Call Spool support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.8.3.2.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support outgoing call spool support in Asterisk.
http://www.asterisk.org/
Hans Zandbelt <hans.zandbelt@gmail.com>
@@

Package: asterisk18-res-agi
Submenu: Telephony
Version: 1.8.3.2-2
Depends: asterisk18 
Menu-Depends: 
Provides: 
Section: net
Category: Network
Title: Asterisk Gateway Interface support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.8.3.2.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support support for the Asterisk Gateway Interface extension in Asterisk.
http://www.asterisk.org/
Hans Zandbelt <hans.zandbelt@gmail.com>
@@

Package: asterisk18-res-crypto
Submenu: Telephony
Version: 1.8.3.2-2
Depends: asterisk18 
Menu-Depends: 
Provides: 
Section: net
Category: Network
Title: Provide Crypto support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.8.3.2.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support Cryptographic Signature capability in Asterisk.
http://www.asterisk.org/
Hans Zandbelt <hans.zandbelt@gmail.com>
@@

Package: asterisk18-app-alarmreceiver
Submenu: Telephony
Version: 1.8.3.2-2
Depends: asterisk18 
Menu-Depends: 
Provides: 
Section: net
Category: Network
Title: Alarm receiver support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.8.3.2.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support Central Station Alarm receiver for Ademco Contact ID in Asterisk.
http://www.asterisk.org/
Hans Zandbelt <hans.zandbelt@gmail.com>
@@

Package: asterisk18-app-setcallerid
Submenu: Telephony
Version: 1.8.3.2-2
Depends: asterisk18 
Menu-Depends: 
Provides: 
Section: net
Category: Network
Title: Set callerid support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.8.3.2.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support support for setting callerid in Asterisk.
http://www.asterisk.org/
Hans Zandbelt <hans.zandbelt@gmail.com>
@@

Package: asterisk18-app-verbose
Submenu: Telephony
Version: 1.8.3.2-2
Depends: asterisk18 
Menu-Depends: 
Provides: 
Section: net
Category: Network
Title: Verbose logging support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.8.3.2.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support Verbose logging application in Asterisk.
http://www.asterisk.org/
Hans Zandbelt <hans.zandbelt@gmail.com>
@@

Package: asterisk18-func-channel
Submenu: Telephony
Version: 1.8.3.2-2
Depends: asterisk18 
Menu-Depends: 
Provides: 
Section: net
Category: Network
Title: Channel info support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.8.3.2.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support Channel info dialplan function in Asterisk.
http://www.asterisk.org/
Hans Zandbelt <hans.zandbelt@gmail.com>
@@

Package: asterisk18-func-blacklist
Submenu: Telephony
Version: 1.8.3.2-2
Depends: asterisk18  and see if it is blacklisted
Menu-Depends: 
Provides: 
Section: net
Category: Network
Title: Blacklist on callerid support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.8.3.2.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support looking up the callerid number in Asterisk.
http://www.asterisk.org/
Hans Zandbelt <hans.zandbelt@gmail.com>
@@

Package: asterisk18-app-originate
Submenu: Telephony
Version: 1.8.3.2-2
Depends: asterisk18 
Menu-Depends: 
Provides: 
Section: net
Category: Network
Title: Originate a call support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.8.3.2.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support originating an outbound call and connecting it to a specified extension or application in Asterisk.
http://www.asterisk.org/
Hans Zandbelt <hans.zandbelt@gmail.com>
@@

Package: asterisk18-func-uri
Submenu: Telephony
Version: 1.8.3.2-2
Depends: asterisk18 
Menu-Depends: 
Provides: 
Section: net
Category: Network
Title: URI encoding and decoding support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.8.3.2.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support Encodes and decodes URI-safe strings in Asterisk.
http://www.asterisk.org/
Hans Zandbelt <hans.zandbelt@gmail.com>
@@


