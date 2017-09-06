/*
 * mbc.h  -- Driver for NXP PCF50633 Main Battery Charger
 *
 * (C) 2006-2008 by Openmoko, Inc.
 * All rights reserved.
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */

#ifndef __LINUX_MFD_PCF50633_MBC_H
#define __LINUX_MFD_PCF50633_MBC_H

#include <linux/mfd/pcf50633/core.h>
#include <linux/platform_device.h>

#define PCF50633_REG_MBCC1	0x43
#define PCF50633_REG_MBCC2	0x44
#define PCF50633_REG_MBCC3	0x45
#define PCF50633_REG_MBCC4	0x46
#define PCF50633_REG_MBCC5	0x47
#define PCF50633_REG_MBCC6	0x48
#define PCF50633_REG_MBCC7	0x49
#define PCF50633_REG_MBCC8	0x4a
#define PCF50633_REG_MBCS1	0x4b
#define PCF50633_REG_MBCS2	0x4c
#define PCF50633_REG_MBCS3	0x4d

enum pcf50633_reg_mbcc1 {
	PCF50633_MBCC1_CHGENA		= 0x01,	/* Charger enable */
	PCF50633_MBCC1_AUTOSTOP		= 0x02,
	PCF50633_MBCC1_AUTORES		= 0x04, /* automatic resume */
	PCF50633_MBCC1_RESUME		= 0x08, /* explicit resume cmd */
	PCF50633_MBCC1_RESTART		= 0x10, /* restart charging */
	PCF50633_MBCC1_PREWDTIME_60M	= 0x20,	/* max. precharging time */
	PCF50633_MBCC1_WDTIME_1H	= 0x00,
	PCF50633_MBCC1_WDTIME_2H	= 0x40,
	PCF50633_MBCC1_WDTIME_4H	= 0x80,
	PCF50633_MBCC1_WDTIME_6H	= 0xc0,
};
#define PCF50633_MBCC1_WDTIME_MASK	  0xc0

enum pcf50633_reg_mbcc2 {
	PCF50633_MBCC2_VBATCOND_2V7	= 0x00,
	PCF50633_MBCC2_VBATCOND_2V85	= 0x01,
	PCF50633_MBCC2_VBATCOND_3V0	= 0x02,
	PCF50633_MBCC2_VBATCOND_3V15	= 0x03,
	PCF50633_MBCC2_VMAX_4V		= 0x00,
	PCF50633_MBCC2_VMAX_4V20	= 0x28,
	PCF50633_MBCC2_VRESDEBTIME_64S	= 0x80,	/* debounce time (32/64sec) */
};

enum pcf50633_reg_mbcc7 {
	PCF50633_MBCC7_USB_100mA	= 0x00,
	PCF50633_MBCC7_USB_500mA	= 0x01,
	PCF50633_MBCC7_USB_1000mA	= 0x02,
	PCF50633_MBCC7_USB_SUSPEND	= 0x03,
	PCF50633_MBCC7_BATTEMP_EN	= 0x04,
	PCF50633_MBCC7_BATSYSIMAX_1A6	= 0x00,
	PCF50633_MBCC7_BATSYSIMAX_1A8	= 0x40,
	PCF50633_MBCC7_BATSYSIMAX_2A0	= 0x80,
	PCF50633_MBCC7_BATSYSIMAX_2A2	= 0xc0,
};
#define PCF50633_MBCC7_USB_MASK 0x03

enum pcf50633_reg_mbcc8 {
	PCF50633_MBCC8_USBENASUS	= 0x10,
};

enum pcf50633_reg_mbcs1 {
	PCF50633_MBCS1_USBPRES		= 0x01,
	PCF50633_MBCS1_USBOK		= 0x02,
	PCF50633_MBCS1_ADAPTPRES	= 0x04,
	PCF50633_MBCS1_ADAPTOK		= 0x08,
	PCF50633_MBCS1_TBAT_OK		= 0x00,
	PCF50633_MBCS1_TBAT_ABOVE	= 0x10,
	PCF50633_MBCS1_TBAT_BELOW	= 0x20,
	PCF50633_MBCS1_TBAT_UNDEF	= 0x30,
	PCF50633_MBCS1_PREWDTEXP	= 0x40,
	PCF50633_MBCS1_WDTEXP		= 0x80,
};

enum pcf50633_reg_mbcs2_mbcmod {
	PCF50633_MBCS2_MBC_PLAY		= 0x00,
	PCF50633_MBCS2_MBC_USB_PRE	= 0x01,
	PCF50633_MBCS2_MBC_USB_PRE_WAIT	= 0x02,
	PCF50633_MBCS2_MBC_USB_FAST	= 0x03,
	PCF50633_MBCS2_MBC_USB_FAST_WAIT = 0x04,
	PCF50633_MBCS2_MBC_USB_SUSPEND	= 0x05,
	PCF50633_MBCS2_MBC_ADP_PRE	= 0x06,
	PCF50633_MBCS2_MBC_ADP_PRE_WAIT	= 0x07,
	PCF50633_MBCS2_MBC_ADP_FAST	= 0x08,
	PCF50633_MBCS2_MBC_ADP_FAST_WAIT = 0x09,
	PCF50633_MBCS2_MBC_BAT_FULL	= 0x0a,
	PCF50633_MBCS2_MBC_HALT		= 0x0b,
};
#define PCF50633_MBCS2_MBC_MASK		0x0f
enum pcf50633_reg_mbcs2_chgstat {
	PCF50633_MBCS2_CHGS_NONE	= 0x00,
	PCF50633_MBCS2_CHGS_ADAPTER	= 0x10,
	PCF50633_MBCS2_CHGS_USB		= 0x20,
	PCF50633_MBCS2_CHGS_BOTH	= 0x30,
};
#define PCF50633_MBCS2_RESSTAT_AUTO	0x40

enum pcf50633_reg_mbcs3 {
	PCF50633_MBCS3_USBLIM_PLAY	= 0x01,
	PCF50633_MBCS3_USBLIM_CGH	= 0x02,
	PCF50633_MBCS3_TLIM_PLAY	= 0x04,
	PCF50633_MBCS3_TLIM_CHG		= 0x08,
	PCF50633_MBCS3_ILIM		= 0x10,	/* 1: Ibat > Icutoff */
	PCF50633_MBCS3_VLIM		= 0x20,	/* 1: Vbat == Vmax */
	PCF50633_MBCS3_VBATSTAT		= 0x40,	/* 1: Vbat > Vbatcond */
	PCF50633_MBCS3_VRES		= 0x80, /* 1: Vbat > Vth(RES) */
};

#define PCF50633_MBCC2_VBATCOND_MASK	  0x03
#define PCF50633_MBCC2_VMAX_MASK	  0x3c

/* Charger status */
#define PCF50633_MBC_USB_ONLINE		0x01
#define PCF50633_MBC_USB_ACTIVE		0x02
#define PCF50633_MBC_ADAPTER_ONLINE	0x04
#define PCF50633_MBC_ADAPTER_ACTIVE	0x08

int pcf50633_mbc_usb_curlim_set(struct pcf50633 *pcf, int ma);

int pcf50633_mbc_get_status(struct pcf50633 *);
int pcf50633_mbc_get_usb_online_status(struct pcf50633 *);

#endif
