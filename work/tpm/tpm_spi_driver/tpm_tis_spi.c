/*
 * Copyright (C) 2015 Infineon Technologies AG
 * Copyright (C) 2016 STMicroelectronics SAS
 *
 * Authors:
 * Peter Huewe <peter.huewe@infineon.com>
 * Christophe Ricard <christophe-h.ricard@st.com>
 *
 * Maintained by: <tpmdd-devel@lists.sourceforge.net>
 *
 * Device driver for TCG/TCPA TPM (trusted platform module).
 * Specifications at www.trustedcomputinggroup.org
 *
 * This device driver implements the TPM interface as defined in
 * the TCG TPM Interface Spec version 1.3, revision 27 via _raw/native
 * SPI access_.
 *
 * It is based on the original tpm_tis device driver from Leendert van
 * Dorn and Kyleen Hall and Jarko Sakkinnen.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, version 2 of the
 * License.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/wait.h>
#include <linux/acpi.h>
#include <linux/freezer.h>

#include <linux/module.h>
#include <linux/spi/spi.h>
#include <linux/gpio.h>
#include <linux/of_irq.h>
#include <linux/of_gpio.h>
#include <linux/tpm.h>
#include <plat/mfp.h>
#include "tpm.h"
#include "tpm_tis_core.h"


#include <linux/dma-mapping.h>

#define USING_DMA 
#define MAX_SPI_FRAMESIZE 64

struct tpm_tis_spi_phy {
	struct tpm_tis_data priv;
	struct spi_device *spi_device;
	u8 *iobuf;
#ifdef USING_DMA
	u8	*tx_buffer;
        u8	*rx_buffer;
	dma_addr_t spi_tx_dma;
        dma_addr_t spi_rx_dma;
#endif
};

struct tpm_data{
	dev_t                   devt;
        spinlock_t              spi_lock;
        struct spi_device       *spi;
        struct list_head        device_entry;

	/* buffer is NULL unless this device is open (users > 0) */
        struct mutex            buf_lock;
        unsigned                users;
        u8                      *tx_buffer;
        u8                      *rx_buffer;
        dma_addr_t spi_tx_dma;
        dma_addr_t spi_rx_dma;
	struct tpm_tis_spi_phy spi_phy;
};


struct tpm_data *tpm_dev;

static inline struct tpm_tis_spi_phy *to_tpm_tis_spi_phy(struct tpm_tis_data *data)
{
	return container_of(data, struct tpm_tis_spi_phy, priv);
}

static int spi_cs_pin_gpio = 164;

static int comip_spi_cs_state(int level)
{
	comip_mfp_config(MFP_PIN_GPIO(164), MFP_PIN_MODE_GPIO);
        gpio_request(spi_cs_pin_gpio, "COMIP SPI CS");
        gpio_direction_output(spi_cs_pin_gpio, !!level);
        gpio_free(spi_cs_pin_gpio);
        return 0;
}


static int tpm_tis_transfer_dma(struct tpm_tis_data *data, u16 len, u8 *rx_buffer, const u8 *tx_buffer){
	
	struct tpm_tis_spi_phy *phy = to_tpm_tis_spi_phy(data);
	
	int i;
	int ret = 0;
	struct spi_message m;
	struct spi_transfer spi_xfer;
	u8 transfer_len;

	phy->iobuf = tx_buffer;
	
	struct spi_transfer k_tmp;
	memset(&k_tmp, 0, sizeof(k_tmp));
	spi_message_init(&m);
	
	m.is_dma_mapped = 1;
	k_tmp.len = len;
	memcpy(phy->tx_buffer,phy->iobuf,k_tmp.len);	
	
	k_tmp.tx_buf = phy->tx_buffer;
	k_tmp.rx_buf = phy->rx_buffer;
	
	k_tmp.tx_dma = phy->spi_tx_dma;
	k_tmp.rx_dma = phy->spi_rx_dma;
	
	spi_message_add_tail(&k_tmp, &m);
	ret = spi_sync_locked(phy->spi_device, &m);
	if (ret < 0) {
		printk("%s %d: spi_sync_locked ret[%d].\n", __func__, __LINE__, ret);
		goto exit;
	}


	if(rx_buffer)
		memcpy(in, rx_buffer, len);
	
	return 0;
}

static int tpm_tis_spi_transfer(struct tpm_tis_data *data, u32 addr, u16 len,
				u8 *in, const u8 *out)
{
	struct tpm_tis_spi_phy *phy = to_tpm_tis_spi_phy(data);
	int ret = 0;
	int i;
	struct spi_message m;
	struct spi_transfer spi_xfer;
	u8 transfer_len;

#ifdef USING_DMA	
	struct spi_transfer k_tmp;
	memset(&k_tmp, 0, sizeof(k_tmp));
#endif
	comip_spi_cs_state(0);

	spi_bus_lock(phy->spi_device->master);

	while (len) {
		transfer_len = min_t(u16, len, MAX_SPI_FRAMESIZE);
		phy->iobuf[0] = (in ? 0x80 : 0) | (transfer_len - 1);
		phy->iobuf[1] = 0xd4;
		phy->iobuf[2] = addr >> 8;
		phy->iobuf[3] = addr;

		memset(&spi_xfer, 0, sizeof(spi_xfer));
		spi_xfer.tx_buf = phy->iobuf;
		spi_xfer.rx_buf = phy->iobuf;
		spi_xfer.len = 4;
		spi_xfer.cs_change = 1;

		spi_message_init(&m);
#ifdef USING_DMA
		m.is_dma_mapped = 1;
		k_tmp.len = 4;
		memcpy(phy->tx_buffer,phy->iobuf,k_tmp.len);	
		
		k_tmp.tx_buf = phy->tx_buffer;
		k_tmp.rx_buf = phy->rx_buffer;
		
		k_tmp.tx_dma = phy->spi_tx_dma;
		k_tmp.rx_dma = phy->spi_rx_dma;
	
		spi_message_add_tail(&k_tmp, &m);
		ret = spi_sync_locked(phy->spi_device, &m);
#else
		spi_message_add_tail(&spi_xfer, &m);
		ret = spi_sync_locked(phy->spi_device, &m);
#endif
		if (ret < 0) {
			printk("%s %d: spi_sync_locked ret[%d].\n", __func__, __LINE__, ret);
			goto exit;
		}
#ifdef USING_DMA	
		if(((u8 *)k_tmp.rx_buf)[3] & 0x01 == 0){
#else
		if ((phy->iobuf[3] & 0x01) == 0) {
#endif
			for (i = 0; i < TPM_RETRY; i++) {
				spi_xfer.len = 1;
				spi_message_init(&m);
#ifdef USING_DMA
				m.is_dma_mapped = 1;
				k_tmp.len = 1;
		                memcpy(phy->tx_buffer,phy->iobuf,k_tmp.len);
	
        		        k_tmp.tx_buf = phy->tx_buffer;
				k_tmp.rx_buf = phy->rx_buffer;

		                k_tmp.tx_dma = phy->spi_tx_dma;
		                k_tmp.rx_dma = phy->spi_rx_dma;

		                spi_message_add_tail(&k_tmp, &m);
		                ret = spi_sync_locked(phy->spi_device, &m);

#else
				spi_message_add_tail(&spi_xfer, &m);
				ret = spi_sync_locked(phy->spi_device, &m);
#endif		
				if (ret < 0)
					goto exit;

#ifdef USING_DMA
				if(((u8 *)k_tmp.rx_buf)[0] & 0x01)
					break;
#else
				if (phy->iobuf[0] & 0x01)
					break;
#endif

			}

			if (i == TPM_RETRY) {
				ret = -ETIMEDOUT;
				goto exit;
			}
		}

		spi_xfer.cs_change = 0;
		spi_xfer.len = transfer_len;
		spi_xfer.delay_usecs = 5;

		if (in) {
			spi_xfer.tx_buf = NULL;
		} else if (out) {
			spi_xfer.rx_buf = NULL;
			memcpy(phy->iobuf, out, transfer_len);
			out += transfer_len;
		}
		spi_message_init(&m);

#ifdef USING_DMA	
		m.is_dma_mapped = 1;
		k_tmp.len = transfer_len;
                memcpy(phy->tx_buffer,phy->iobuf,k_tmp.len);

                k_tmp.tx_buf = phy->tx_buffer;
                k_tmp.rx_buf = phy->rx_buffer;

                k_tmp.tx_dma = phy->spi_tx_dma;
                k_tmp.rx_dma = phy->spi_rx_dma;

                spi_message_add_tail(&k_tmp, &m);
                ret = spi_sync_locked(phy->spi_device, &m);
#else
		spi_message_add_tail(&spi_xfer, &m);
		ret = spi_sync_locked(phy->spi_device, &m);
		
#endif
		if (ret < 0)
			goto exit;

		if (in) {
#ifdef USING_DMA
			memcpy(in, k_tmp.rx_buf, transfer_len);
#else
			memcpy(in, phy->iobuf, transfer_len);
#endif
			in += transfer_len;
		}

		len -= transfer_len;
	}

exit:
	comip_spi_cs_state(1); //cs release
	spi_bus_unlock(phy->spi_device->master);
	return ret;
}

static int tpm_tis_spi_read_bytes(struct tpm_tis_data *data, u32 addr,
				  u16 len, u8 *result)
{
	return tpm_tis_spi_transfer(data, addr, len, result, NULL);
}

static int tpm_tis_spi_write_bytes(struct tpm_tis_data *data, u32 addr,
				   u16 len, const u8 *value)
{
	return tpm_tis_spi_transfer(data, addr, len, NULL, value);
}

static int tpm_tis_spi_read16(struct tpm_tis_data *data, u32 addr, u16 *result)
{
	int rc;

	rc = data->phy_ops->read_bytes(data, addr, sizeof(u16), (u8 *)result);
	if (!rc)
		*result = le16_to_cpu(*result);
	return rc;
}

static int tpm_tis_spi_read32(struct tpm_tis_data *data, u32 addr, u32 *result)
{
	int rc;

	rc = data->phy_ops->read_bytes(data, addr, sizeof(u32), (u8 *)result);
	if (!rc)
		*result = le32_to_cpu(*result);
	return rc;
}

static int tpm_tis_spi_write32(struct tpm_tis_data *data, u32 addr, u32 value)
{
	value = cpu_to_le32(value);
	return data->phy_ops->write_bytes(data, addr, sizeof(u32),
					   (u8 *)&value);
}

static int tpm_dma_init(void){
	int	status = 0;;
	tpm_dev->tx_buffer = dma_alloc_coherent(&tpm_dev->spi->dev,
                                                      PAGE_SIZE*2,
                                                      &tpm_dev->spi_tx_dma,
                                                      GFP_DMA);
	if(tpm_dev->tx_buffer == NULL)
		status = -ENXIO;	

	tpm_dev->rx_buffer = tpm_dev->tx_buffer + PAGE_SIZE;
	tpm_dev->spi_rx_dma = tpm_dev->spi_tx_dma + PAGE_SIZE;
	
	return status;	
}

static const struct tpm_tis_phy_ops tpm_spi_phy_ops = {
	.read_bytes = tpm_tis_spi_read_bytes,
	.write_bytes = tpm_tis_spi_write_bytes,
	.read16 = tpm_tis_spi_read16,
	.read32 = tpm_tis_spi_read32,
	.write32 = tpm_tis_spi_write32,
};

static int tpm_tis_spi_probe(struct spi_device *dev)
{
	struct tpm_tis_spi_phy *phy;
	int irq;
	int	status = 0;;

	phy = devm_kzalloc(&dev->dev, sizeof(struct tpm_tis_spi_phy),
			   GFP_KERNEL);
	if (!phy)
		return -ENOMEM;

	phy->spi_device = dev;

	//phy->iobuf = devm_kmalloc(&dev->dev, MAX_SPI_FRAMESIZE, GFP_KERNEL);
	phy->iobuf = devm_kzalloc(&dev->dev, MAX_SPI_FRAMESIZE, GFP_KERNEL);
	if (!phy->iobuf)
		return -ENOMEM;

	/* If the SPI device has an IRQ then use that */
	if (dev->irq > 0)
		irq = dev->irq;
	else
		irq = -1;

		printk("%s %d called \n",__func__,__LINE__);
#ifdef USING_DMA
	phy->spi_device->dev.coherent_dma_mask = ~0;		
	phy->tx_buffer = dma_alloc_coherent(&phy->spi_device->dev,
					PAGE_SIZE*2,
					&phy->spi_tx_dma,
					GFP_DMA);
        if(phy->tx_buffer == NULL){
		status = -ENXIO;
		printk("%s %d called err occur!\n",__func__,__LINE__);
		return status;
	}

        phy->rx_buffer = phy->tx_buffer + PAGE_SIZE;
        phy->spi_rx_dma =(dma_addr_t)(phy->spi_tx_dma + PAGE_SIZE);

#endif
	return tpm_tis_core_init(&dev->dev, &phy->priv, irq, &tpm_spi_phy_ops
#ifdef CONFIG_ACPI
				 ,NULL
#endif
				);
}

static SIMPLE_DEV_PM_OPS(tpm_tis_pm, tpm_pm_suspend, tpm_tis_resume);

static int tpm_tis_spi_remove(struct spi_device *dev)
{
	struct tpm_chip *chip = spi_get_drvdata(dev);
	
	tpm_chip_unregister(chip);
	tpm_tis_remove(chip);
	return 0;
}

static const struct spi_device_id tpm_tis_spi_id[] = {
	{"tpm_tis_spi", 0},
	{}
};
MODULE_DEVICE_TABLE(spi, tpm_tis_spi_id);

static const struct of_device_id of_tis_spi_match[] = {
	{ .compatible = "tcg,tpm_tis-spi", },
	{}
};
MODULE_DEVICE_TABLE(of, of_tis_spi_match);

#ifdef CONFIG_ACPI
static const struct acpi_device_id acpi_tis_spi_match[] = {
	{"SMO0768", 0},
	{}
};
MODULE_DEVICE_TABLE(acpi, acpi_tis_spi_match);
#endif

static struct spi_driver tpm_tis_spi_driver = {
	.driver = {
		.owner = THIS_MODULE,
		.name = "tpm_tis_spi",
		.pm = &tpm_tis_pm,
		.of_match_table = of_match_ptr(of_tis_spi_match),
#ifdef CONFIG_ACPI
		.acpi_match_table = ACPI_PTR(acpi_tis_spi_match),
#endif
	},
	.probe = tpm_tis_spi_probe,
	.remove = tpm_tis_spi_remove,
	.id_table = tpm_tis_spi_id,
};
module_spi_driver(tpm_tis_spi_driver);

MODULE_DESCRIPTION("TPM Driver for native SPI access");
MODULE_LICENSE("GPL");
