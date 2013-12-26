#ifndef __CPU_COMM_H__
#define __CPU_COMM_H__

#define CPU_COMM_VERSION    "0.1.0"
#define PFX		            "CPU_COMM"

#define M_DATA_SZ   52      //module data size

#ifdef __KERNEL__

#define MKCHAN(chip, subsys, chanid)  (((chip & 0xFF) << 24) | ((subsys & 0xFF) << 16) | ((chanid & 0xFFFF)))
#define CHIP_ID(x)      (((x) >> 24) & 0xFF)
#define SUBSYS_ID(x)    (((x) >> 16) & 0xFF)
#define CHAN_ID(x)      ((x) & 0xFFFF)

/* channel_t consists of three parts:
 * 1. chip id: bits 24~31
 * 2. subsystem id: bits 16 ~ 23
 * 3. channel id: bits 0 ~ 15
 */
typedef unsigned int channel_t;

typedef enum {
    CHIP_ID_LOCAL = 0xff,
    CHIP_ID_FIRST = 0,    
    CHIP_ID_HOST = CHIP_ID_FIRST,
    CHIP_ID_DEV0 = CHIP_ID_HOST,    //device0
    CHIP_ID_DEV1,                   //device1
    CHIP_ID_MAX
} chip_id_t;

typedef enum {
    SUBSYS_ID_FIRST = 0, 
    SUBSYS_ID_FA726 = SUBSYS_ID_FIRST,
    SUBSYS_ID_FA626 = SUBSYS_ID_FA726,
    SUBSYS_ID_FC7500,
    SUBSYS_ID_MAX
} subsys_id_t;

typedef enum {
    CH_ID_FIRST = 0,
    CH_ID_FRAMMAP = CH_ID_FIRST,
    CH_ID_DVR,
    CH_ID_DRIVER_LAST,
    CH_ID_AP0 = CH_ID_DRIVER_LAST,
    CH_ID_AP1,
    CH_ID_AP2,
    CH_ID_AP3,
    CH_ID_AP4,
    CH_ID_AP5,
    CH_ID_AP6,
    CH_ID_AP7,
    CH_ID_AP8,
    CH_ID_AP9,
    CH_ID_AP10,
    CH_ID_AP11,
    CH_ID_AP12,
    CH_ID_AP13,
    CH_ID_AP14,
    CH_ID_AP15,
    CH_ID_AP16,
    CH_ID_MINORS_PER_BOARD
} ch_id_t;

/* the message size is 64 bytes which is ring descriptor.
 */
typedef struct {
    channel_t     t_channel;    //who is the target channel
    unsigned char m_data[M_DATA_SZ];    //module data
    unsigned char rx_ready;     //internal use
    unsigned char tx_empty;     //internal use
    unsigned char reserved[2];  //internal use
    struct list_head    list;   //internal use
} cpu_comm_msg_t;

/*
 * @This function registers a client to CPU_COMM module. When the frames come, it will be delivered
 *      to the channel of this reigstered client.
 *
 * @function int cpu_comm_register(ch_id_t ch_id, char *name);
 * @param ch_id indicates the module id prdefined in ch_id_t
 * @param name indicates your name.
 * @return 0 on success, <0 on error
 */
int cpu_comm_register(ch_id_t ch_id, char *name);

/*
 * @This function de-registers a client from CPU_COMM module.
 *
 * @function int cpu_comm_unregister(ch_id_t ch_id, char *name);
 * @param ch_id indicates the module id prdefined in ch_id_t 
 * @return 0 on success, <0 on error
 */
int cpu_comm_unregister(ch_id_t ch_id);

/*
 * @This function sends the msg into the channel. 
 *
 * @function int cpu_comm_msg_snd(cpu_comm_msg_t *msg, int b_wait);
 * @param msg indicates the data the module wants to send. Note: msg->channel is
 *      necessary. It indicates the destination.
 * @param b_wait indicates whether the caller will be blocked if the ring buffer is full or the 
 *      buffer of the peer is full.
 * @return 0 on success, <0 on error
 */
int cpu_comm_msg_snd(cpu_comm_msg_t *msg, int b_wait);

/*
 * @This function receives the msg from the channel. 
 *
 * @function int cpu_comm_msg_rcv(cpu_comm_msg_t *msg, int b_wait);
 * @param msg indicates the data the module wants to receive. Note: msg->channel is
 *      necessary. It indicates the destination.
 * @param b_wait indicates whether the caller will be blocked if the channel is empty.
 * @return 0 on success, <0 on error
 */
int cpu_comm_msg_rcv(cpu_comm_msg_t *msg, int b_wait);

/*
 * @This function returns the number of msgs queued in this channel.
 *
 * @function int cpu_comm_msg_qlen(channel_t channel);
 * @param channel indicates the channel
 * @return number of messages queued in this channel.
 */
int cpu_comm_msg_qlen(channel_t channel);

/*
 * @This function returns the number of devices on the system
 *
 * @function int cpu_comm_num_devices(void)
 * @param: none
 * @return number of devices on the system.
 *  Note: if the device, this function always returns 1.
 */
int cpu_comm_num_devices(void);

/*
 * @This function translate the physical address from remote into a global physical address.
 *  
 * @function unsigned int cpu_comm_get_phyaddr(unsigned int phy_addr, int chip_id, int subsys_id)
 * @param: phy_addr from remote
 * @param: chip_id indicates the phy_addr from which chip
 * @param: subsys_id indicates the phy_addr from which sub-system
 * @return global physical address
 */
unsigned int cpu_comm_get_phyaddr(unsigned int phy_addr, int chip_id, int subsys_id);

/*
 * @This function returns who I am.
 *
 * @function int cpu_comm_get_identifier(int *chip_id, int *subsys_id)
 * @param: chip_id indicates the numbered chip id in the whole system. -1 means host. 0 means the 
 *          first device in the PCI bus.
 * @param: subsys_id indicates the numbered sub system id within one chip. -1 means local host, 0
 *          means the first sub-system which is FA626.
 * @return 0 for success others for fail.
 */
int cpu_comm_get_identifier(int *chip_id, int *subsys_id);

#endif /* __KERNEL__ */

// ioctl 
#define CPU_COMM_MAGIC 'h'

#define CPU_COMM_READ_MAX   _IOR(CPU_COMM_MAGIC, 1, unsigned int)
#define CPU_COMM_WRITE_MAX  _IOR(CPU_COMM_MAGIC, 2, unsigned int)
#define CPU_COMM_QLEN       _IOR(CPU_COMM_MAGIC, 3, unsigned int)   //how many frames are in q.
#define CPU_COMM_MAX_LIST_LEN   _IOR(CPU_COMM_MAGIC, 4, unsigned int)   //the max qlen

#endif /* __CPU_COMM_H__ */




