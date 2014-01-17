/*
 *  drivers/media/radio/radio-si470x-i2c.c
 *
 *  Driver for I2C radios for the Silicon Labs Si470x FM Radio Receivers:
 *
 *  Copyright (c) 2010 Lijian <lijian@sunplusapp.com>
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

/*
 * History:
 * 2010-11-23	Lijian <lijian@sunplusapp.com>
 *		Version 1.0.0
 *		- First working version
 *
 * ToDo:
 * -
 */

/* driver definitions */
#define DRIVER_AUTHOR "Lijian <lijian@sunplusapp.com>"
#define DRIVER_NAME "radio-si470x-i2c"
#define DRIVER_KERNEL_VERSION KERNEL_VERSION(1, 0, 0)
#define DRIVER_CARD "Silicon Labs Si470x FM Radio Receiver"
#define DRIVER_DESC "I2C radio driver for Si470x FM Radio Receivers"
#define DRIVER_VERSION "1.0.0"


/* kernel includes */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/input.h>
#include <linux/version.h>
#include <linux/videodev2.h>
#include <linux/mutex.h>
#include <media/v4l2-common.h>
#include <media/v4l2-ioctl.h>
#include <media/rds.h>
#include <asm/unaligned.h>
#include <plat/s3c6410.h>
#include <linux/delay.h>
#include <mach/gpio.h>
#include <plat/gpio-cfg.h>
#include <plat/regs-gpio.h>
#include "si470x.h"

/**************************************************************************
 * Module Parameters
 **************************************************************************/

/* Radio Nr */
static int radio_nr = -1;
module_param(radio_nr, int, 0444);
MODULE_PARM_DESC(radio_nr, "Radio Nr");

/* Spacing (kHz) */
/* 0: 200 kHz (USA, Australia) */
/* 1: 100 kHz (Europe, Japan) */
/* 2:  50 kHz */
static unsigned short space = 2;
module_param(space, ushort, 0444);
MODULE_PARM_DESC(space, "Spacing: 0=200kHz 1=100kHz *2=50kHz*");

/* Bottom of Band (MHz) */
/* 0: 87.5 - 108 MHz (USA, Europe)*/
/* 1: 76   - 108 MHz (Japan wide band) */
/* 2: 76   -  90 MHz (Japan) */
static unsigned short band = 1;
module_param(band, ushort, 0444);
MODULE_PARM_DESC(band, "Band: 0=87.5..108MHz *1=76..108MHz* 2=76..90MHz");

/* De-emphasis */
/* 0: 75 us (USA) */
/* 1: 50 us (Europe, Australia, Japan) */
static unsigned short de = 1;
module_param(de, ushort, 0444);
MODULE_PARM_DESC(de, "De-emphasis: 0=75us *1=50us*");

/* Tune timeout */
static unsigned int tune_timeout = 3000;
module_param(tune_timeout, uint, 0644);
MODULE_PARM_DESC(tune_timeout, "Tune timeout: *3000*");

/* Seek timeout */
static unsigned int seek_timeout = 5000;
module_param(seek_timeout, uint, 0644);
MODULE_PARM_DESC(seek_timeout, "Seek timeout: *5000*");

/* RDS buffer blocks */
static unsigned int rds_buf = 100;
module_param(rds_buf, uint, 0444);
MODULE_PARM_DESC(rds_buf, "RDS buffer entries: *100*");

/* RDS maximum block errors */
static unsigned short max_rds_errors = 1;
/* 0 means   0  errors requiring correction */
/* 1 means 1-2  errors requiring correction (used by original USBRadio.exe) */
/* 2 means 3-5  errors requiring correction */
/* 3 means   6+ errors or errors in checkword, correction not possible */
module_param(max_rds_errors, ushort, 0644);
MODULE_PARM_DESC(max_rds_errors, "RDS maximum block errors: *1*");

/* RDS poll frequency */
static unsigned int rds_poll_time = 40;
/* 40 is used by the original USBRadio.exe */
/* 50 is used by radio-cadet */
/* 75 should be okay */
/* 80 is the usual RDS receive interval */
module_param(rds_poll_time, uint, 0644);
MODULE_PARM_DESC(rds_poll_time, "RDS poll time (ms): *40*");


/**************************************************************************
 * Software/Hardware Versions
 **************************************************************************/
#define RADIO_SW_VERSION_NOT_BOOTLOADABLE	6
#define RADIO_SW_VERSION					7
#define RADIO_SW_VERSION_CURRENT			15
#define RADIO_HW_VERSION					1

#define SCRATCH_PAGE_SW_VERSION				1
#define SCRATCH_PAGE_HW_VERSION				2

/**************************************************************************
 * Hardware I/O Pin OP Functions
 **************************************************************************/
static void si470x_hw_reset(void)
{
	s3c_gpio_cfgpin(S3C64XX_GPK(6), S3C_GPIO_SFN(1));
	s3c_gpio_setpin(S3C64XX_GPK(6), 0);
	udelay(200);
	s3c_gpio_setpin(S3C64XX_GPK(6), 1);
}

/**************************************************************************
 * General Driver Definitions
 **************************************************************************/
/*
 * si470x_device - private data
 */
struct si470x_device {
	/* reference to video device */
	struct video_device *videodev;

	/* driver management */
	unsigned int users;
	unsigned char disconnected;
	struct mutex disconnect_lock;

	/* Silabs internal registers (0..15) */
	unsigned short registers[RADIO_REGISTER_NUM];

	/* RDS receive buffer */
	struct delayed_work work;
	wait_queue_head_t read_queue;
	struct mutex lock;		/* buffer locking */
	unsigned char *buffer;		/* size is always multiple of three */
	unsigned int buf_size;
	unsigned int rd_index;
	unsigned int wr_index;
};
static struct si470x_device *radio;

/*
 * The frequency is set in units of 62.5 Hz when using V4L2_TUNER_CAP_LOW,
 * 62.5 kHz otherwise.
 * The tuner is able to have a channel spacing of 50, 100 or 200 kHz.
 * tuner->capability is therefore set to V4L2_TUNER_CAP_LOW
 * The FREQ_MUL is then: 1 MHz / 62.5 Hz = 16000
 */
#define FREQ_MUL (1000000 / 62.5)

/**************************************************************************
 * General Driver Functions
 **************************************************************************/
#define si470x_be16(be16)										\
	((((u16)(((u8*)(be16))[0])) << 8) | (((u8*)(be16))[1]))
#define si470x_put_be16(le16, buf)								\
	do {														\
		(((u8*)(buf))[0]) = (((u16)(le16) >> 8) & 0xFF);		\
		(((u8*)(buf))[1]) = (le16) & 0xFF;						\
	} while(0)
static void dbg_dump_regs(struct si470x_device *radio, const char *tip)
{
    int i;
    printk("%s\n", tip);
    for(i = 0; i < 8; i++)
        printk(" %04X", radio->registers[i]);
    printk("\n");
    for(i = 8; i < 16; i++)
        printk(" %04X", radio->registers[i]);
    printk("\n");
}
static void dbg_dump_mem(void *base, int size, int linecount, const char *tip)
{
    char convchar(u8 ch) {
        if((ch >= ' ') && (ch <= 0x7F))
            return ch;
        return '.';
    }
    u8 *p = (u8 *)base;
    int row = size / linecount;
    int rest = size % linecount;
    int i;
    printk("[%p:%d] %s\n", base, size, tip);
    for(i = 0; i < row; i++)
    {
        int j;
        printk("%p:", p);
        for(j = 0; j < linecount; j++)
            printk(" %02X", p[j]);
        printk(" | ");
        for(j = 0; j < linecount; j++)
            printk("%c", convchar(*p++));
        printk("\n");
    }
    if(rest)
    {
        int j;
        printk("%p:", p);
        for(j = 0; j < rest; j++)
            printk(" %02X", p[j]);
        for(; j < linecount; j++)
            printk("   ");
        printk(" | ");
        for(j = 0; j < rest; j++)
            printk("%c", convchar(*p++));
        printk("\n");
    }
}
/* NOTE:
 * The si470x read operation start from the upper byte of 0x0A reg and
 * automatically increase the address counter.
 *
 * The si470x write operation start from the upper byte of 0x02 reg and
 * automatically increase the address counter.
*/
#if 0
static u8 si470x_rdmap_reg2idx[] = {
    0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05
};
static u8 si470x_rdmap_idx2reg[] = {
    0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09
};
#endif
/*
 * si470x_get_register - read register
 */
static int si470x_get_register(struct si470x_device *radio, int regnr)
{
    u16 buf[RADIO_REGISTER_NUM];
	int retval;
    int idx = regnr;
    if(idx< 0x0A)
        idx += RADIO_REGISTER_NUM;
    idx -= 0x0A;
    retval = s3c6410_softi2c_xfer16(SI470X_ADDR, 1, buf, idx + 1);
//	retval = s3c6410_softi2c_xfer(SI470X_ADDR, 1, buf, (idx + 1) * RADIO_REGISTER_SIZE);
	if(retval == 0)
	{
        radio->registers[regnr] = buf[idx];
//		radio->registers[regnr] = si470x_be16(&buf[idx * RADIO_REGISTER_SIZE]);
	}
	else
		printk(KERN_INFO DRIVER_DESC ": %s xfer = %d\n", __func__, retval);
	return (retval == 0) ? 0 : -EINVAL;
}

/*
 * si470x_set_register - write register
 */
static int si470x_set_register(struct si470x_device *radio, int regnr)
{
     u16 buf[RADIO_REGISTER_NUM - 2];
//	u8 buf[(RADIO_REGISTER_NUM - 2) * RADIO_REGISTER_SIZE];//skip the first 2 regs
	int retval;
	int tmpnr;
    if(regnr < 2)
        return -EINVAL;
    regnr -= 2;
	for(tmpnr = 0; tmpnr <= regnr; tmpnr++)
        buf[tmpnr] = radio->registers[tmpnr + 2];
//		si470x_put_be16(radio->registers[tmpnr + 2], &buf[tmpnr * RADIO_REGISTER_SIZE]);
    retval = s3c6410_softi2c_xfer16(SI470X_ADDR, 0, buf, regnr + 1);
//	retval = s3c6410_softi2c_xfer(SI470X_ADDR, 0, buf, (regnr + 1) * RADIO_REGISTER_SIZE);
	if(retval)
		printk(KERN_INFO DRIVER_DESC ": %s xfer = %d\n", __func__, retval);
	return (retval == 0) ? 0 : -EINVAL;
}

/*
 * si470x_get_all_registers - read entire registers
 */
static int si470x_get_all_registers(struct si470x_device *radio)
{
    u16 buf[RADIO_REGISTER_NUM];
//	u8 buf[RADIO_REGISTER_NUM * RADIO_REGISTER_SIZE];
	int retval;
	int regnr;
    retval = s3c6410_softi2c_xfer16(SI470X_ADDR, 1, buf, RADIO_REGISTER_NUM);
//	retval = s3c6410_softi2c_xfer(SI470X_ADDR, 1, buf, RADIO_REGISTER_NUM * RADIO_REGISTER_SIZE);
	if(retval == 0)
	{
        for(regnr = STATUSRSSI; regnr < RADIO_REGISTER_NUM; regnr++)
            radio->registers[regnr] = buf[regnr - STATUSRSSI];
//            radio->registers[regnr] = si470x_be16(&buf[(regnr - 0x0A) * RADIO_REGISTER_SIZE]);
		for(regnr = 0; regnr < STATUSRSSI; regnr++)
            radio->registers[regnr] = buf[regnr + RDS_REGISTER_NUM];
//			radio->registers[regnr] = si470x_be16(&buf[(regnr + (RADIO_REGISTER_NUM - 0x0A)) * RADIO_REGISTER_SIZE]);
	}
	else
		printk(KERN_INFO DRIVER_DESC ": %s xfer = %d\n", __func__, retval);
	return (retval == 0) ? 0 : -EINVAL;
}

/*
 * si470x_get_rds_registers - read rds registers
 */
static int si470x_get_rds_registers(struct si470x_device *radio)
{
    u16 buf[RADIO_REGISTER_NUM];
//	u8 buf[RADIO_REGISTER_NUM * RADIO_REGISTER_SIZE];
	int retval;
	int regnr;
    retval = s3c6410_softi2c_xfer16(SI470X_ADDR, 1, buf, RDS_REGISTER_NUM);
//	retval = s3c6410_softi2c_xfer(SI470X_ADDR, 1, buf, RADIO_REGISTER_NUM * RADIO_REGISTER_SIZE);
	if(retval == 0)
	{
		for(regnr = STATUSRSSI; regnr < RADIO_REGISTER_NUM; regnr++)
            radio->registers[regnr] = buf[regnr - STATUSRSSI];
//			radio->registers[regnr] = si470x_be16(&buf[regnr * RADIO_REGISTER_SIZE]);
	}
	else
		printk(KERN_INFO DRIVER_DESC ": %s xfer = %d\n", __func__, retval);
	return (retval == 0) ? 0 : -EINVAL;
}

static int si470x_powerup(struct si470x_device *radio, int up)
{
    int retval = 0;
    if(up)
    {
        si470x_hw_reset();
        msleep(1);
        si470x_get_all_registers(radio);
        radio->registers[TEST1] |= TEST1_XOSCEN;
        retval = si470x_set_register(radio, TEST1);
        msleep(500);
        radio->registers[POWERCFG] |= (POWERCFG_DMUTE | POWERCFG_ENABLE);
        radio->registers[POWERCFG] &= ~POWERCFG_DISABLE;
        radio->registers[SYSCONFIG2] = 0x0C1F;
        radio->registers[SYSCONFIG3] = 0x0038;
        retval = si470x_set_register(radio, SYSCONFIG3);
        msleep(120);
    }
    else
    {
        radio->registers[POWERCFG] |= (POWERCFG_ENABLE | POWERCFG_DISABLE);
        radio->registers[TEST1] |= TEST1_AHIZEN;
        retval = si470x_set_register(radio, TEST1);
        msleep(300);
    }
    return retval;
}
/*
 * si470x_set_chan - set the channel
 */
static int si470x_set_chan(struct si470x_device *radio, unsigned short chan)
{
	int retval;
	unsigned long timeout;
	bool timed_out = 0;

	/* start tuning */
	radio->registers[CHANNEL] &= ~CHANNEL_CHAN;
	radio->registers[CHANNEL] |= CHANNEL_TUNE | chan;
	retval = si470x_set_register(radio, CHANNEL);
	if (retval < 0)
		goto done;

	/* wait till tune operation has completed */
	timeout = jiffies + msecs_to_jiffies(tune_timeout);
	do {
		retval = si470x_get_register(radio, STATUSRSSI);
		if (retval < 0)
			goto stop;
		timed_out = time_after(jiffies, timeout);
	} while (((radio->registers[STATUSRSSI] & STATUSRSSI_STC) == 0) &&
		(!timed_out));
	if ((radio->registers[STATUSRSSI] & STATUSRSSI_STC) == 0)
		printk(KERN_WARNING DRIVER_NAME ": tune does not complete\n");
	if (timed_out)
		printk(KERN_WARNING DRIVER_NAME
			": tune timed out after %u ms\n", tune_timeout);

stop:
	/* stop tuning */
	radio->registers[CHANNEL] &= ~CHANNEL_TUNE;
	retval = si470x_set_register(radio, CHANNEL);

done:
	return retval;
}

/*
 * si470x_get_freq - get the frequency
 */
static int si470x_get_freq(struct si470x_device *radio, unsigned int *freq)
{
	unsigned int spacing, band_bottom;
	unsigned short chan;
	int retval;

	/* Spacing (kHz) */
	switch ((radio->registers[SYSCONFIG2] & SYSCONFIG2_SPACE) >> 4) {
	/* 0: 200 kHz (USA, Australia) */
	case 0:
		spacing = 0.200 * FREQ_MUL; break;
	/* 1: 100 kHz (Europe, Japan) */
	case 1:
		spacing = 0.100 * FREQ_MUL; break;
	/* 2:  50 kHz */
	default:
		spacing = 0.050 * FREQ_MUL; break;
	};

	/* Bottom of Band (MHz) */
	switch ((radio->registers[SYSCONFIG2] & SYSCONFIG2_BAND) >> 6) {
	/* 0: 87.5 - 108 MHz (USA, Europe) */
	case 0:
		band_bottom = 87.5 * FREQ_MUL; break;
	/* 1: 76   - 108 MHz (Japan wide band) */
	default:
		band_bottom = 76   * FREQ_MUL; break;
	/* 2: 76   -  90 MHz (Japan) */
	case 2:
		band_bottom = 76   * FREQ_MUL; break;
	};

	/* read channel */
	retval = si470x_get_register(radio, READCHAN);
	chan = radio->registers[READCHAN] & READCHAN_READCHAN;

	/* Frequency (MHz) = Spacing (kHz) x Channel + Bottom of Band (MHz) */
	*freq = chan * spacing + band_bottom;

	return retval;
}

/*
 * si470x_set_freq - set the frequency
 */
static int si470x_set_freq(struct si470x_device *radio, unsigned int freq)
{
	unsigned int spacing, band_bottom;
	unsigned short chan;

	/* Spacing (kHz) */
	switch ((radio->registers[SYSCONFIG2] & SYSCONFIG2_SPACE) >> 4) {
	/* 0: 200 kHz (USA, Australia) */
	case 0:
		spacing = 0.200 * FREQ_MUL; break;
	/* 1: 100 kHz (Europe, Japan) */
	case 1:
		spacing = 0.100 * FREQ_MUL; break;
	/* 2:  50 kHz */
	default:
		spacing = 0.050 * FREQ_MUL; break;
	};

	/* Bottom of Band (MHz) */
	switch ((radio->registers[SYSCONFIG2] & SYSCONFIG2_BAND) >> 6) {
	/* 0: 87.5 - 108 MHz (USA, Europe) */
	case 0:
		band_bottom = 87.5 * FREQ_MUL; break;
	/* 1: 76   - 108 MHz (Japan wide band) */
	default:
		band_bottom = 76   * FREQ_MUL; break;
	/* 2: 76   -  90 MHz (Japan) */
	case 2:
		band_bottom = 76   * FREQ_MUL; break;
	};

	/* Chan = [ Freq (Mhz) - Bottom of Band (MHz) ] / Spacing (kHz) */
	chan = (freq - band_bottom) / spacing;

	return si470x_set_chan(radio, chan);
}

/*
 * si470x_set_seek - set seek
 */
static int si470x_set_seek(struct si470x_device *radio,
		unsigned int wrap_around, unsigned int seek_upward)
{
	int retval = 0;
	unsigned long timeout;
	bool timed_out = 0;

	/* start seeking */
	radio->registers[POWERCFG] |= POWERCFG_SEEK;
	if (wrap_around == 1)
		radio->registers[POWERCFG] &= ~POWERCFG_SKMODE;
	else
		radio->registers[POWERCFG] |= POWERCFG_SKMODE;
	if (seek_upward == 1)
		radio->registers[POWERCFG] |= POWERCFG_SEEKUP;
	else
		radio->registers[POWERCFG] &= ~POWERCFG_SEEKUP;
	retval = si470x_set_register(radio, POWERCFG);
	if (retval < 0)
		goto done;

	/* wait till seek operation has completed */
	timeout = jiffies + msecs_to_jiffies(seek_timeout);
	do {
		retval = si470x_get_register(radio, STATUSRSSI);
		if (retval < 0)
			goto stop;
		timed_out = time_after(jiffies, timeout);
	} while (((radio->registers[STATUSRSSI] & STATUSRSSI_STC) == 0) &&
		(!timed_out));
	if ((radio->registers[STATUSRSSI] & STATUSRSSI_STC) == 0)
		printk(KERN_WARNING DRIVER_NAME ": seek does not complete\n");
	if (radio->registers[STATUSRSSI] & STATUSRSSI_SF)
		printk(KERN_WARNING DRIVER_NAME
			": seek failed / band limit reached\n");
	if (timed_out)
		printk(KERN_WARNING DRIVER_NAME
			": seek timed out after %u ms\n", seek_timeout);

stop:
	/* stop seeking */
	radio->registers[POWERCFG] &= ~POWERCFG_SEEK;
	retval = si470x_set_register(radio, POWERCFG);

done:
	/* try again, if timed out */
	if ((retval == 0) && timed_out)
		retval = -EAGAIN;

	return retval;
}

/*
 * si470x_start - switch on radio
 */
static int si470x_start(struct si470x_device *radio)
{
	int retval;

	/* powercfg */
	radio->registers[POWERCFG] =
		POWERCFG_DMUTE | POWERCFG_ENABLE | POWERCFG_RDSM;
	retval = si470x_set_register(radio, POWERCFG);
	if (retval < 0)
		goto done;

	/* sysconfig 1 */
	radio->registers[SYSCONFIG1] = SYSCONFIG1_DE;
	retval = si470x_set_register(radio, SYSCONFIG1);
	if (retval < 0)
		goto done;

	/* sysconfig 2 */
	radio->registers[SYSCONFIG2] =
		(0x3f  << 8) |				/* SEEKTH */
		((band  << 6) & SYSCONFIG2_BAND)  |	/* BAND */
		((space << 4) & SYSCONFIG2_SPACE) |	/* SPACE */
		15;					/* VOLUME (max) */
	retval = si470x_set_register(radio, SYSCONFIG2);
	if (retval < 0)
		goto done;

	/* reset last channel */
	retval = si470x_set_chan(radio,
		radio->registers[CHANNEL] & CHANNEL_CHAN);

done:
	return retval;
}

/*
 * si470x_stop - switch off radio
 */
static int si470x_stop(struct si470x_device *radio)
{
	int retval;

	/* sysconfig 1 */
	radio->registers[SYSCONFIG1] &= ~SYSCONFIG1_RDS;
	retval = si470x_set_register(radio, SYSCONFIG1);
	if (retval < 0)
		goto done;

	/* powercfg */
	radio->registers[POWERCFG] &= ~POWERCFG_DMUTE;
	/* POWERCFG_ENABLE has to automatically go low */
	radio->registers[POWERCFG] |= POWERCFG_ENABLE |	POWERCFG_DISABLE;
	retval = si470x_set_register(radio, POWERCFG);

done:
	return retval;
}

/*
 * si470x_rds_on - switch on rds reception
 */
static int si470x_rds_on(struct si470x_device *radio)
{
	int retval;

	/* sysconfig 1 */
	mutex_lock(&radio->lock);
	radio->registers[SYSCONFIG1] |= SYSCONFIG1_RDS;
	retval = si470x_set_register(radio, SYSCONFIG1);
	if (retval < 0)
		radio->registers[SYSCONFIG1] &= ~SYSCONFIG1_RDS;
	mutex_unlock(&radio->lock);

	return retval;
}

/**************************************************************************
 * RDS Driver Functions
 **************************************************************************/
/*
 * si470x_rds - rds processing function
 */
static void si470x_rds(struct si470x_device *radio)
{
	unsigned char blocknum;
	unsigned short bler; /* rds block errors */
	unsigned short rds;
	unsigned char tmpbuf[3];

	/* get rds blocks */
	if (si470x_get_rds_registers(radio) < 0)
		return;
	if ((radio->registers[STATUSRSSI] & STATUSRSSI_RDSR) == 0) {
		/* No RDS group ready */
		return;
	}
	if ((radio->registers[STATUSRSSI] & STATUSRSSI_RDSS) == 0) {
		/* RDS decoder not synchronized */
		return;
	}

	/* copy all four RDS blocks to internal buffer */
	mutex_lock(&radio->lock);
	for (blocknum = 0; blocknum < 4; blocknum++) {
		switch (blocknum) {
		default:
			bler = (radio->registers[STATUSRSSI] &
					STATUSRSSI_BLERA) >> 9;
			rds = radio->registers[RDSA];
			break;
		case 1:
			bler = (radio->registers[READCHAN] &
					READCHAN_BLERB) >> 14;
			rds = radio->registers[RDSB];
			break;
		case 2:
			bler = (radio->registers[READCHAN] &
					READCHAN_BLERC) >> 12;
			rds = radio->registers[RDSC];
			break;
		case 3:
			bler = (radio->registers[READCHAN] &
					READCHAN_BLERD) >> 10;
			rds = radio->registers[RDSD];
			break;
		};

		/* Fill the V4L2 RDS buffer */
		put_unaligned_le16(rds, &tmpbuf);
		tmpbuf[2] = blocknum;		/* offset name */
		tmpbuf[2] |= blocknum << 3;	/* received offset */
		if (bler > max_rds_errors)
			tmpbuf[2] |= 0x80; /* uncorrectable errors */
		else if (bler > 0)
			tmpbuf[2] |= 0x40; /* corrected error(s) */

		/* copy RDS block to internal buffer */
		memcpy(&radio->buffer[radio->wr_index], &tmpbuf, 3);
		radio->wr_index += 3;

		/* wrap write pointer */
		if (radio->wr_index >= radio->buf_size)
			radio->wr_index = 0;

		/* check for overflow */
		if (radio->wr_index == radio->rd_index) {
			/* increment and wrap read pointer */
			radio->rd_index += 3;
			if (radio->rd_index >= radio->buf_size)
				radio->rd_index = 0;
		}
	}
	mutex_unlock(&radio->lock);

	/* wake up read queue */
	if (radio->wr_index != radio->rd_index)
		wake_up_interruptible(&radio->read_queue);
}

/*
 * si470x_work - rds work function
 */
static void si470x_work(struct work_struct *work)
{
	struct si470x_device *radio = container_of(work, struct si470x_device,
		work.work);

	/* safety checks */
	if (radio->disconnected)
		return;
	if ((radio->registers[SYSCONFIG1] & SYSCONFIG1_RDS) == 0)
		return;

	si470x_rds(radio);
	schedule_delayed_work(&radio->work, msecs_to_jiffies(rds_poll_time));
}

/**************************************************************************
 * File Operations Interface
 **************************************************************************/
/*
 * si470x_fops_read - read RDS data
 */
static ssize_t si470x_fops_read(struct file *file, char __user *buf,
		size_t count, loff_t *ppos)
{
	struct si470x_device *radio = video_drvdata(file);
	int retval = 0;
	unsigned int block_count = 0;

	/* switch on rds reception */
	if ((radio->registers[SYSCONFIG1] & SYSCONFIG1_RDS) == 0) {
		si470x_rds_on(radio);
		schedule_delayed_work(&radio->work,
			msecs_to_jiffies(rds_poll_time));
	}

	/* block if no new data available */
	while (radio->wr_index == radio->rd_index) {
		if (file->f_flags & O_NONBLOCK) {
			retval = -EWOULDBLOCK;
			goto done;
		}
		if (wait_event_interruptible(radio->read_queue,
			radio->wr_index != radio->rd_index) < 0) {
			retval = -EINTR;
			goto done;
		}
	}

	/* calculate block count from byte count */
	count /= 3;

	/* copy RDS block out of internal buffer and to user buffer */
	mutex_lock(&radio->lock);
	while (block_count < count) {
		if (radio->rd_index == radio->wr_index)
			break;

		/* always transfer rds complete blocks */
		if (copy_to_user(buf, &radio->buffer[radio->rd_index], 3))
			/* retval = -EFAULT; */
			break;

		/* increment and wrap read pointer */
		radio->rd_index += 3;
		if (radio->rd_index >= radio->buf_size)
			radio->rd_index = 0;

		/* increment counters */
		block_count++;
		buf += 3;
		retval += 3;
	}
	mutex_unlock(&radio->lock);

done:
	return retval;
}

/*
 * si470x_fops_poll - poll RDS data
 */
static unsigned int si470x_fops_poll(struct file *file,
		struct poll_table_struct *pts)
{
	struct si470x_device *radio = video_drvdata(file);
	int retval = 0;

	/* switch on rds reception */
	if ((radio->registers[SYSCONFIG1] & SYSCONFIG1_RDS) == 0) {
		si470x_rds_on(radio);
		schedule_delayed_work(&radio->work,
			msecs_to_jiffies(rds_poll_time));
	}

	poll_wait(file, &radio->read_queue, pts);

	if (radio->rd_index != radio->wr_index)
		retval = POLLIN | POLLRDNORM;

	return retval;
}

/*
 * si470x_fops_open - file open
 */
static int si470x_fops_open(struct inode *inode, struct file *file)
{
	struct si470x_device *radio = video_drvdata(file);
	int retval = 0;

	lock_kernel();
	radio->users++;
/*
	retval = usb_autopm_get_interface(radio->intf);
	if (retval < 0) {
		radio->users--;
		retval = -EIO;
		goto done;
	}
*/
	if (radio->users == 1) {
		retval = si470x_start(radio);
//		if (retval < 0)
//			usb_autopm_put_interface(radio->intf);
	}

//done:
	unlock_kernel();
	return retval;
}

/*
 * si470x_fops_release - file release
 */
static int si470x_fops_release(struct inode *inode, struct file *file)
{
	struct si470x_device *radio = video_drvdata(file);
	int retval = 0;

	/* safety check */
	if (!radio) {
		retval = -ENODEV;
		goto done;
	}

	mutex_lock(&radio->disconnect_lock);
	radio->users--;
	if (radio->users == 0) {
		if (radio->disconnected) {
			video_unregister_device(radio->videodev);
			kfree(radio->buffer);
			kfree(radio);
			goto unlock;
		}

		/* stop rds reception */
		cancel_delayed_work_sync(&radio->work);

		/* cancel read processes */
		wake_up_interruptible(&radio->read_queue);

		retval = si470x_stop(radio);
//		usb_autopm_put_interface(radio->intf);
	}

unlock:
	mutex_unlock(&radio->disconnect_lock);

done:
	return retval;
}

/*
 * si470x_fops - file operations interface
 */
static const struct file_operations si470x_fops = {
	.owner		= THIS_MODULE,
	.llseek		= no_llseek,
	.read		= si470x_fops_read,
	.poll		= si470x_fops_poll,
	.ioctl		= video_ioctl2,
#ifdef CONFIG_COMPAT
	.compat_ioctl	= v4l_compat_ioctl32,
#endif
	.open		= si470x_fops_open,
	.release	= si470x_fops_release,
};

/**************************************************************************
 * Video4Linux Interface
 **************************************************************************/

/*
 * si470x_v4l2_queryctrl - query control
 */
static struct v4l2_queryctrl si470x_v4l2_queryctrl[] = {
	{
		.id		= V4L2_CID_AUDIO_VOLUME,
		.type		= V4L2_CTRL_TYPE_INTEGER,
		.name		= "Volume",
		.minimum	= 0,
		.maximum	= 15,
		.step		= 1,
		.default_value	= 15,
	},
	{
		.id		= V4L2_CID_AUDIO_MUTE,
		.type		= V4L2_CTRL_TYPE_BOOLEAN,
		.name		= "Mute",
		.minimum	= 0,
		.maximum	= 1,
		.step		= 1,
		.default_value	= 1,
	},
};


/*
 * si470x_vidioc_querycap - query device capabilities
 */
static int si470x_vidioc_querycap(struct file *file, void *priv,
		struct v4l2_capability *capability)
{
	strlcpy(capability->driver, DRIVER_NAME, sizeof(capability->driver));
	strlcpy(capability->card, DRIVER_CARD, sizeof(capability->card));
	sprintf(capability->bus_info, "I2C");
	capability->version = DRIVER_KERNEL_VERSION;
	capability->capabilities = V4L2_CAP_HW_FREQ_SEEK |
		V4L2_CAP_TUNER | V4L2_CAP_RADIO;

	return 0;
}


/*
 * si470x_vidioc_queryctrl - enumerate control items
 */
static int si470x_vidioc_queryctrl(struct file *file, void *priv,
		struct v4l2_queryctrl *qc)
{
	unsigned char i = 0;
	int retval = -EINVAL;

	/* abort if qc->id is below V4L2_CID_BASE */
	if (qc->id < V4L2_CID_BASE)
		goto done;

	/* search video control */
	for (i = 0; i < ARRAY_SIZE(si470x_v4l2_queryctrl); i++) {
		if (qc->id == si470x_v4l2_queryctrl[i].id) {
			memcpy(qc, &(si470x_v4l2_queryctrl[i]), sizeof(*qc));
			retval = 0; /* found */
			break;
		}
	}

	/* disable unsupported base controls */
	/* to satisfy kradio and such apps */
	if ((retval == -EINVAL) && (qc->id < V4L2_CID_LASTP1)) {
		qc->flags = V4L2_CTRL_FLAG_DISABLED;
		retval = 0;
	}

done:
	if (retval < 0)
		printk(KERN_WARNING DRIVER_NAME
			": query controls failed with %d\n", retval);
	return retval;
}


/*
 * si470x_vidioc_g_ctrl - get the value of a control
 */
static int si470x_vidioc_g_ctrl(struct file *file, void *priv,
		struct v4l2_control *ctrl)
{
	struct si470x_device *radio = video_drvdata(file);
	int retval = 0;

	/* safety checks */
	if (radio->disconnected) {
		retval = -EIO;
		goto done;
	}

	switch (ctrl->id) {
	case V4L2_CID_AUDIO_VOLUME:
		ctrl->value = radio->registers[SYSCONFIG2] &
				SYSCONFIG2_VOLUME;
		break;
	case V4L2_CID_AUDIO_MUTE:
		ctrl->value = ((radio->registers[POWERCFG] &
				POWERCFG_DMUTE) == 0) ? 1 : 0;
		break;
	default:
		retval = -EINVAL;
	}

done:
	if (retval < 0)
		printk(KERN_WARNING DRIVER_NAME
			": get control failed with %d\n", retval);
	return retval;
}

/*
 * si470x_vidioc_s_ctrl - set the value of a control
 */
static int si470x_vidioc_s_ctrl(struct file *file, void *priv,
		struct v4l2_control *ctrl)
{
	struct si470x_device *radio = video_drvdata(file);
	int retval = 0;

	/* safety checks */
	if (radio->disconnected) {
		retval = -EIO;
		goto done;
	}

	switch (ctrl->id) {
	case V4L2_CID_AUDIO_VOLUME:
		radio->registers[SYSCONFIG2] &= ~SYSCONFIG2_VOLUME;
		radio->registers[SYSCONFIG2] |= ctrl->value;
		retval = si470x_set_register(radio, SYSCONFIG2);
		break;
	case V4L2_CID_AUDIO_MUTE:
		if (ctrl->value == 1)
			radio->registers[POWERCFG] &= ~POWERCFG_DMUTE;
		else
			radio->registers[POWERCFG] |= POWERCFG_DMUTE;
		retval = si470x_set_register(radio, POWERCFG);
		break;
	default:
		retval = -EINVAL;
	}

done:
	if (retval < 0)
		printk(KERN_WARNING DRIVER_NAME
			": set control failed with %d\n", retval);
	return retval;
}

/*
 * si470x_vidioc_g_audio - get audio attributes
 */
static int si470x_vidioc_g_audio(struct file *file, void *priv,
		struct v4l2_audio *audio)
{
	/* driver constants */
	audio->index = 0;
	strcpy(audio->name, "Radio");
	audio->capability = V4L2_AUDCAP_STEREO;
	audio->mode = 0;

	return 0;
}

/*
 * si470x_vidioc_g_tuner - get tuner attributes
 */
static int si470x_vidioc_g_tuner(struct file *file, void *priv,
		struct v4l2_tuner *tuner)
{
	struct si470x_device *radio = video_drvdata(file);
	int retval = 0;

	/* safety checks */
	if (radio->disconnected) {
		retval = -EIO;
		goto done;
	}
	if (tuner->index != 0) {
		retval = -EINVAL;
		goto done;
	}

	retval = si470x_get_register(radio, STATUSRSSI);
	if (retval < 0)
		goto done;

	/* driver constants */
	strcpy(tuner->name, "FM");
	tuner->type = V4L2_TUNER_RADIO;
	tuner->capability = V4L2_TUNER_CAP_LOW | V4L2_TUNER_CAP_STEREO;

	/* range limits */
	switch ((radio->registers[SYSCONFIG2] & SYSCONFIG2_BAND) >> 6) {
	/* 0: 87.5 - 108 MHz (USA, Europe, default) */
	default:
		tuner->rangelow  =  87.5 * FREQ_MUL;
		tuner->rangehigh = 108   * FREQ_MUL;
		break;
	/* 1: 76   - 108 MHz (Japan wide band) */
	case 1 :
		tuner->rangelow  =  76   * FREQ_MUL;
		tuner->rangehigh = 108   * FREQ_MUL;
		break;
	/* 2: 76   -  90 MHz (Japan) */
	case 2 :
		tuner->rangelow  =  76   * FREQ_MUL;
		tuner->rangehigh =  90   * FREQ_MUL;
		break;
	};

	/* stereo indicator == stereo (instead of mono) */
	if ((radio->registers[STATUSRSSI] & STATUSRSSI_ST) == 1)
		tuner->rxsubchans = V4L2_TUNER_SUB_MONO | V4L2_TUNER_SUB_STEREO;
	else
		tuner->rxsubchans = V4L2_TUNER_SUB_MONO;

	/* mono/stereo selector */
	if ((radio->registers[POWERCFG] & POWERCFG_MONO) == 1)
		tuner->audmode = V4L2_TUNER_MODE_MONO;
	else
		tuner->audmode = V4L2_TUNER_MODE_STEREO;

	/* min is worst, max is best; signal:0..0xffff; rssi: 0..0xff */
	tuner->signal = (radio->registers[STATUSRSSI] & STATUSRSSI_RSSI)
				* 0x0101;

	/* automatic frequency control: -1: freq to low, 1 freq to high */
	/* AFCRL does only indicate that freq. differs, not if too low/high */
	tuner->afc = (radio->registers[STATUSRSSI] & STATUSRSSI_AFCRL) ? 1 : 0;

done:
	if (retval < 0)
		printk(KERN_WARNING DRIVER_NAME
			": get tuner failed with %d\n", retval);
	return retval;
}

/*
 * si470x_vidioc_s_tuner - set tuner attributes
 */
static int si470x_vidioc_s_tuner(struct file *file, void *priv,
		struct v4l2_tuner *tuner)
{
	struct si470x_device *radio = video_drvdata(file);
	int retval = -EINVAL;

	/* safety checks */
	if (radio->disconnected) {
		retval = -EIO;
		goto done;
	}
	if (tuner->index != 0)
		goto done;

	/* mono/stereo selector */
	switch (tuner->audmode) {
	case V4L2_TUNER_MODE_MONO:
		radio->registers[POWERCFG] |= POWERCFG_MONO;  /* force mono */
		break;
	case V4L2_TUNER_MODE_STEREO:
		radio->registers[POWERCFG] &= ~POWERCFG_MONO; /* try stereo */
		break;
	default:
		goto done;
	}

	retval = si470x_set_register(radio, POWERCFG);

done:
	if (retval < 0)
		printk(KERN_WARNING DRIVER_NAME
			": set tuner failed with %d\n", retval);
	return retval;
}

/*
 * si470x_vidioc_g_frequency - get tuner or modulator radio frequency
 */
static int si470x_vidioc_g_frequency(struct file *file, void *priv,
		struct v4l2_frequency *freq)
{
	struct si470x_device *radio = video_drvdata(file);
	int retval = 0;

	/* safety checks */
	if (radio->disconnected) {
		retval = -EIO;
		goto done;
	}
	if (freq->tuner != 0) {
		retval = -EINVAL;
		goto done;
	}

	freq->type = V4L2_TUNER_RADIO;
	retval = si470x_get_freq(radio, &freq->frequency);

done:
	if (retval < 0)
		printk(KERN_WARNING DRIVER_NAME
			": get frequency failed with %d\n", retval);
	return retval;
}

/*
 * si470x_vidioc_s_frequency - set tuner or modulator radio frequency
 */
static int si470x_vidioc_s_frequency(struct file *file, void *priv,
		struct v4l2_frequency *freq)
{
	struct si470x_device *radio = video_drvdata(file);
	int retval = 0;

	/* safety checks */
	if (radio->disconnected) {
		retval = -EIO;
		goto done;
	}
	if (freq->tuner != 0) {
		retval = -EINVAL;
		goto done;
	}

	retval = si470x_set_freq(radio, freq->frequency);

done:
	if (retval < 0)
		printk(KERN_WARNING DRIVER_NAME
			": set frequency failed with %d\n", retval);
	return retval;
}

/*
 * si470x_vidioc_s_hw_freq_seek - set hardware frequency seek
 */
static int si470x_vidioc_s_hw_freq_seek(struct file *file, void *priv,
		struct v4l2_hw_freq_seek *seek)
{
	struct si470x_device *radio = video_drvdata(file);
	int retval = 0;

	/* safety checks */
	if (radio->disconnected) {
		retval = -EIO;
		goto done;
	}
	if (seek->tuner != 0) {
		retval = -EINVAL;
		goto done;
	}

	retval = si470x_set_seek(radio, seek->wrap_around, seek->seek_upward);

done:
	if (retval < 0)
		printk(KERN_WARNING DRIVER_NAME
			": set hardware frequency seek failed with %d\n",
			retval);
	return retval;
}

/*
 * si470x_ioctl_ops - video device ioctl operations
 */
static const struct v4l2_ioctl_ops si470x_ioctl_ops = {
	.vidioc_querycap	= si470x_vidioc_querycap,
	.vidioc_queryctrl	= si470x_vidioc_queryctrl,
	.vidioc_g_ctrl		= si470x_vidioc_g_ctrl,
	.vidioc_s_ctrl		= si470x_vidioc_s_ctrl,
	.vidioc_g_audio		= si470x_vidioc_g_audio,
	.vidioc_g_tuner		= si470x_vidioc_g_tuner,
	.vidioc_s_tuner		= si470x_vidioc_s_tuner,
	.vidioc_g_frequency	= si470x_vidioc_g_frequency,
	.vidioc_s_frequency	= si470x_vidioc_s_frequency,
	.vidioc_s_hw_freq_seek	= si470x_vidioc_s_hw_freq_seek,
};


/*
 * si470x_viddev_template - video device interface
 */
static struct video_device si470x_viddev_template = {
	.fops			= &si470x_fops,
	.name			= DRIVER_NAME,
	.release		= video_device_release,
	.ioctl_ops		= &si470x_ioctl_ops,
};

/**************************************************************************
 * Module Interface
 **************************************************************************/
/*
 * si470x_module_init - module init
 */
static int __init si470x_module_init(void)
{
	int retval = 0;

	printk(KERN_INFO DRIVER_DESC ", Version " DRIVER_VERSION "\n");

	/* private data allocation and initialization */
	radio = kzalloc(sizeof(struct si470x_device), GFP_KERNEL);
	if (!radio) {
		retval = -ENOMEM;
		goto err_initial;
	}
	radio->users = 0;
	radio->disconnected = 0;
	mutex_init(&radio->disconnect_lock);
	mutex_init(&radio->lock);

	/* video device allocation and initialization */
	radio->videodev = video_device_alloc();
	if (!radio->videodev) {
		retval = -ENOMEM;
		goto err_radio;
	}
	memcpy(radio->videodev, &si470x_viddev_template,
			sizeof(si470x_viddev_template));
	video_set_drvdata(radio->videodev, radio);
	si470x_powerup(radio, 1);
	/* show some infos about the specific device */
	if (si470x_get_all_registers(radio) < 0) {
		retval = -EIO;
		goto err_all;
	}
	printk(KERN_INFO DRIVER_NAME ": DeviceID=0x%4.4hx ChipID=0x%4.4hx\n",
			radio->registers[DEVICEID], radio->registers[CHIPID]);

	/* check if firmware is current */
	if ((radio->registers[CHIPID] & CHIPID_FIRMWARE)
			< RADIO_SW_VERSION_CURRENT) {
		printk(KERN_WARNING DRIVER_NAME
			": This driver is known to work with "
			"firmware version %hu,\n", RADIO_SW_VERSION_CURRENT);
		printk(KERN_WARNING DRIVER_NAME
			": but the device has firmware version %hu.\n",
			radio->registers[CHIPID] & CHIPID_FIRMWARE);
		printk(KERN_WARNING DRIVER_NAME
			": If you have some trouble using this driver,\n");
		printk(KERN_WARNING DRIVER_NAME
			": please report to V4L ML at "
			"video4linux-list@redhat.com\n");
	}

	/* set initial frequency */
	si470x_set_freq(radio, 87.5 * FREQ_MUL); /* available in all regions */

	/* rds buffer allocation */
	radio->buf_size = rds_buf * 3;
	radio->buffer = kmalloc(radio->buf_size, GFP_KERNEL);
	if (!radio->buffer) {
		retval = -EIO;
		goto err_all;
	}

	/* rds buffer configuration */
	radio->wr_index = 0;
	radio->rd_index = 0;
	init_waitqueue_head(&radio->read_queue);

	/* prepare rds work function */
	INIT_DELAYED_WORK(&radio->work, si470x_work);

	/* register video device */
	retval = video_register_device(radio->videodev, VFL_TYPE_RADIO, radio_nr);
	if (retval) {
		printk(KERN_WARNING DRIVER_NAME
				": Could not register video device\n");
		goto err_all;
	}
//	usb_set_intfdata(intf, radio);

	return 0;
err_all:
	video_device_release(radio->videodev);
	kfree(radio->buffer);
err_radio:
	kfree(radio);
err_initial:
	return retval;
}

/*
 * si470x_module_exit - module exit
 */
static void __exit si470x_module_exit(void)
{
//	struct si470x_device *radio = usb_get_intfdata(intf);
	mutex_lock(&radio->disconnect_lock);
	radio->disconnected = 1;
	cancel_delayed_work_sync(&radio->work);
//	usb_set_intfdata(intf, NULL);
	if (radio->users == 0) {
		video_unregister_device(radio->videodev);
		kfree(radio->buffer);
		kfree(radio);
	}
	mutex_unlock(&radio->disconnect_lock);
}

module_init(si470x_module_init);
module_exit(si470x_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_VERSION(DRIVER_VERSION);
