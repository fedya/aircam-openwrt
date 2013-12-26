#
# USB peripheral controller drivers
#
ifeq ($(CONFIG_USB_GADGET_DEBUG),y)
	EXTRA_CFLAGS		+= -DDEBUG
endif

obj-$(CONFIG_USB_DUMMY_HCD)	+= dummy_hcd.o
#ifeq ($(CONFIG_GM_USB_DEVICE),y)
#
#ifeq ($(CONFIG_GM_FUSB220),y)
#ifeq ($(CONFIG_GM_USB_UDC),y)
#obj-$(CONFIG_GM_USB_DEVICE)+= GM_udc.o
#endif
#endif
#
##ifeq ($(CONFIG_GM_FOTG2XX),y)
##obj-$(CONFIG_GM_USB_DEVICE)+= GM_udc.o
##endif
#endif

#ifeq ($(CONFIG_GM_USB_UDC),y)
#obj-$(CONFIG_GM_USB_UDC)   += GM_udc.o
#endif

g_GM_udc-objs              := GM_udc.o
#ifeq ($(CONFIG_GM_FOTG2XX_NOHOST),y)
#ifeq ($(CONFIG_GM_USB_UDC),m)
ifneq ($(CONFIG_GM_FOTG2XX),y)
g_GM_udc-objs              += ../host/fotg2xx_opt.o
endif
#endif
#endif
obj-$(CONFIG_GM_USB_UDC)   += g_GM_udc.o

obj-$(CONFIG_USB_NET2280)	+= net2280.o
obj-$(CONFIG_USB_AMD5536UDC)	+= amd5536udc.o
obj-$(CONFIG_USB_PXA25X)	+= pxa25x_udc.o
obj-$(CONFIG_USB_PXA27X)	+= pxa27x_udc.o
obj-$(CONFIG_USB_GOKU)		+= goku_udc.o
obj-$(CONFIG_USB_OMAP)		+= omap_udc.o
obj-$(CONFIG_USB_LH7A40X)	+= lh7a40x_udc.o
obj-$(CONFIG_USB_S3C2410)	+= s3c2410_udc.o
obj-$(CONFIG_USB_AT91)		+= at91_udc.o
obj-$(CONFIG_USB_ATMEL_USBA)	+= atmel_usba_udc.o
obj-$(CONFIG_USB_FSL_USB2)	+= fsl_usb2_udc.o
obj-$(CONFIG_USB_M66592)	+= m66592-udc.o
obj-$(CONFIG_USB_FSL_QE)	+= fsl_qe_udc.o

#
# USB gadget drivers
#
ifeq ($(CONFIG_GM_USB_DEVICE),y)
ifeq ($(CONFIG_GM_FUSB220),y)
g_GM_zero-objs             := FTC_zero.o usbstring.o config.o epautoconf.o
else
g_GM_zero-objs             := GM_test.o usbstring.o config.o epautoconf.o
endif
endif

g_zero-objs			:= zero.o
g_ether-objs			:= ether.o
g_serial-objs			:= serial.o
g_midi-objs			:= gmidi.o
gadgetfs-objs			:= inode.o
g_file_storage-objs		:= file_storage.o
g_printer-objs			:= printer.o
g_cdc-objs			:= cdc2.o

obj-$(CONFIG_GM_PATH_TEST) += g_GM_zero.o 
obj-$(CONFIG_USB_ZERO)		+= g_zero.o
obj-$(CONFIG_USB_ETH)		+= g_ether.o
obj-$(CONFIG_USB_GADGETFS)	+= gadgetfs.o
obj-$(CONFIG_USB_FILE_STORAGE)	+= g_file_storage.o
obj-$(CONFIG_USB_G_SERIAL)	+= g_serial.o
obj-$(CONFIG_USB_G_PRINTER)	+= g_printer.o
obj-$(CONFIG_USB_MIDI_GADGET)	+= g_midi.o
obj-$(CONFIG_USB_CDC_COMPOSITE) += g_cdc.o

