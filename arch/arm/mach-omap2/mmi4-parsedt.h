/*
 * Copyright (C) 2013 Ultratronik GmbH
 * Author: Andreas Naumann <anaumann@ultratronik.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __ARCH_ARM_MMI4_PARSEDT_H__
#define __ARCH_ARM_MMI4_PARSEDT_H__

//#include <linux/i2c/tsc2007.h>
//#include <linux/input/qt2160-slider.h>
#include <linux/interrupt.h>
#include <linux/input.h>

struct i2c_bus_hw_info  {
	int id;
	int freq;
};

struct i2c_bus_bb_info  {
	struct platform_device *gpio_i2c;
	int id;
};

struct display_info {
	u32 gpio_dp;
	u32 gpio_bp;
	u32 gpio_br;
};

struct mmi4_eth_data{
	int irq_gpio;
	int fifo_gpio;
	int cs;
};

struct mmi4_can_data{
	int irq_gpio;
	int cs;
};

struct omap2_hsmmc_info* get_omap2_hsmmc_info(struct omap2_hsmmc_info*);
struct omap_nand_platform_data* get_nand_pdata( struct omap_nand_platform_data* nand );
int fill_musb_board_data( struct omap_musb_board_data
		*board_data);
int fill_usbhs_bdata(struct usbhs_omap_board_data*	board_data);
int fill_eth_platform_device(struct platform_device *, struct mmi4_eth_data *eth_data);
int fill_i2c_board_info(struct i2c_board_info *bus_info);
struct i2c_board_info * get_i2c_devices(struct i2c_board_info *i2c_boardinfo, int bus_id, int *devnum);
struct i2c_bus_bb_info * fill_i2c_bus_bb_info(struct i2c_bus_bb_info *bus_info, int bus_id);
struct i2c_bus_hw_info * fill_i2c_bus_hw_info(struct i2c_bus_hw_info *bus_info, int bus_id);
int get_max_i2c_bus_id(int *ret_max);
struct display_info *get_display_info( struct display_info* display_info);
struct platform_device* get_pwm_pdev( struct platform_device *pwm );
struct platform_device *get_audio_dev(struct platform_device* audio);
struct platform_device* get_can_dev( struct platform_device* can, struct mmi4_can_data* can_data);
int fill_gpioled_dev( struct platform_device* gpio_led_info );
#endif
