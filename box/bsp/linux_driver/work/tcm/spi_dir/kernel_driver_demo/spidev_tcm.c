/*
 * Simple synchronous userspace interface to SPI devices
 *
 * Copyright (C) 2006 SWAPP
 *	Andrea Paterniani <a.paterniani@swapp-eng.it>
 * Copyright (C) 2007 David Brownell (simplification, cleanup)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/ioctl.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/dma-mapping.h>
#include <linux/err.h>
#include <linux/list.h>
#include <linux/errno.h>
#include <linux/mutex.h>
#include <linux/slab.h>
#include <linux/compat.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/gpio.h>
#include <linux/of_gpio.h>
#include <linux/spi/spi.h>
#include <linux/spi/spidev.h>
#include <plat/mfp.h>
#include <linux/delay.h>
#include <asm/uaccess.h>


/*
 * This supports access to SPI devices using normal userspace I/O calls.
 * Note that while traditional UNIX/POSIX I/O semantics are half duplex,
 * and often mask message boundaries, full SPI support requires full duplex
 * transfers.  There are several kinds of internal message boundaries to
 * handle chipselect management and other protocol options.
 *
 * SPI has a character major number assigned.  We allocate minor numbers
 * dynamically using a bitmask.  You must use hotplug tools, such as udev
 * (or mdev with busybox) to create and destroy the /dev/spidevB.C device
 * nodes, since there is no fixed association of minor numbers with any
 * particular SPI bus or device.
 */
#define SPIDEV_MAJOR			406	/* assigned */
#define N_SPI_MINORS			39	/* ... up to 256 */

static DECLARE_BITMAP(minors, N_SPI_MINORS);


/* Bit masks for spi_device.mode management.  Note that incorrect
 * settings for some settings can cause *lots* of trouble for other
 * devices on a shared bus:
 *
 *  - CS_HIGH ... this device will be active when it shouldn't be
 *  - 3WIRE ... when active, it won't behave as it should
 *  - NO_CS ... there will be no explicit message boundaries; this
 *	is completely incompatible with the shared bus model
 *  - READY ... transfers may proceed when they shouldn't.
 *
 * REVISIT should changing those flags be privileged?
 */
#define SPI_MODE_MASK		(SPI_CPHA | SPI_CPOL | SPI_CS_HIGH \
				| SPI_LSB_FIRST | SPI_3WIRE | SPI_LOOP \
				| SPI_NO_CS | SPI_READY)

int ssi_ssn_pin, ssi_clk_pin;
struct spidev_data {
	dev_t			devt;
	spinlock_t		spi_lock;
	struct spi_device	*spi;
	struct list_head	device_entry;

	/* buffer is NULL unless this device is open (users > 0) */
	struct mutex		buf_lock;
	unsigned		users;
	u8			*buffer;
	u8			*rx_buffer;
	dma_addr_t spi_tx_dma;
	dma_addr_t spi_rx_dma;
};

static const struct of_device_id spidev_dt_ids[] = {
	{ .compatible = "rohm,dh2228fv" },
	{ .compatible = "dmt,tcm-DMT-FAC-CG4Q-V2" },
	{},
};

#define XDJA_SPI_IOC_RD_GPIO                _IOR(SPI_IOC_MAGIC, 5, __u32)
#define XDJA_SPI_IOC_WD_GPIO_HIGH           _IO(SPI_IOC_MAGIC, 6)
#define XDJA_SPI_IOC_WD_GPIO_LOW            _IO(SPI_IOC_MAGIC, 7)
#define SSI0_BASE 		0xA0880000
#define SSI1_BASE 		0xA0881000
#define SSI2_BASE 		0xA08A2000
unsigned int ssi_base = SSI1_BASE;
#define BASE 			(ssi_base)
#define SPI_BASE	(unsigned long)ioremap(BASE,4)
#define XDJA_CLK_BASE		0xA0899000
#define XDJA_SPI_CNT	1

#define SSI_CTRL0		(unsigned long)ioremap(BASE+0x00,4)
#define SSI_CTRL1		(unsigned long)ioremap(BASE+0x04,4)
#define SSI_EN			(unsigned long)ioremap(BASE+0x08,4)
#define SSI_BUAD		(unsigned long)ioremap(BASE+0x14,4)
#define SSI_TXFTL		(unsigned long)ioremap(BASE+0x18,4)
#define SSI_RXFTL		(unsigned long)ioremap(BASE+0x1C,4)
#define SSI_STS		(unsigned long)ioremap(BASE+0x28,4)
#define SSI_IE			(unsigned long)ioremap(BASE+0x2C,4)
#define SSI_IC			(unsigned long)ioremap(BASE+0x48,4)
#define SSI_DATA		(unsigned long)ioremap(BASE+0x60,4)
#define SSI_CLK		(unsigned long)ioremap(XDJA_CLK_BASE+0x120,4)

#define SSI_CLK_EN		13		//ssi1->13 ssi0->12
#define LC_SSI_EN		(1 << 0)

void __iomem *  spi_data;
void __iomem *  spi_sts;

MODULE_DEVICE_TABLE(of, spidev_dt_ids);

static LIST_HEAD(device_list);
static DEFINE_MUTEX(device_list_lock);

static unsigned bufsiz = 4096;
module_param(bufsiz, uint, S_IRUGO);
MODULE_PARM_DESC(bufsiz, "data bytes in biggest supported SPI message");

#define USING_DMA
/*-------------------------------------------------------------------------*/

/*
 * We can't use the standard synchronous wrappers for file I/O; we
 * need to protect against async removal of the underlying spi_device.
 */
static void spidev_complete(void *arg)
{
	complete(arg);
}

static ssize_t
spidev_sync(struct spidev_data *spidev, struct spi_message *message)
{
	DECLARE_COMPLETION_ONSTACK(done);
	int status;

	message->complete = spidev_complete;
	message->context = &done;

	spin_lock_irq(&spidev->spi_lock);
	if (spidev->spi == NULL)
		status = -ESHUTDOWN;
	else
		status = spi_async(spidev->spi, message);
	spin_unlock_irq(&spidev->spi_lock);

	if (status == 0) {
		wait_for_completion(&done);
		status = message->status;
		if (status == 0)
			status = message->actual_length;
	}
	return status;
}

static inline ssize_t
spidev_sync_write(struct spidev_data *spidev, size_t len)
{
	struct spi_transfer	t = {
			.tx_buf		= spidev->buffer,
			.len		= len,
		};
	struct spi_message	m;

	spi_message_init(&m);
	spi_message_add_tail(&t, &m);
	return spidev_sync(spidev, &m);
}

static inline ssize_t
spidev_sync_read(struct spidev_data *spidev, size_t len)
{
	struct spi_transfer	t = {
			.rx_buf		= spidev->buffer,
			.len		= len,
		};
	struct spi_message	m;

	spi_message_init(&m);
	spi_message_add_tail(&t, &m);
	return spidev_sync(spidev, &m);
}

/*-------------------------------------------------------------------------*/

/* Read-only message with current device setup */
static ssize_t
spidev_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	struct spidev_data	*spidev;
	ssize_t			status = 0;

	/* chipselect only toggles at start or end of operation */
	if (count > bufsiz)
		return -EMSGSIZE;

	spidev = filp->private_data;

	mutex_lock(&spidev->buf_lock);
	status = spidev_sync_read(spidev, count);
	if (status > 0) {
		unsigned long	missing;

		missing = copy_to_user(buf, spidev->buffer, status);
		if (missing == status)
			status = -EFAULT;
		else
			status = status - missing;
	}
	mutex_unlock(&spidev->buf_lock);

	return status;
}

/* Write-only message with current device setup */
static ssize_t
spidev_write(struct file *filp, const char __user *buf,
		size_t count, loff_t *f_pos)
{
	struct spidev_data	*spidev;
	ssize_t			status = 0;
	unsigned long		missing;

	/* chipselect only toggles at start or end of operation */
	if (count > bufsiz)
		return -EMSGSIZE;

	spidev = filp->private_data;

	mutex_lock(&spidev->buf_lock);
	missing = copy_from_user(spidev->buffer, buf, count);
	if (missing == 0) {
		status = spidev_sync_write(spidev, count);
	} else
		status = -EFAULT;
	mutex_unlock(&spidev->buf_lock);

	return status;
}

static int spidev_message(struct spidev_data *spidev,
		struct spi_ioc_transfer *u_xfers, unsigned n_xfers)
{
	struct spi_message	msg;
	struct spi_transfer	*k_xfers;
	struct spi_transfer	*k_tmp;
	struct spi_ioc_transfer *u_tmp;
	unsigned		n, total;
	u8			*buf;
	int			status = -EFAULT;
#ifdef USING_DMA
    int buffer_offset = 0;
    int rx_buffer_offset = 0;
#endif

	spi_message_init(&msg);
#ifdef USING_DMA
    msg.is_dma_mapped = 1;
#endif
	k_xfers = kcalloc(n_xfers, sizeof(*k_tmp), GFP_KERNEL);
	if (k_xfers == NULL)
		return -ENOMEM;

	/* Construct spi_message, copying any tx data to bounce buffer.
	 * We walk the array of user-provided transfers, using each one
	 * to initialize a kernel version of the same transfer.
	 */
	buf = spidev->buffer;
	total = 0;
	for (n = n_xfers, k_tmp = k_xfers, u_tmp = u_xfers;
			n;
			n--, k_tmp++, u_tmp++) {
		k_tmp->len = u_tmp->len;

		total += k_tmp->len;
		/* Check total length of transfers.  Also check each
		 * transfer length to avoid arithmetic overflow.
		 */
		if (total > bufsiz || k_tmp->len > bufsiz) {
			status = -EMSGSIZE;
			goto done;
		}

		if (u_tmp->rx_buf) {
#ifndef USING_DMA
			k_tmp->rx_buf = buf;
#else
			k_tmp->rx_buf = spidev->rx_buffer + rx_buffer_offset;
			k_tmp->rx_dma = spidev->spi_rx_dma + rx_buffer_offset;
#endif
			if (!access_ok(VERIFY_WRITE, (u8 __user *)
						(uintptr_t) u_tmp->rx_buf,
						u_tmp->len))
				goto done;
		}
		if (u_tmp->tx_buf) {
#ifndef USING_DMA
			k_tmp->tx_buf = buf;
			if (copy_from_user(buf, (const u8 __user *)
						(uintptr_t) u_tmp->tx_buf,
					u_tmp->len))
				goto done;
#else
			k_tmp->tx_buf = spidev->buffer + buffer_offset;
			k_tmp->tx_dma = spidev->spi_tx_dma + buffer_offset;
			if (copy_from_user(k_tmp->tx_buf, (const u8 __user *)
						(uintptr_t) u_tmp->tx_buf,
					u_tmp->len))
				goto done;
#endif
		}
#ifndef USING_DMA
		buf += k_tmp->len;
#else
        rx_buffer_offset += k_tmp->len;
        buffer_offset += k_tmp->len;
#endif

		k_tmp->cs_change = !!u_tmp->cs_change;
		k_tmp->bits_per_word = u_tmp->bits_per_word;
		k_tmp->delay_usecs = u_tmp->delay_usecs;
		k_tmp->speed_hz = u_tmp->speed_hz;
#ifdef VERBOSE
		dev_dbg(&spidev->spi->dev,
			"  xfer len %zd %s%s%s%dbits %u usec %uHz\n",
			u_tmp->len,
			u_tmp->rx_buf ? "rx " : "",
			u_tmp->tx_buf ? "tx " : "",
			u_tmp->cs_change ? "cs " : "",
			u_tmp->bits_per_word ? : spidev->spi->bits_per_word,
			u_tmp->delay_usecs,
			u_tmp->speed_hz ? : spidev->spi->max_speed_hz);
#endif
		spi_message_add_tail(k_tmp, &msg);
	}

	status = spidev_sync(spidev, &msg);
	if (status < 0)
		goto done;

	/* copy any rx data out of bounce buffer */
#ifndef USING_DMA
	buf = spidev->buffer;
#else
	buf = spidev->rx_buffer;
#endif
	for (n = n_xfers, u_tmp = u_xfers; n; n--, u_tmp++) {
		if (u_tmp->rx_buf) {
			if (__copy_to_user((u8 __user *)
					(uintptr_t) u_tmp->rx_buf, buf,
					u_tmp->len)) {
				status = -EFAULT;
				goto done;
			}
		}
		buf += u_tmp->len;
	}
	status = total;

done:
	kfree(k_xfers);
	return status;
}

static long
spidev_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	int			err = 0;
	int			retval = 0;
	struct spidev_data	*spidev;
	struct spi_device	*spi;
	u32			tmp;
	unsigned		n_ioc;
	struct spi_ioc_transfer	*ioc;

	/* Check type and command number */
	if (_IOC_TYPE(cmd) != SPI_IOC_MAGIC)
		return -ENOTTY;

	/* Check access direction once here; don't repeat below.
	 * IOC_DIR is from the user perspective, while access_ok is
	 * from the kernel perspective; so they look reversed.
	 */
	if (_IOC_DIR(cmd) & _IOC_READ)
		err = !access_ok(VERIFY_WRITE,
				(void __user *)arg, _IOC_SIZE(cmd));
	if (err == 0 && _IOC_DIR(cmd) & _IOC_WRITE)
		err = !access_ok(VERIFY_READ,
				(void __user *)arg, _IOC_SIZE(cmd));
	if (err)
		return -EFAULT;

	/* guard against device removal before, or while,
	 * we issue this ioctl.
	 */
	spidev = filp->private_data;
	spin_lock_irq(&spidev->spi_lock);
	spi = spi_dev_get(spidev->spi);
	spin_unlock_irq(&spidev->spi_lock);

	if (spi == NULL)
		return -ESHUTDOWN;

	/* use the buffer lock here for triple duty:
	 *  - prevent I/O (from us) so calling spi_setup() is safe;
	 *  - prevent concurrent SPI_IOC_WR_* from morphing
	 *    data fields while SPI_IOC_RD_* reads them;
	 *  - SPI_IOC_MESSAGE needs the buffer locked "normally".
	 */
	mutex_lock(&spidev->buf_lock);

	switch (cmd) {
	/* read requests */
	case SPI_IOC_RD_MODE:
		retval = __put_user(spi->mode & SPI_MODE_MASK,
					(__u8 __user *)arg);
		break;
	case SPI_IOC_RD_LSB_FIRST:
		retval = __put_user((spi->mode & SPI_LSB_FIRST) ?  1 : 0,
					(__u8 __user *)arg);
		break;
	case SPI_IOC_RD_BITS_PER_WORD:
		retval = __put_user(spi->bits_per_word, (__u8 __user *)arg);
		break;
	case SPI_IOC_RD_MAX_SPEED_HZ:
		retval = __put_user(spi->max_speed_hz, (__u32 __user *)arg);
		break;

	/* write requests */
	case SPI_IOC_WR_MODE:
		retval = __get_user(tmp, (u8 __user *)arg);
		if (retval == 0) {
			u8	save = spi->mode;

			if (tmp & ~SPI_MODE_MASK) {
				retval = -EINVAL;
				break;
			}

			tmp |= spi->mode & ~SPI_MODE_MASK;
			spi->mode = (u8)tmp;
			retval = spi_setup(spi);
			if (retval < 0)
				spi->mode = save;
			else
				dev_dbg(&spi->dev, "spi mode %02x\n", tmp);
		}
		break;
	case SPI_IOC_WR_LSB_FIRST:
		retval = __get_user(tmp, (__u8 __user *)arg);
		if (retval == 0) {
			u8	save = spi->mode;

			if (tmp)
				spi->mode |= SPI_LSB_FIRST;
			else
				spi->mode &= ~SPI_LSB_FIRST;
			retval = spi_setup(spi);
			if (retval < 0)
				spi->mode = save;
			else
				dev_dbg(&spi->dev, "%csb first\n",
						tmp ? 'l' : 'm');
		}
		break;
	case SPI_IOC_WR_BITS_PER_WORD:
		retval = __get_user(tmp, (__u8 __user *)arg);
		if (retval == 0) {
			u8	save = spi->bits_per_word;

			spi->bits_per_word = tmp;
			retval = spi_setup(spi);
			if (retval < 0)
				spi->bits_per_word = save;
			else
				dev_dbg(&spi->dev, "%d bits per word\n", tmp);
		}
		break;
	case SPI_IOC_WR_MAX_SPEED_HZ:
		retval = __get_user(tmp, (__u32 __user *)arg);
		if (retval == 0) {
			u32	save = spi->max_speed_hz;

			spi->max_speed_hz = tmp;
			retval = spi_setup(spi);
			if (retval < 0)
				spi->max_speed_hz = save;
			else
				dev_dbg(&spi->dev, "%d Hz (max)\n", tmp);
		}
		break;

	default:
		/* segmented and/or full-duplex I/O request */
		if (_IOC_NR(cmd) != _IOC_NR(SPI_IOC_MESSAGE(0))
				|| _IOC_DIR(cmd) != _IOC_WRITE) {
			retval = -ENOTTY;
			break;
		}

		tmp = _IOC_SIZE(cmd);
		if ((tmp % sizeof(struct spi_ioc_transfer)) != 0) {
			retval = -EINVAL;
			break;
		}
		n_ioc = tmp / sizeof(struct spi_ioc_transfer);
		if (n_ioc == 0)
			break;

		/* copy into scratch area */
		ioc = kmalloc(tmp, GFP_KERNEL);
		if (!ioc) {
			retval = -ENOMEM;
			break;
		}
		if (__copy_from_user(ioc, (void __user *)arg, tmp)) {
			kfree(ioc);
			retval = -EFAULT;
			break;
		}

		/* translate to spi_message, execute */
		retval = spidev_message(spidev, ioc, n_ioc);
		kfree(ioc);
		break;
	}

	mutex_unlock(&spidev->buf_lock);
	spi_dev_put(spi);
	return retval;
}

unsigned char ssi_readwrite_byte(void __iomem *base, unsigned char txdata,unsigned char TRflag)
{
	uint32_t  spirxdata = 0;
	uint32_t  spitxdata = txdata;
	uint32_t  i= 0;
	uint32_t  reg=0;

	if(TRflag == 0){
		i=0;
		while(1){
			i++;
			reg = readl(spi_sts);

			if( !!((reg & (1 << 2))) == 1){//TXFIFO is empty
				break;
			}
			if(i>0xff0){
				break;
			}
		}
	}

	writel(txdata, spi_data);

	if(TRflag == 1){
		i= 0;
		reg = readl(spi_sts);
		while((reg & (1 << 3)) == 0){//RXFIFO is empty
			i++;
			reg = readl(spi_sts);
			if(i>0xff0){
				break;
			}
		}
		spirxdata=readl(spi_data);
	}
	return spirxdata;
}

int ssi_write_nbytes(unsigned char *txbuf,int len)
{

	int j,ia,ib;
	uint32_t i;
	uint32_t rxdata = 0;
	unsigned char *tx;
	uint32_t  reg=0;


	tx = txbuf;
	ia = len/64;
	ib = len%64;

	for(j=0;j<ia;j++){
		for(i=0;i<64;i++){
			rxdata= ssi_readwrite_byte(SPI_BASE,tx[i],0);
		}
		tx +=64;
	}
	for(i=0;i<ib;i++){
		rxdata= ssi_readwrite_byte(SPI_BASE,tx[i],0);
	}

	i=0;
	while(1){
		i++;
		reg = readl(spi_sts);

		if( !!((reg & (1 << 2))) == 1){//TXFIFO is empty
			break;
		}
		if(i>0xff0){
			return -1;
		}
	}

	return 0;
}

int ssi_read_nbytes(unsigned char* rxbuf,unsigned int len)
{
	int j,ia,ib;
	uint32_t i;
	uint32_t txdata = 0xff;
	uint32_t rxdata = 0;
	unsigned char *rx;
	uint32_t  reg=0;

	rx = rxbuf;
	ia = len/64;
	ib = len%64;

	i=0;
	while(1){
		reg = readl(spi_sts);
		if((reg & (1 << 3)) == 0){//No valid data in RXFIFO
			break;
		}
		rxdata=readl(spi_data);
		i++;
		if(i>0xff0){
			return -1;
		}
	}

	for(j=0;j<ia;j++){
		for(i=0;i<64;i++){
			rx[i]= ssi_readwrite_byte(SPI_BASE,txdata,1);
		}
		rx +=64;
	}
	for(i=0;i<ib;i++){
		rx[i]= ssi_readwrite_byte(SPI_BASE,txdata,1);
	}
	return 0;
}

static void spi_ch_init_gh()
{
	u32 reg = 0;
	reg=readl(SSI_CLK); //
	reg |= (1 << SSI_CLK_EN);
	writel(reg,SSI_CLK);

	reg = 0;
	reg = readl(SSI_EN);
	reg &= ~LC_SSI_EN; // disable SSI
	writel(reg, SSI_EN);

	reg = 0;
	writel(reg, SSI_CTRL0);
	reg = (1 << 7) | (1 << 6) | 0x7;
	writel(reg, SSI_CTRL0);

	reg = 0x08;  //0x10 -> 10M
	writel(reg, SSI_BUAD);

	reg = 0x0F;
	writel(reg, SSI_TXFTL);

	writel(reg, SSI_RXFTL);

	reg = 0x00;
	writel(reg, SSI_IE);

	reg = readl(SSI_EN);
	reg |= LC_SSI_EN; // enable SSI
	writel(reg, SSI_EN);
}


static int cs_set(int state){

	gpio_request(ssi_ssn_pin, "ssi_ssn_pin");
        gpio_direction_output(ssi_ssn_pin, state);
        gpio_free(ssi_ssn_pin);
}

static ssize_t spi_write_gh(struct file *filp, const char __user *buf, size_t cnt, loff_t *off)
{
	unsigned char *data;
	long err = 0;
	unsigned int len;

	if(cnt<1)
		data = kzalloc(1, GFP_KERNEL);
	else
		data = kzalloc(cnt, GFP_KERNEL);

	len = (unsigned int)cnt;
	err = copy_from_user(data, buf,cnt);

	cs_set(0);	
	ssi_write_nbytes(data,len);
	cs_set(1);	

	kfree(data);
	return len;
}

static ssize_t spi_read_gh(struct file *filp, char __user *buf, size_t cnt, loff_t *off)
{
	unsigned char *data;
	long err = 0;
	unsigned int len;

	if(cnt<1)
		data = kzalloc(1, GFP_KERNEL);
	else
		data = kzalloc(cnt, GFP_KERNEL);

	len = (unsigned int)cnt;
	memset(data,0,cnt);
	cs_set(0);
	ssi_read_nbytes(data,len);
	cs_set(1);

	err = copy_to_user(buf, data,len);

	kfree(data);
	return len;
}
static long
spidev_ioctl_gh(struct file *filp, unsigned int cmd, unsigned long arg)
{
	struct spidev_data	*spidev;
	spidev = filp->private_data;
	struct spi_ioc_transfer	*tr;
	tr = kzalloc(sizeof(struct spi_ioc_transfer), GFP_KERNEL);
	char recv_temp[4] = {0};
	int retval = 0;
	/* Check type and command number */
	if (_IOC_TYPE(cmd) != SPI_IOC_MAGIC) {
		printk(" cmd error \n");
		return -ENOTTY;
	}
	switch (cmd) {
	case SPI_IOC_RD_MODE:
	case SPI_IOC_WR_MODE:
	case SPI_IOC_RD_LSB_FIRST:
	case SPI_IOC_WR_LSB_FIRST:
	case SPI_IOC_RD_MAX_SPEED_HZ:
	case SPI_IOC_WR_MAX_SPEED_HZ:
	case SPI_IOC_RD_BITS_PER_WORD:
	case SPI_IOC_WR_BITS_PER_WORD:
	case XDJA_SPI_IOC_WD_GPIO_LOW:
	case XDJA_SPI_IOC_WD_GPIO_HIGH:
	case XDJA_SPI_IOC_RD_GPIO:
		return retval;
	default:
		mutex_lock(&spidev->buf_lock);
		memset(tr,0,sizeof(tr));
		//get_user
		copy_from_user(tr, (struct spi_ioc_transfer __user *)arg, sizeof(struct spi_ioc_transfer));

		copy_from_user(recv_temp, (char *)tr->tx_buf, 4);

		if( (recv_temp[0]) == 0xff && (recv_temp[1]) == 0xfe && (recv_temp[2]) == 0xfd && (recv_temp[3]) == 0xfc){
			retval = spi_read_gh(filp, (void *) (tr->rx_buf), tr->len, 0);
		}
		else{
			retval = spi_write_gh(filp, (void *) (tr->tx_buf), tr->len, 0);
		}
		kfree(tr);
		mutex_unlock(&spidev->buf_lock);
		return retval;
	}
}

#ifdef CONFIG_COMPAT
static long
spidev_compat_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	return spidev_ioctl(filp, cmd, (unsigned long)compat_ptr(arg));
}
#else
#define spidev_compat_ioctl NULL
#endif /* CONFIG_COMPAT */

static int spidev_open(struct inode *inode, struct file *filp)
{
	struct spidev_data	*spidev;
	int			status = -ENXIO;

	mutex_lock(&device_list_lock);

	list_for_each_entry(spidev, &device_list, device_entry) {
		if (spidev->devt == inode->i_rdev) {
			status = 0;
			break;
		}
	}
	if (status == 0) {
		if (!spidev->buffer) {
#ifndef USING_DMA
			spidev->buffer = kmalloc(bufsiz, GFP_KERNEL);
			if (!spidev->buffer) {
				dev_dbg(&spidev->spi->dev, "open/ENOMEM\n");
				status = -ENOMEM;
			}
#else
		    spidev->spi->dev.coherent_dma_mask = ~0;
		    spidev->buffer = dma_alloc_coherent(&spidev->spi->dev,
		    				      PAGE_SIZE*2,
		    				      &spidev->spi_tx_dma,
		    				      GFP_DMA);
		    spidev->rx_buffer = spidev->buffer + PAGE_SIZE;
		    spidev->spi_rx_dma = spidev->spi_tx_dma + PAGE_SIZE;
#endif
		}
		if (status == 0) {
			spidev->users++;
			filp->private_data = spidev;
			nonseekable_open(inode, filp);
		}
	} else
		pr_debug("spidev: nothing for minor %d\n", iminor(inode));

	mutex_unlock(&device_list_lock);
	return status;
}

static int spidev_open_gh(struct inode *inode, struct file *filp)
{
	struct spidev_data	*spidev;
	int			status = -ENXIO;

	mutex_lock(&device_list_lock);

	list_for_each_entry(spidev, &device_list, device_entry) {
		if (spidev->devt == inode->i_rdev) {
			status = 0;
			break;
		}
	}
	filp->private_data = spidev;

	mutex_unlock(&device_list_lock);
	return status;
}

static int spidev_release_gh(struct inode *inode, struct file *filp)
{
	return 0;
}

static int spidev_release(struct inode *inode, struct file *filp)
{
	struct spidev_data	*spidev;
	int			status = 0;

	mutex_lock(&device_list_lock);
	spidev = filp->private_data;
	filp->private_data = NULL;

	/* last close? */
	spidev->users--;
	if (!spidev->users) {
		int		dofree;

#ifndef USING_DMA
		kfree(spidev->buffer);
#else
		dma_free_coherent(&spidev->spi->dev, PAGE_SIZE*2,
				  spidev->buffer, spidev->spi_tx_dma);
#endif
		spidev->buffer = NULL;

		/* ... after we unbound from the underlying device? */
		spin_lock_irq(&spidev->spi_lock);
		dofree = (spidev->spi == NULL);
		spin_unlock_irq(&spidev->spi_lock);

		if (dofree)
			kfree(spidev);
	}
	mutex_unlock(&device_list_lock);

	return status;
}
#if 0
static const struct file_operations spidev_fops = {
	.owner =	THIS_MODULE,
	/* REVISIT switch to aio primitives, so that userspace
	 * gets more complete API coverage.  It'll simplify things
	 * too, except for the locking.
	 */
	.write =	spidev_write,
	.read =		spidev_read,
	.unlocked_ioctl = spidev_ioctl,
	.compat_ioctl = spidev_compat_ioctl,
	.open =		spidev_open,
	.release =	spidev_release,
	.llseek =	no_llseek,
};
#else
static const struct file_operations spidev_fops = {
	.owner =	THIS_MODULE,
	/* REVISIT switch to aio primitives, so that userspace
	 * gets more complete API coverage.  It'll simplify things
	 * too, except for the locking.
	 */
	.write =	spi_write_gh,
	.read =		spi_read_gh,
	.unlocked_ioctl = spidev_ioctl_gh,
	.compat_ioctl = spidev_compat_ioctl,
	.open =		spidev_open_gh,
	.release =	spidev_release_gh,
	.llseek =	no_llseek,
};

#endif

static int tcm_chip_detect(void){
        char tcm_startup[18] = {0x3A,0x00,0x0E,0x34,0x00,0xC1,0x00,0x00,0x00,0x0C,0x00,0x00,0x80,0x99,0x00,0x01,0xB8,0x97};
        char rx[20];
        int i = 0;
        int ret = 0;
        memset(rx,0,sizeof(rx));
        cs_set(0);
        ssi_write_nbytes(tcm_startup,sizeof(tcm_startup));
        cs_set(1);
        mdelay(10);
        cs_set(0);
        ssi_read_nbytes(rx,sizeof(rx));
        cs_set(1);

        if(rx[0] != 0x3b){
                ret = -1;
        	printk("TCM not found!\n");
	}else{
        	printk("TCM found!\n");
	}
#if 0
        for(i = 0; i<sizeof(rx); i++){
                printk(" %02x ",rx[i]);
        }
#endif
	return ret;
}


/*-------------------------------------------------------------------------*/

/* The main reason to have this class is to make mdev/udev create the
 * /dev/spidevB.C character device nodes exposing our userspace API.
 * It also simplifies memory management.
 */

static struct class *spidev_class;

/*-------------------------------------------------------------------------*/

static int spidev_probe(struct spi_device *spi)
{
	struct spidev_data	*spidev;
	int			status;
	unsigned long		minor;
	unsigned int bus_num;
	int spidev_power_gpio, spidev_reset_gpio;
	printk("tcm probe!\n");
	if (spi->dev.of_node && !of_match_device(spidev_dt_ids, &spi->dev)) {
		dev_err(&spi->dev, "buggy DT: spidev listed directly in DT\n");
		WARN_ON(spi->dev.of_node &&
			!of_match_device(spidev_dt_ids, &spi->dev));
	}
#ifdef CONFIG_OF
	/*temporary solution: hsm chip connected to spi0 in dtvl3110p*/
	if(of_property_read_u32(spi->dev.of_node,"bus_num",&bus_num)==0){
		switch(bus_num){
			case 0:
				ssi_base = SSI0_BASE;
				break;
			case 1:
				ssi_base = SSI1_BASE;
				break;
			case 2:
                                ssi_base = SSI2_BASE;
				break;
			defuat:
				ssi_base = SSI1_BASE;
		}

	}else{
		ssi_base = SSI1_BASE;
	}

	printk("ssi_base: 0x%x\n",ssi_base);
	if (spi->dev.of_node && !of_match_device(spidev_dt_ids, &spi->dev)) {
                dev_err(&spi->dev, "buggy DT: spidev listed directly in DT\n");
                WARN_ON(spi->dev.of_node &&
                        !of_match_device(spidev_dt_ids, &spi->dev));
        }
        if(of_property_read_u32(spi->dev.of_node,"ssi_ssn_pin",&ssi_ssn_pin)==0){

            //    comip_mfp_config(ssi_ssn_pin, MFP_PIN_MODE_0);
                comip_mfp_config(ssi_ssn_pin, MFP_PIN_MODE_GPIO);
                printk("TCM: set ssi_ssn_pin :%d mode 0\n",ssi_ssn_pin);
        }
        if(of_property_read_u32(spi->dev.of_node,"ssi_clk_pin",&ssi_clk_pin)==0){

                comip_mfp_config(ssi_clk_pin, MFP_PIN_MODE_0);
                printk("TCM: set ssi_clk_pin :%d mode 0\n",ssi_clk_pin);
        }

	cs_set(1);
/*
	spidev_power_gpio= of_get_named_gpio(spi->dev.of_node, "power_gpio", 0);
	if(spidev_power_gpio > 0){
		gpio_request(spidev_power_gpio, "power_gpio");
		gpio_direction_output(spidev_power_gpio, 1);
		gpio_free(spidev_power_gpio);	
	}
	spidev_reset_gpio= of_get_named_gpio(spi->dev.of_node, "reset_gpio", 0);
	gpio_request(spidev_reset_gpio, "reset_gpio");
	gpio_direction_output(spidev_reset_gpio, 1);
	gpio_free(spidev_reset_gpio);
*/
#endif
	/* Allocate driver data */
	spidev = kzalloc(sizeof(*spidev), GFP_KERNEL);
	if (!spidev)
		return -ENOMEM;

	/* Initialize the driver data */
	spidev->spi = spi;
	spin_lock_init(&spidev->spi_lock);
	mutex_init(&spidev->buf_lock);

	INIT_LIST_HEAD(&spidev->device_entry);

	/* If we can allocate a minor number, hook up this device.
	 * Reusing minors is fine so long as udev or mdev is working.
	 */
	mutex_lock(&device_list_lock);
	minor = find_first_zero_bit(minors, N_SPI_MINORS);
	if (minor < N_SPI_MINORS) {
		struct device *dev;

		spidev->devt = MKDEV(SPIDEV_MAJOR, minor);
		dev = device_create(spidev_class, &spi->dev, spidev->devt,
				    spidev, "tcm",
				    spi->master->bus_num, spi->chip_select);
		status = PTR_RET(dev);
	} else {
		dev_dbg(&spi->dev, "no minor number available!\n");
		status = -ENODEV;
	}
	if (status == 0) {
		set_bit(minor, minors);
		list_add(&spidev->device_entry, &device_list);
	}
	mutex_unlock(&device_list_lock);

	if (status == 0)
		spi_set_drvdata(spi, spidev);
	else
		kfree(spidev);

	spi_data  = ioremap(BASE+0x60,4);
	spi_sts   = ioremap(BASE+0x28,4);
	spi_ch_init_gh();

	status = tcm_chip_detect();

	return status;
}

static int spidev_remove(struct spi_device *spi)
{
	struct spidev_data	*spidev = spi_get_drvdata(spi);

	/* make sure ops on existing fds can abort cleanly */
	spin_lock_irq(&spidev->spi_lock);
	spidev->spi = NULL;
	spi_set_drvdata(spi, NULL);
	spin_unlock_irq(&spidev->spi_lock);

	/* prevent new opens */
	mutex_lock(&device_list_lock);
	list_del(&spidev->device_entry);
	device_destroy(spidev_class, spidev->devt);
	clear_bit(MINOR(spidev->devt), minors);
	if (spidev->users == 0)
		kfree(spidev);
	mutex_unlock(&device_list_lock);

	return 0;
}

static struct spi_driver spidev_spi_driver = {
	.driver = {
		.name =		"tcm",
		.owner =	THIS_MODULE,
		.of_match_table = of_match_ptr(spidev_dt_ids),
	},
	.probe =	spidev_probe,
	.remove =	spidev_remove,

	/* NOTE:  suspend/resume methods are not necessary here.
	 * We don't do anything except pass the requests to/from
	 * the underlying controller.  The refrigerator handles
	 * most issues; the controller driver handles the rest.
	 */
};

/*-------------------------------------------------------------------------*/

static int __init spidev_init(void)
{
	int status;

	/* Claim our 256 reserved device numbers.  Then register a class
	 * that will key udev/mdev to add/remove /dev nodes.  Last, register
	 * the driver which manages those device numbers.
	 */
	BUILD_BUG_ON(N_SPI_MINORS > 256);
	status = register_chrdev(SPIDEV_MAJOR, "tcm", &spidev_fops);
	if (status < 0)
		return status;

	spidev_class = class_create(THIS_MODULE, "tcm");
	if (IS_ERR(spidev_class)) {
		unregister_chrdev(SPIDEV_MAJOR, spidev_spi_driver.driver.name);
		return PTR_ERR(spidev_class);
	}
	status = spi_register_driver(&spidev_spi_driver);
	if (status < 0) {
		class_destroy(spidev_class);
		unregister_chrdev(SPIDEV_MAJOR, spidev_spi_driver.driver.name);
	}	
	return status;
}
module_init(spidev_init);

static void __exit spidev_exit(void)
{
	spi_unregister_driver(&spidev_spi_driver);
	class_destroy(spidev_class);
	unregister_chrdev(SPIDEV_MAJOR, spidev_spi_driver.driver.name);
}
module_exit(spidev_exit);

MODULE_AUTHOR("Andrea Paterniani, <a.paterniani@swapp-eng.it>");
MODULE_DESCRIPTION("User mode SPI device interface");
MODULE_LICENSE("GPL");
MODULE_ALIAS("spi:spidev");
