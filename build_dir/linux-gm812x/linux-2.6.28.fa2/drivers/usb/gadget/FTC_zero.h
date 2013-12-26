#ifndef __ZERO_USB_H
#define __ZERO_USB_H
	
//******************** FTC_USB_user.H ********************************
#ifndef __USB_USER_H
#define __USB_USER_H

#define TRUE			1
#define FALSE			0

//////////////////////////////////////////
#define EP0MAXPACKETSIZE	0x40

#define maxCONFIG	2
#define maxINTERFACE	2

//////////////////////////////////////////
//-----------------------------------------------------------------------
//Full speed
#define FS_USB_SPEC_VER        	0X0200
#define FS_VENDOR_ID           	0X2310
#define FS_PRODUCT_ID          	0X5678
#define FS_DEVICE_RELEASE_NO   	0X0001

// device configuration:
#define FS_bDeviceClass         0X00
#define FS_bDeviceSubClass      0X00
#define FS_bDeviceProtocol      0X00
#define FS_CONFIGURATION_NUMBER 0X01

//High speed
//-----------------------------------------------------------------------
#define HS_USB_SPEC_VER        	0X0200
#define HS_VENDOR_ID           	0X2310
#define HS_PRODUCT_ID          	0X5678
#define HS_DEVICE_RELEASE_NO    0X0001

// device configuration:
#define HS_bDeviceClass         0X00
#define HS_bDeviceSubClass      0X00
#define HS_bDeviceProtocol      0X00
#define HS_CONFIGURATION_NUMBER 0X01

//-----------------------------------------------------------------------
#define Dev_iManufacturer		1
#define Dev_iProduct				2
#define Dev_iSerialNumber		3
#define Config1_iConfiguration		4
#define Intf0_iInterface			5

#define STRING_00_LENGTH			0x04
#define STRING_10_LENGTH			0x1a
#define STRING_20_LENGTH			0x22
#define STRING_30_LENGTH			0x2e
#define STRING_40_LENGTH			0x26
#define STRING_50_LENGTH			0x22
#define STRING_60_LENGTH			0x1e
#define STRING_70_LENGTH			0x10
#define STRING_80_LENGTH			0x0e

#endif /* __USB_USER_H  */

//******************** FTC_USB.H ********************************
#ifndef __USB_H
	#define __USB_H
#define ZERO_EP_NUM	4   // define AP EP number
#define MAX_BULK_SIZE	0x1000
#define MAX_INTR_SIZE	512
	

typedef enum 
{
	IDLE,
	STATE_CBW_SET_QUEUE,
	STATE_CBW_QUEUE_FINISH,
	STATE_CB_DATA_IN_SET_QUEUE,
	STATE_CB_DATA_IN_QUEUE_FINISH,
	STATE_CB_DATA_OUT_SET_QUEUE,
	STATE_CB_DATA_OUT_QUEUE_FINISH,
	STATE_CSW_SET_QUEUE,
	STATE_CSW_QUEUE_FINISH
} MassStorageState;


typedef enum 
{
	STATE_INTR_SET_QUEUE,
	STATE_INTR_QUEUE_FINISH
} INTR_TEST_STATE;

enum zero_state 
{
	ZERO_STATE_COMMAND_PHASE = -10,		// This one isn't used anywhere
	ZERO_STATE_DATA_PHASE,
	ZERO_STATE_STATUS_PHASE,

	ZERO_STATE_IDLE = 0,
	ZERO_STATE_ABORT_BULK_OUT,
	ZERO_STATE_RESET,
	ZERO_STATE_INTERFACE_CHANGE,
	ZERO_STATE_CONFIG_CHANGE,
	ZERO_STATE_DISCONNECT,
	ZERO_STATE_EXIT,
	ZERO_STATE_TERMINATED
};


/* Command Block Wrapper */
struct bulk_cb_wrap
{
	u32	Signature;		// Contains 'USBC'
	u32	Tag;			// Unique per command id
	u32	DataTransferLength;	// Size of the data
	u8	Flags;			// Direction in bit 7
	u8	Lun;			// LUN (normally 0)
	u8	Length;			// Of the CDB, <= MAX_COMMAND_SIZE
	u8	CDB[16];		// Command Data Block
};
#define USB_BULK_CB_WRAP_LEN	31
#define USB_BULK_CB_SIG		0x43425355	// Spells out USBC
#define USB_BULK_IN_FLAG	0x80

/* Command Status Wrapper */
struct bulk_cs_wrap 
{
	u32	Signature;		// Should = 'USBS'
	u32	Tag;			// Same as original command
	u32	Residue;		// Amount not transferred
	u8	Status;			// See below
};
#define USB_BULK_CS_WRAP_LEN	13
#define USB_BULK_CS_SIG		0x53425355	// Spells out 'USBS'
#define USB_STATUS_PASS		0
#define USB_STATUS_FAIL			1
#define USB_STATUS_PHASE_ERROR	2

enum zero_buffer_state 
{
	BUF_STATE_EMPTY = 0,
	BUF_STATE_FULL,
	BUF_STATE_BUSY
};

struct zero_buffhd 
{
	void	    *buf;
	dma_addr_t	 dma;
	volatile enum zero_buffer_state	state;
	struct zero_buffhd				*next;

	/* The NetChip 2280 is faster, and handles some protocol faults
	 * better, if we don't submit any short bulk-out read requests.
	 * So we will record the intended request length here. */
	unsigned int out_intended_length;

	struct usb_request	*inreq;
	volatile int		inreq_busy;
	struct usb_request	*outreq;
	volatile int		outreq_busy;
};

#endif /* __USB_H  */

//********************************************************************

#define REGISTERED		0
#define CLEAR_BULK_HALTS	1

/* Number of buffers we will use.  2 is enough for double-buffering */
#define NUM_BUFFERS	2

#define WAKE_UP_BY_BULK		0
#define WAKE_UP_BY_INTR_IN		1
#define WAKE_UP_BY_INTR_OUT	2

#define BULK_IN_COMPLETE		0
#define BULK_OUT_COMPLETE		1
#define INTR_IN_COMPLETE		2
#define INTR_OUT_COMPLETE		3

struct FTC_zero_dev 
{
	spinlock_t		     lock;
	struct usb_gadget	*gadget;
	
	struct usb_ep			*ep0;			// save gadget->ep0 into ZeroDev->ep0
	struct usb_request	*ep0req;		/* for control responses */
	const char		*ep0req_name;
	volatile unsigned int	ep0_req_tag;

	// for set config use
	u8			config, new_config;
	
	// For exception and thread handling
	enum zero_state		state;		
	unsigned int		exception_req_tag;
	struct task_struct	*thread_task;
	int			thread_pid;
	sigset_t		thread_signal_mask;
	struct completion	thread_notifier;
	wait_queue_head_t	thread_wqh;
	int			thread_wakeup_needed;

	// Bulk transfer buffer setup
	struct zero_buffhd	*next_buffhd_to_fill;
	struct zero_buffhd	*next_buffhd_to_drain;
	struct zero_buffhd buffhds[NUM_BUFFERS];
	// Bulk transfer test structure
	struct bulk_cb_wrap Bulk_CBW;
	struct bulk_cs_wrap Bulk_CSW;
	u32 u32VirtMemoryIndex;
	MassStorageState Mass_stor_state;
 	unsigned int		bulk_out_maxpacket;
	u8  u8VirtMemory[MAX_BULK_SIZE];
	
	// Interrupt transfer buffer setup
	struct zero_buffhd buffhds_intr_in;
	struct zero_buffhd buffhds_intr_out;
	// Interrupt related
	u8 u8InterruptArray[MAX_INTR_SIZE];
	u8 u8InterruptINCount;
	u8 u8Interrupt_TX_COUNT ;
	u8 u8Interrupt_RX_COUNT ;
	u8 u8InterruptOutCount ;
	u8 u8IntrIN_queue_set;
	u8 u8IntrOUT_queue_set;

	INTR_TEST_STATE Intr_IN_State;
	INTR_TEST_STATE Intr_OUT_State;

	u8 u8EP_Queue_Complete[4];
		
	///* filesem protects: backing files in use */
	//struct rw_semaphore	filesem;

	volatile int		intreq_busy;
	
	unsigned int		running : 1;
	unsigned int		bulk_in_enabled : 1;
	unsigned int		bulk_out_enabled : 1;
	unsigned int		intr_in_enabled : 1;
	unsigned int		intr_out_enabled : 1;
	unsigned int		short_packet_received : 1;
	
	unsigned long		atomic_bitflags;
	/* when configured, we have one of two configs:
	* - source data (in to host) and sink it (out from host)
	* - or loop it back (out from host back in to host)
	*/
	struct usb_request *req[ZERO_EP_NUM+1];
	struct usb_ep  *Bin_ep, *Bout_ep;
	struct usb_ep  *Iin_ep, *Iout_ep;

	u8 u8UsbConfigValue;
	u8 u8UsbInterfaceValue;
	u8 u8UsbInterfaceAlternateSetting;

	// Mass Storage related
	MassStorageState eUsbMassStorageState;

	//debug
	u8 u8LineCount;
};


/*------------------ device descriptor table define---------------------------------*/
/*
 * DESCRIPTORS ... most are static, but strings and (full) configuration
 * descriptors are built on demand.  Also the (static) config and interface
 * descriptors are adjusted during fsg_bind().
 */
#define EP0_BUFSIZE	4096

/* There is only one configuration. */
#define	CONFIG_VALUE		1

static const char EP_BULK_IN_NAME[] = "ep1-bulkin";
#define EP_BULK_IN_NUM			1
#define FS_BULK_IN_MAXPACKET	64
#define HS_BULK_IN_MAXPACKET	512

static const char EP_BULK_OUT_NAME[] = "ep2-bulkout";
#define EP_BULK_OUT_NUM		2
#define FS_BULK_OUT_MAXPACKET	64
#define HS_BULK_OUT_MAXPACKET	512

static const char EP_INTR_IN_NAME[] = "ep3-intin";
#define EP_INTR_IN_NUM			3
#define FS_INTR_IN_MAXPACKET	64
#define HS_INTR_IN_MAXPACKET	64

static const char EP_INTR_OUT_NAME[] = "ep4-intout";
#define EP_INTR_OUT_NUM		4
#define FS_INTR_OUT_MAXPACKET	64
#define HS_INTR_OUT_MAXPACKET	64

static struct usb_device_descriptor device_desc =
{
	.bLength =		sizeof device_desc,
	.bDescriptorType =	USB_DT_DEVICE,

	.bcdUSB =		__constant_cpu_to_le16(0x0200),
	.bDeviceClass =		FS_bDeviceClass,
	
	.bDeviceSubClass=0,
	.bDeviceProtocol=0,
	
#if 0
	// YPING:some info must be filled when "zero_bind()"
	.bMaxPacketSize0=EP0MAXPACKETSIZE,				// bMaxPacketSize0
#endif

	/* The next three values can be overridden by module parameters */
	.idVendor =		__constant_cpu_to_le16(FS_VENDOR_ID),
	.idProduct =		__constant_cpu_to_le16(FS_PRODUCT_ID),
	.bcdDevice =		__constant_cpu_to_le16(FS_DEVICE_RELEASE_NO),

	.iManufacturer =	Dev_iManufacturer,
	.iProduct =		Dev_iProduct,
	.iSerialNumber =	Dev_iSerialNumber,
	.bNumConfigurations =	FS_CONFIGURATION_NUMBER,
};

#ifndef	MAX_USB_POWER
/* Any hub supports this steady state bus power consumption */
#define MAX_USB_POWER	100	/* mA */
#endif

static struct usb_config_descriptor config_desc = 
{
	.bLength =		sizeof config_desc,
	.bDescriptorType =	USB_DT_CONFIG,

	/* wTotalLength adjusted during bind() */
	.bNumInterfaces =	1,
	.bConfigurationValue =	CONFIG_VALUE,
	.iConfiguration =	Config1_iConfiguration,
	.bmAttributes =		USB_CONFIG_ATT_ONE | USB_CONFIG_ATT_SELFPOWER,
	.bMaxPower =		(MAX_USB_POWER + 1) / 2,
};

/* There is only one interface. */
static struct usb_interface_descriptor intf_desc = 
{
	.bLength =		sizeof intf_desc,
	.bDescriptorType =	USB_DT_INTERFACE,

	.bInterfaceNumber = 0,
	.bAlternateSetting = 0,
	.bNumEndpoints =	4,		// Adjusted during bind()
	.bInterfaceClass =	0,
	.bInterfaceSubClass =	0,	// Adjusted during bind()
	.bInterfaceProtocol =	0,	// Adjusted during bind()
	.iInterface = Intf0_iInterface,
};

/* Three full-speed endpoint descriptors: bulk-in, bulk-out,
 * and interrupt-in, interrupt-out. */
static const struct usb_endpoint_descriptor fs_bulk_in_desc =
{
	.bLength = USB_DT_ENDPOINT_SIZE,
	.bDescriptorType = USB_DT_ENDPOINT,

	.bEndpointAddress = EP_BULK_IN_NUM | USB_DIR_IN,
	.bmAttributes = USB_ENDPOINT_XFER_BULK,
	.wMaxPacketSize = __constant_cpu_to_le16(FS_BULK_IN_MAXPACKET),
};

static const struct usb_endpoint_descriptor fs_bulk_out_desc = 
{
	.bLength =		USB_DT_ENDPOINT_SIZE,
	.bDescriptorType =	USB_DT_ENDPOINT,

	.bEndpointAddress =	EP_BULK_OUT_NUM,
	.bmAttributes =		USB_ENDPOINT_XFER_BULK,
	.wMaxPacketSize =	__constant_cpu_to_le16(FS_BULK_OUT_MAXPACKET),
};

static const struct usb_endpoint_descriptor fs_intr_in_desc = 
{
	.bLength =		USB_DT_ENDPOINT_SIZE,
	.bDescriptorType =	USB_DT_ENDPOINT,

	.bEndpointAddress =	EP_INTR_IN_NUM | USB_DIR_IN,
	.bmAttributes =		USB_ENDPOINT_XFER_INT,
	.wMaxPacketSize =	__constant_cpu_to_le16(FS_INTR_IN_MAXPACKET),
	.bInterval =		1,	// frames -> 1 ms
};

static const struct usb_endpoint_descriptor fs_intr_out_desc = 
{
	.bLength =		USB_DT_ENDPOINT_SIZE,
	.bDescriptorType =	USB_DT_ENDPOINT,

	.bEndpointAddress =	EP_INTR_OUT_NUM ,
	.bmAttributes =		USB_ENDPOINT_XFER_INT,
	.wMaxPacketSize =	__constant_cpu_to_le16(FS_INTR_OUT_MAXPACKET),
	.bInterval =		1,	// frames -> 1 ms
};

#define	HIGHSPEED
#ifdef	HIGHSPEED

/*
 * USB 2.0 devices need to expose both high speed and full speed
 * descriptors, unless they only run at full speed.
 *
 * That means alternate endpoint descriptors (bigger packets)
 * and a "device qualifier" ... plus more construction options
 * for the config descriptor.
 */
static const struct usb_endpoint_descriptor hs_bulk_in_desc = 
{
	.bLength =		USB_DT_ENDPOINT_SIZE,
	.bDescriptorType =	USB_DT_ENDPOINT,

	.bEndpointAddress =	EP_BULK_IN_NUM | USB_DIR_IN,
	.bmAttributes =		USB_ENDPOINT_XFER_BULK,
	.wMaxPacketSize =	__constant_cpu_to_le16(HS_BULK_IN_MAXPACKET),
};

static const struct usb_endpoint_descriptor hs_bulk_out_desc = 
{
	.bLength =		USB_DT_ENDPOINT_SIZE,
	.bDescriptorType =	USB_DT_ENDPOINT,

	.bEndpointAddress =	EP_BULK_OUT_NUM,
	.bmAttributes =		USB_ENDPOINT_XFER_BULK,
	.wMaxPacketSize =	__constant_cpu_to_le16(HS_BULK_OUT_MAXPACKET),
//	.bInterval =		1,	// NAK every 1 uframe		//YPING ?? bug??
};

static const struct usb_endpoint_descriptor hs_intr_in_desc =
{
	.bLength =		USB_DT_ENDPOINT_SIZE,
	.bDescriptorType =	USB_DT_ENDPOINT,

	.bEndpointAddress =	EP_INTR_IN_NUM | USB_DIR_IN,
	.bmAttributes =		USB_ENDPOINT_XFER_INT,
	.wMaxPacketSize =	__constant_cpu_to_le16(HS_INTR_IN_MAXPACKET),
	.bInterval =		1,	// 2**(1-1) = 1 uframes -> 125 us
};

static const struct usb_endpoint_descriptor hs_intr_out_desc =
{
	.bLength =		USB_DT_ENDPOINT_SIZE,
	.bDescriptorType =	USB_DT_ENDPOINT,

	.bEndpointAddress =	EP_INTR_OUT_NUM ,
	.bmAttributes =		USB_ENDPOINT_XFER_INT,
	.wMaxPacketSize =	__constant_cpu_to_le16(HS_INTR_OUT_MAXPACKET),
	.bInterval =		1,	// 2**(1-1) = 1 uframes -> 125 us
};

static struct usb_qualifier_descriptor dev_qualifier = 
{
	.bLength =		sizeof dev_qualifier,
	.bDescriptorType =	USB_DT_DEVICE_QUALIFIER,

	.bcdUSB =		__constant_cpu_to_le16(0x0200),
	.bDeviceClass =		USB_CLASS_PER_INTERFACE,

	.bNumConfigurations =	1,
};

/* Maxpacket and other transfer characteristics vary by speed. */
#define ep_desc(g,fs,hs)	(((g)->speed==USB_SPEED_HIGH) ? (hs) : (fs))

#else

/* If there's no high speed support, maxpacket doesn't change. */
#define ep_desc(g,fs,hs)	fs

#endif	/* !HIGHSPEED */

#define USB_ZERO_BUFSIZ	4096
#define DELAYED_STATUS	(EP0_BUFSIZE + 999)	// An impossibly large value

#endif /* __ZERO_USB_H  */
