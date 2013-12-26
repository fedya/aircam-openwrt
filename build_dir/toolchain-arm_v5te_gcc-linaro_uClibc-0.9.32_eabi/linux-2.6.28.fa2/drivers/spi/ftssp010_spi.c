/** 
 * @file ftssp010_spi.c
 * @brief this file realizes grain media's spi controller, Copyright (C) 2010 GM Corp. 
 * @date 2010-08-25
 *
 * $Revision: 1.4 $
 * $Date: 2011/08/01 09:35:00 $
 * 
 * ChangeLog:
 *  $Log: ftssp010_spi.c,v $
 *  Revision 1.4  2011/08/01 09:35:00  harry_hs
 *  bug fix
 *
 *  Revision 1.3  2011/07/22 08:17:03  harry_hs
 *  add ssp-1
 *
 *  Revision 1.2  2010/09/17 03:46:40  mars_ch
 *  *: use spin_lock_irqsave instead of local_irq_disable
 *
 *  Revision 1.1  2010/08/26 05:01:30  mars_ch
 *  +: add GM spi controller support
 *
 */

#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/spinlock.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>
#include <linux/delay.h>
#include <linux/dma-mapping.h>
#include <linux/spi/spi.h>
#include <linux/spi/flash.h>
#include <linux/gpio.h>
#include <linux/io.h>
#include <mach/vmalloc.h>
#include <asm/div64.h>

#include <mach/platform/gpio.h>
#include <mach/ftssp010_spi.h>
#include <mach/hardware.h>

/******************************************************************************
 * internal functions for FTSSP010
 *****************************************************************************/
static inline void ftssp010_enable(void __iomem *base)
{
    u32 cr2 = inl(base + FTSSP010_OFFSET_CR2);

    cr2 |= FTSSP010_CR2_SSPEN;
    outl(cr2, base + FTSSP010_OFFSET_CR2);
}

static inline void ftssp010_disable(void __iomem *base)
{
    u32 cr2 = inl(base + FTSSP010_OFFSET_CR2);

    cr2 &= ~FTSSP010_CR2_SSPEN;
    outl(cr2, base + FTSSP010_OFFSET_CR2);
}

static inline void ftssp010_clear_fifo(void __iomem *base)
{
    u32 cr2 = inl(base + FTSSP010_OFFSET_CR2);

    cr2 |= FTSSP010_CR2_TXFCLR | FTSSP010_CR2_RXFCLR;
    outl(cr2, base + FTSSP010_OFFSET_CR2);
}

static void ftssp010_set_bits_per_word(void __iomem *base, int bpw)
{
    u32 cr1 = inl(base + FTSSP010_OFFSET_CR1);

    cr1 &= ~FTSSP010_CR1_SDL_MASK;

    if (unlikely(((bpw - 1) < 0) || ((bpw - 1) > 32))) {
        FTSSP010_SPI_PRINT("%s fails: bpw - 1 = %d\n", __func__, bpw - 1);
        return;
    } else {
        cr1 |= FTSSP010_CR1_SDL(bpw - 1);
    }

    outl(cr1, base + FTSSP010_OFFSET_CR1);
}

static void ftssp010_write_word(void __iomem *base, const void *data, int wsize)
{
    u32 tmp = 0;

    if (data) {
        switch (wsize) {
        case 1:
            tmp = *(const u8 *)data;
            break;

        case 2:
            tmp = *(const u16 *)data;
            break;

        default:
            tmp = *(const u32 *)data;
            break;
        }
    }

    outl(tmp, base + FTSSP010_OFFSET_DATA);
}

static void ftssp010_read_word(void __iomem *base, void *buf, int wsize)
{
    u32 data = inl(base + FTSSP010_OFFSET_DATA);

    if (buf) {
        switch (wsize) {
        case 1:
            *(u8 *) buf = data;
            break;

        case 2:
            *(u16 *) buf = data;
            break;

        default:
            *(u32 *) buf = data;
            break;
        }
    }
}

static inline unsigned int ftssp010_read_status(void __iomem *base)
{
    return inl(base + FTSSP010_OFFSET_STATUS);
}

static inline int ftssp010_txfifo_not_full(void __iomem *base)
{
    return ftssp010_read_status(base) & FTSSP010_STATUS_TFNF;
}

static inline int ftssp010_rxfifo_valid_entries(void __iomem *base)
{
    return FTSSP010_STATUS_GET_RFVE(ftssp010_read_status(base));
}

static inline unsigned int ftssp010_read_feature(void __iomem *base)
{
    return inl(base + FTSSP010_OFFSET_FEATURE);
}

static inline int ftssp010_rxfifo_depth(void __iomem *base)
{
    return FTSSP010_FEATURE_RXFIFO_DEPTH(ftssp010_read_feature(base)) + 1;
}

/**
 * Note: due to PMU and IP clock limitation, speed_hz may be not just the same as real bclk, it is likely higher
 */
static inline int ftssp010_set_speed(struct ftssp010_spi *ftssp010_spi, u32 speed_hz)
{
    u16 scldiv = 0;
    u32 cr1 = 0;   

    cr1 = inl(ftssp010_spi->base + FTSSP010_OFFSET_CR1);
    cr1 &= ~0xFFFF;
    scldiv = (ftssp010_spi->hw_platform->working_clk)/(2*speed_hz) - 1;
    cr1 |= scldiv;
    outl(cr1, ftssp010_spi->base + FTSSP010_OFFSET_CR1);

    return 0;
}

static inline void ftssp010_cs_high(struct ftssp010_spi *ftssp010_spi, u8 cs)
{
    if (unlikely(ftssp010_spi->hw_platform->cs_high == NULL)) {
        FTSSP010_SPI_PRINT("%s fails: hw_platform->cs_high NULL\n", __func__);
        return;
    }

    ftssp010_spi->hw_platform->cs_high(cs);
}

static inline void ftssp010_cs_low(struct ftssp010_spi *ftssp010_spi, u8 cs)
{
    if (unlikely(ftssp010_spi->hw_platform->cs_low == NULL)) {
        FTSSP010_SPI_PRINT("%s fails: hw_platform->cs_low NULL\n", __func__);
        return;
    }

    ftssp010_spi->hw_platform->cs_low(cs);
}

/** 
 * @brief real spi transfer work is done here
 * 
 * @param ftssp010_spi alias of controller
 * @param spi specific spi device
 * @param t one spi transfer
 * @param wsize bytes per word, one byte is 8 bits
 * 
 * @return return the number of transfered words 
 */
static int _ftssp010_spi_work_transfer(struct ftssp010_spi *ftssp010_spi,
                                       struct spi_device *spi, struct spi_transfer *t, int wsize)
{
    unsigned long flags;
    unsigned len = t->len;
    const void *tx_buf = t->tx_buf;
    void *rx_buf = t->rx_buf;
    unsigned int dummy_buf;
    
    while (len > 0) {
        int count = 0;
        int i = 0;

        spin_lock_irqsave(&ftssp010_spi->lock, flags);

        /* tx FIFO not full */
        while (ftssp010_txfifo_not_full(ftssp010_spi->base)) {
            if (len <= 0) {
                break;
            }

            ftssp010_write_word(ftssp010_spi->base, tx_buf, wsize);

            if (tx_buf) {
                tx_buf += wsize;
            }

            count++;
            len -= wsize;

            /* avoid Rx FIFO overrun */
            if (count >= ftssp010_spi->rxfifo_depth) {
                break;
            }
        }

        FTSSP010_SPI_PRINT("In %s: tx done\n", __func__);
        
        if (rx_buf == NULL)
            rx_buf = (void *)&dummy_buf;
            
        /* receive the same number as transfered */
        for (i = 0; i < count; i++) {
            /* wait until sth. in rx fifo */
            int j = 0;
            while (!ftssp010_rxfifo_valid_entries(ftssp010_spi->base)
                   && (++j < 10000)) {
                FTSSP010_SPI_PRINT("In %s: wait ftssp010_rxfifo_valid_entries\n", __func__);
            }

            ftssp010_read_word(ftssp010_spi->base, rx_buf, wsize);

            if (rx_buf && (rx_buf != (void *)&dummy_buf)) {
                rx_buf += wsize;
            }
        }

        FTSSP010_SPI_PRINT("In %s: rx done\n", __func__);

        spin_unlock_irqrestore(&ftssp010_spi->lock, flags);
    }
    
    return t->len - len;
}
                                       
/** 
 * @brief based on the specific spi device's parameters to set controller properly work
 * 
 * @param ftssp010_spi alias of controller 
 * @param spi specific spi device
 * @param t one spi transfer
 * 
 * @return 0 to indicate success else a negative to show the error type
 */
static int ftssp010_spi_work_transfer(struct ftssp010_spi *ftssp010_spi,
                                      struct spi_device *spi, struct spi_transfer *t)
{
    unsigned int bpw = 0;
    unsigned int wsize = 0;     /* word size */
    int ret = 0;

    bpw = t->bits_per_word ? t->bits_per_word : spi->bits_per_word;

    if (bpw == 0 || bpw > 32) {
        FTSSP010_SPI_PRINT("%s fails: wrong bpw(%d) by CS(%d)\n", __func__, bpw,
                           spi->chip_select);
        return -EINVAL;
    }

    if (bpw <= 8) {
        wsize = 1;
    } else if (bpw <= 16) {
        wsize = 2;
    } else {
        wsize = 4;
    }

    ftssp010_set_bits_per_word(ftssp010_spi->base, bpw);
    
    if ((t->speed_hz == 0) || (t->speed_hz > spi->max_speed_hz)) {
        //use default device's clk
        t->speed_hz = spi->max_speed_hz;
    }

    ftssp010_set_speed(ftssp010_spi, t->speed_hz);
    
    ftssp010_cs_low(ftssp010_spi, spi->chip_select);

    ret = _ftssp010_spi_work_transfer(ftssp010_spi, spi, t, wsize);

    if (t->cs_change) {
        ftssp010_cs_high(ftssp010_spi, spi->chip_select);
    }

    if (t->delay_usecs) {
        udelay(t->delay_usecs);
    }

    return ret;
}

#if DEBUG_FTSSP010_SPI
static void dump_ssp0_reg(void __iomem * base)
{
    printk("0x00 = %x\n", readl(base));
    printk("0x04 = %x\n", readl((base + 0x04)));
    printk("0x08 = %x\n", readl((base + 0x08)));
    printk("0x0C = %x\n", readl((base + 0x0C)));
    printk("0x10 = %x\n", readl((base + 0x10)));
    printk("0x14 = %x\n", readl((base + 0x14)));
    printk("0x18 = %x\n", readl((base + 0x18)));
    printk("0x1C = %x\n", readl((base + 0x1C)));
}
#endif

/** 
 * @brief work until all transfers in one spi message done
 * 
 * @param ftssp010_spi alias of controller
 * @param m one spi message
 */
static void ftssp010_spi_work_message(struct ftssp010_spi *ftssp010_spi, struct spi_message *m)
{
    struct spi_device *spi = m->spi;
    struct spi_transfer *t;
    
    m->status = 0;
    m->actual_length = 0;

    ftssp010_enable(ftssp010_spi->base);
    ftssp010_cs_low(ftssp010_spi, spi->chip_select);
            
#if DEBUG_FTSSP010_SPI
    dump_ssp0_reg(ftssp010_spi->base);
#endif
    list_for_each_entry(t, &m->transfers, transfer_list) {
        int ret;

        if ((ret = ftssp010_spi_work_transfer(ftssp010_spi, spi, t)) < 0) {
            m->status = ret;
            break;
        }
        m->actual_length += ret;
    }

    ftssp010_cs_high(ftssp010_spi, spi->chip_select);
    ftssp010_disable(ftssp010_spi->base);
    
    m->complete(m->context);
}


/** 
 * @brief work until all spi messages requested by a specific device done
 * 
 * @param work one requested work to be done
 */
static void ftssp010_spi_work(struct work_struct *work)
{
    struct ftssp010_spi *ftssp010_spi = NULL;
    unsigned long flags = 0;

    ftssp010_spi = container_of(work, struct ftssp010_spi, work);
    
    spin_lock_irqsave(&ftssp010_spi->lock, flags);

    while (!list_empty(&ftssp010_spi->message_queue)) {
        struct spi_message *m = NULL;

        m = container_of(ftssp010_spi->message_queue.next, struct spi_message, queue);
        list_del_init(&m->queue);

        spin_unlock_irqrestore(&ftssp010_spi->lock, flags);
        ftssp010_spi_work_message(ftssp010_spi, m);

        spin_lock_irqsave(&ftssp010_spi->lock, flags);
    }

    spin_unlock_irqrestore(&ftssp010_spi->lock, flags);
}

/** 
 * @brief ISR for SPI controller get something wrong
 * 
 * @param irq spi's irq number
 * @param dev_id specific spi info
 * 
 * @return always return IRQ_HANDLED to indicate we got the irq
 */
static irqreturn_t ftssp010_spi_interrupt(int irq, void *dev_id)
{
    struct spi_master *master = dev_id;
    struct ftssp010_spi *ftssp010_spi = NULL;
    u32 isr = 0;

    ftssp010_spi = spi_master_get_devdata(master);

    isr = inl(ftssp010_spi->base + FTSSP010_OFFSET_ISR);

    if (isr & FTSSP010_ISR_RFOR) {
        outl(FTSSP010_ISR_RFOR, ftssp010_spi->base + FTSSP010_OFFSET_ISR);
        FTSSP010_SPI_PRINT("In %s: RX Overrun\n", __func__);
    }

    wake_up(&ftssp010_spi->waitq);

    return IRQ_HANDLED;
}

/* the spi->mode bits understood by this driver: */
#define MODEBITS (SPI_CPOL | SPI_CPHA | SPI_LSB_FIRST | SPI_LOOP)

/** 
 * @brief set proper controller's state based on specific device
 * 
 * @param ftssp010_spi alias of controller
 * @param mode device's specific mode
 * 
 * @return always return 0 to indicate set up done
 */
static int ftssp010_spi_master_setup_mode(struct ftssp010_spi
                                          *ftssp010_spi, u8 mode)
{
    u32 cr0 = 0;

    if (mode & ~MODEBITS) {
        FTSSP010_SPI_PRINT("%s fails: some SPI mode not support currently\n", __func__);
        return -EINVAL;
    }

    cr0 |= FTSSP010_CR0_FSPO | FTSSP010_CR0_FFMT_SPI | FTSSP010_CR0_OPM_MASTER_STEREO;

    if (mode & SPI_CPOL) {
        cr0 |= FTSSP010_CR0_SCLKPO;
    }

    if (mode & SPI_CPHA) {
        cr0 |= FTSSP010_CR0_SCLKPH;
    }

    if (mode & SPI_LOOP) {
        cr0 |= FTSSP010_CR0_LBM;
    }

    if (mode & SPI_LSB_FIRST) {
        cr0 |= FTSSP010_CR0_LSB;
    }

    outl(cr0, ftssp010_spi->base + FTSSP010_OFFSET_CR0);

    return 0;
}

/** 
 * @brief init specific spi divice's setting, this should be given by device. Or, we will give normal value.
 *        
 * @param spi specific device
 * 
 * @return return 0 when everything goes fine else return a negative to indicate error
 */
static int ftssp010_spi_master_setup(struct spi_device *spi)
{
    struct ftssp010_spi *ftssp010_spi = NULL;
    unsigned int bpw = spi->bits_per_word;
    int ret = 0;
    
    ftssp010_spi = spi_master_get_devdata(spi->master);

    if (spi->chip_select > spi->master->num_chipselect) {
        FTSSP010_SPI_PRINT
            ("%s fails: invalid chipselect %u (%u defined)\n",
             __func__, spi->chip_select, spi->master->num_chipselect);
        return -EINVAL;
    }

    /* check bits per word */
    if (bpw == 0) {
        FTSSP010_SPI_PRINT("In %s: bpw == 0, use 8 bits by default\n", __func__);
        bpw = 8;
    }

    if (bpw == 0 || bpw > 32) {
        FTSSP010_SPI_PRINT("%s fails: invalid bpw%u (1 to 32)\n", __func__, bpw);
        return -EINVAL;
    }

    spi->bits_per_word = bpw;

    /* check mode */

    if ((ret = ftssp010_spi_master_setup_mode(ftssp010_spi, spi->mode)) < 0) {
        FTSSP010_SPI_PRINT("%s fails: ftssp010_spi_master_setup_mode not OK\n", __func__);
        return ret;
    }

    /* check speed */
    if (!spi->max_speed_hz) {
        FTSSP010_SPI_PRINT("%s fails: max speed not specified\n", __func__);
        return -EINVAL;
    }
   
    if (spi->max_speed_hz > ftssp010_spi->hw_platform->working_clk) {
        panic("In %s: spi->max_speed_hz(%dHz) is out of current IP's capability(%d).\n", 
               __func__, spi->max_speed_hz, ftssp010_spi->hw_platform->working_clk);
    }

    ftssp010_set_bits_per_word(ftssp010_spi->base, bpw);

    ftssp010_cs_high(ftssp010_spi, spi->chip_select);
    ftssp010_clear_fifo(ftssp010_spi->base);

    return 0;
}

/** 
 * @brief send a complete spi message
 * 
 * @param spi specific spi device
 * @param m the message to be sent
 * 
 * @return return 0 when everything goes fine else return a negative to indicate error
 */
static int ftssp010_spi_master_send_message(struct spi_device *spi, struct spi_message *m)
{
    struct ftssp010_spi *ftssp010_spi;
    struct spi_transfer *t;
    unsigned long flags;

    ftssp010_spi = spi_master_get_devdata(spi->master);

    /* sanity check */
    list_for_each_entry(t, &m->transfers, transfer_list) {
        if (!(t->tx_buf || t->rx_buf) && t->len) {
            FTSSP010_SPI_PRINT("%s fails: missing rx or tx buf\n", __func__);
            return -EINVAL;
        }
    }

    m->status = -EINPROGRESS;
    m->actual_length = 0;

    /* transfer */

    spin_lock_irqsave(&ftssp010_spi->lock, flags);

    list_add_tail(&m->queue, &ftssp010_spi->message_queue);
    queue_work(ftssp010_spi->workqueue, &ftssp010_spi->work);

    spin_unlock_irqrestore(&ftssp010_spi->lock, flags);
    
    return 0;
}

static int ftssp010_spi_master_transfer(struct spi_device *spi, struct spi_message *m)
{
    if (unlikely(list_empty(&m->transfers))) {
        return -EINVAL;
    }

    return ftssp010_spi_master_send_message(spi, m);
}

static void ftssp010_spi_master_cleanup(struct spi_device *spi)
{
    if (!spi->controller_state) {
        return;
    }
}

/** 
 * @brief probe spi controller
 * 
 * @param pdev the spi controller is a platform device specified in arch/arm/mach-GM/platform-XXX/platform.c
 * 
 * @return return 0 when everything goes fine else return a negative to indicate error
 */
static int ftssp010_spi_probe(struct platform_device *pdev)
{
    struct resource *res = NULL;
    int irq = 0;
    void __iomem *base = NULL;
    int ret = 0;
    struct spi_master *master = NULL;
    struct ftssp010_spi *ftssp010_spi = NULL;
    struct ftssp010_spi_hw_platform *spi_hw_platform = NULL;    
        
    spi_hw_platform = ftssp010_spi_get_hw_platform(pdev->id);
    if (unlikely(spi_hw_platform == NULL)) {
        panic("%s: SSP(%d) spi_hw_platform is NULL.\n", __func__, pdev->id);
    }
    
    if ((res = platform_get_resource(pdev, IORESOURCE_MEM, 0)) == 0) {
        return -ENXIO;
    }

    if ((irq = platform_get_irq(pdev, 0)) < 0) {
        return irq;
    }

    /* setup spi core */
    if ((master = spi_alloc_master(&pdev->dev, sizeof(struct ftssp010_spi))) == NULL) {
        ret = -ENOMEM;
        goto err_dealloc;
    }

    master->bus_num = pdev->id;
    master->num_chipselect = spi_hw_platform->nr_chip_select;
    master->setup = ftssp010_spi_master_setup;
    master->transfer = ftssp010_spi_master_transfer;
    master->cleanup = ftssp010_spi_master_cleanup;
    platform_set_drvdata(pdev, master);

    /* setup master private data */
    ftssp010_spi = spi_master_get_devdata(master);

    spin_lock_init(&ftssp010_spi->lock);
    INIT_LIST_HEAD(&ftssp010_spi->message_queue);
    INIT_WORK(&ftssp010_spi->work, ftssp010_spi_work);
    init_waitqueue_head(&ftssp010_spi->waitq);

    if ((base = ioremap_nocache(res->start, (res->end - res->start + 1))) == NULL) {
        ret = -ENOMEM;
        goto err_dealloc;
    }
    
    /* Error Handler */
    if ((ret = request_irq(irq, ftssp010_spi_interrupt, 0, pdev->dev.bus_id, master))) {
        goto err_unmap;
    }

    ftssp010_spi->controller_id = pdev->id;
    ftssp010_spi->hw_platform = spi_hw_platform; 
    ftssp010_spi->irq = irq;
    ftssp010_spi->pbase = res->start;
    ftssp010_spi->base = base;
    ftssp010_spi->master = master;
    ftssp010_spi->rxfifo_depth = ftssp010_rxfifo_depth(base);

    if ((ftssp010_spi->workqueue = create_singlethread_workqueue(pdev->dev.bus_id)) == NULL) {
        goto err_free_irq;
    }

    /* Initialize the hardware */
    outl(FTSSP010_ICR_RFOR | FTSSP010_ICR_RFTH | FTSSP010_ICR_RFTHOD(1),
         base + FTSSP010_OFFSET_ICR);
    
    if ((ret = spi_register_master(master)) != 0) {
        dev_err(&pdev->dev, "register master failed\n");
        goto err_destroy_workqueue;
    }
    
    /* go! */
    printk("Probe FTSSP010 SPI Controller at 0x%08x (irq %d)\n", (unsigned int)res->start, irq);

    return 0;

  err_destroy_workqueue:
    destroy_workqueue(ftssp010_spi->workqueue);
  err_free_irq:
    free_irq(irq, master);
  err_unmap:
    iounmap(base);
  err_dealloc:
    spi_master_put(master);

    return ret;
}

/** 
 * @brief remove the spi controller from kernel
 * 
 * @param pdev the spi controller 
 * 
 * @return always return 0 to indicate it is not workable any more
 */
static int ftssp010_spi_remove(struct platform_device *pdev)
{
    struct spi_master *master = NULL;
    struct ftssp010_spi *ftssp010_spi = NULL;
    struct spi_message *m = NULL;

    printk("Remove FTSSP010 Controller\n");

    master = platform_get_drvdata(pdev);
    ftssp010_spi = spi_master_get_devdata(master);

    /* Terminate remaining queued transfers */
    list_for_each_entry(m, &ftssp010_spi->message_queue, queue) {
        m->status = -ESHUTDOWN;
        m->complete(m->context);
    }

    destroy_workqueue(ftssp010_spi->workqueue);
    free_irq(ftssp010_spi->irq, master);
    iounmap(ftssp010_spi->base);
    spi_unregister_master(master);

    return 0;
}

/** 
 * @brief SPI controller driver is a platform driver to probe platform device SPI controller 
 */
static struct platform_driver ftssp010_spi_driver = {
    .probe = ftssp010_spi_probe,
    .remove = ftssp010_spi_remove,
    .suspend = NULL,
    .resume = NULL,
    .driver = {
               .name = "ftssp010_spi",
               .owner = THIS_MODULE,
               },
};

/** 
 * @brief module init
 * 
 * @return return 0 when everything goes fine else return a negative to indicate error
 */
static int __init ftssp010_spi_init(void)
{
    int ret = 0;
        
    ret = platform_driver_register(&ftssp010_spi_driver);
    if (unlikely(ret != 0)) {
        printk("%s: register platform driver failed(%d)\n", __func__, ret);
        goto fail;
    }

    return ret;

  fail:
    return ret;
}

/** 
 * @brief module exit routine
 * 
 * @return no need to return value
 */
static void __exit ftssp010_spi_exit(void)
{
    platform_driver_unregister(&ftssp010_spi_driver);
    printk("deinit SPI Done\n");
}

module_init(ftssp010_spi_init);
module_exit(ftssp010_spi_exit);

MODULE_AUTHOR("Grain Media Corp");
MODULE_DESCRIPTION("FTSSP010 SPI Driver");
MODULE_LICENSE("GPL");
