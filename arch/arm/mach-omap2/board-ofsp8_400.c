/*
 * linux/arch/arm/mach-omap2/board-ofsp8_400.c
 *
 * Copyright (C) 2011 Ultratronik
 * Author: Christian Jaeger <cjaeger@ultratronik.de>
 *
 * Modified from mach-omap2/board-ofsp8.c
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/gpio.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/i2c.h>
//#include <linux/i2c-gpio.h>
#include <linux/platform_data/i2c-gpio.h>
#include <linux/platform_data/i2c-omap.h>
#include <linux/platform_data/tsc2007.h>

//#include <linux/i2c/twl.h> // needed?
#include <linux/platform_data/omap-twl4030.h>

#include <linux/mmc/card.h>
#include <linux/platform_device.h>
#include <linux/regulator/machine.h>
#include <linux/usb/otg.h>
#include <linux/can/platform/sja1000.h>
#include <linux/pwm_backlight.h>
//#include <linux/input/qt2160-slider.h>

#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>

#include <net/ax88796.h>

#include <mach/hardware.h>
#include <asm/switch_to.h>
#include <asm/mach-types.h>
#include <asm/mach/arch.h>

#include <asm/mach/map.h>
#include <asm/mach/flash.h>

//#include <plat/board.h> needd?
#include "common.h"
//#include <plat/prcm.h> needed?
//#include <video/omapdss.h>
//#include <video/omap-panel-generic-dpi.h>
//#include <plat/gpmc.h>
#include <linux/platform_data/gpmc-omap.h>
//#include <plat/nand.h>
#include <linux/platform_data/mtd-nand-omap2.h>
//#include <plat/mmc.h>
#include <linux/platform_data/mmc-omap.h>
//#include <plat/usb.h>
//#include <linux/platform_data/usb-omap.h>
#include <linux/fb.h>

#include "mux.h"
#include "board-ofsp8_400.h"
#include "hsmmc.h"
#include "common-board-devices.h"

#include <linux/of.h>
#include <linux/of_platform.h>

/*HW Config device tree structure*/

static int ret_splash = 0;

static const char *omap3_boards_compat[] __initdata = {
	"ti,omap3",
	NULL,
};

int ofsp8_400_get_tspendown(void)
{
	return !gpio_get_value_cansleep(g_tsc2007_gpio);
};

struct display_info ofsp8_400_display_info = {
	.gpio_dp = VIDEO_GPIO_DISP_POW,
	.gpio_bp = VIDEO_GPIO_BKL_POW,
	.gpio_br = VIDEO_GPIO_BKL_BR,
};

struct i2c_bus_hw_info default_i2c_hw_bus = {
	.id = 2,
	.freq = 100,
};

/*struct tsc2007_platform_data ofsp8_400_tscdata = {*/
/*	.model                  = 2007,*/
/*	.x_plate_ohms           = 50,*/
/*	.get_pendown_state      = ofsp8_400_get_tspendown,*/
/*	.irq_flags              = IRQF_TRIGGER_FALLING,*/
/*};*/

static struct i2c_board_info __initdata ofsp8_400_i2c2_boardinfo[] = {
	{
		I2C_BOARD_INFO("rtc8564", 0x51),
	},
	{
		I2C_BOARD_INFO("24c32", 0x50),
	},
/*	{*/
/*		I2C_BOARD_INFO("tsc2007", 0x48),*/
/*		.platform_data = &ofsp8_400_tscdata,*/
/*	},*/
};

/* NAND flash */

/*static struct omap_nand_platform_data ofsp8_400_nand_data = {*/
/*	.cs		= NAND_CS,*/
/*	.devsize	= NAND_BUSWIDTH_16, // 16-bit */
/*	.ecc_opt	= OMAP_ECC_BCH8_CODE_HW,*/
/*};*/

/*void __init ofsp8_400_flash_init(void)*/
/*{*/
/*	if( get_nand_pdata(&ofsp8_400_nand_data) == NULL ) */
/*		return; */
/*	if (gpmc_nand_init(&ofsp8_400_nand_data) < 0)*/
/*		pr_err(NAND_DRIVER_NAME ": Unable to register NAND device\n");*/
/*}*/

/* Ethernet (Asix AX88796 10/100 ethernet controller) */

static struct resource ofsp8_400_eth_resources[] = {
	{
		.flags  = IORESOURCE_MEM,
	}, {
		.flags  = IORESOURCE_MEM,
	}, {
		.flags  = IORESOURCE_IRQ | IORESOURCE_IRQ_LOWLEVEL,
	}
};

static u8 default_mac_addr [6] = {0x00, 0x1b, 0xe8, 0x00, 0x00, 0x00,};

static struct ax_plat_data ofsp8_400_eth_platdata = {
	.flags          = AXFLG_MAC_FROMPLATFORM,
	.wordlength     = 2,
	.mac_addr       = default_mac_addr,
};

static struct platform_device ofsp8_400_eth_device = {
	.name		= ETH_DRIVER_NAME,
	.id		= -1,
	.resource	= ofsp8_400_eth_resources,
	.num_resources	= ARRAY_SIZE(ofsp8_400_eth_resources),
	.dev		= {
		.platform_data = &ofsp8_400_eth_platdata,
	},
};

static int __init setup_ethmac(char *str)
{
	char *mac_part;
	int mac_len = 18;
	char tmp_ethaddr[mac_len];
	char *cur_pos;
	int i;

	if(str[0] == '\0')
		return 1;

	cur_pos = &tmp_ethaddr[0];
	strncpy(tmp_ethaddr, str, mac_len);
	for(i=0; i < mac_len / 3; i++) {
		mac_part = strsep(&cur_pos, ":");
		if( mac_part != NULL)
			ofsp8_400_eth_platdata.mac_addr[i] = (u8) simple_strtol(mac_part, NULL, 16);
		else
			return 1;
	}

	return 0;
}

static int setup_splash(char *str)
{
/*	ret_splash = simple_strtol(str, NULL, 10);*/
/*	pr_notice("mmi4: Splashscreen is enabled in u-boot");*/
	return 0;
}

static struct platform_device ofsp8_400_audio_device = {
		.name   = "mmi4-soc-audio",
		.id     = -1,
};

void __init ofsp8_400_audio_init(void)
{
/*	int err;

	if( get_audio_dev(&ofsp8_400_audio_device) == NULL )
		return;
		
	err = platform_device_register(&ofsp8_400_audio_device);
	if (err) {
		pr_err("%s: Failed to add platform device\n", ofsp8_400_audio_device.name);
	}
	*/
}

struct mmi4_eth_data eth_data = {
	.irq_gpio = ETH_GPIO_IRQ,
	.fifo_gpio = ETH_GPIO_FIFO,
	.cs = ETH_CS,
};

void __init ofsp8_400_eth_init(void)
{
/*	int err;*/
/*	unsigned long cs_mem_base;*/
/*	if(fill_eth_platform_device(&ofsp8_400_eth_device, &eth_data))*/
/*		return;*/

/*	 ax88796 driver does not support clk-framework, so clk_get
	  and clk_enable should be called here. But as this is
	  called before clk-initialization this is not possible.
	  So we relay on other hardware to keep l3_clk alive.
	 */

/*	err = gpio_request(eth_data.irq_gpio, ETH_DRIVER_NAME " irq");*/
/*	if (err) {*/
/*		pr_err("%s: Failed to request GPIO%d for IRQ\n", ofsp8_400_eth_device.name,*/
/*				eth_data.irq_gpio);*/
/*		goto err_req_gpio_irq;*/
/*	}*/
/*	gpio_direction_input(eth_data.irq_gpio);*/
/*        if ( eth_data.fifo_gpio > 0 )*/
/*	        err = gpio_request(eth_data.fifo_gpio, ETH_DRIVER_NAME " fifo");*/
/*        else*/
/*                pr_notice ("Ethernet FIFO GPIO is not defined\n");*/
/*	if (err) {*/
/*		pr_err("%s: Failed to request GPIO%d for "*/
/*				"FIFO-select\n", ofsp8_400_eth_device.name,*/
/*				eth_data.fifo_gpio);*/
/*		goto err_req_gpio_fifo;*/
/*	}*/
/*        if ( eth_data.fifo_gpio > 0 )*/
/*	        gpio_direction_output(eth_data.fifo_gpio, 0);*/

/*	ofsp8_400_eth_device.resource[2].start = gpio_to_irq(eth_data.irq_gpio);*/
/*	ofsp8_400_eth_device.resource[2].end =	gpio_to_irq(eth_data.irq_gpio);*/

/*	gpmc_cs_request(eth_data.cs, SZ_16M, &cs_mem_base);*/

/*	ofsp8_400_eth_device.resource[0].start = cs_mem_base;*/
/*	ofsp8_400_eth_device.resource[0].end = cs_mem_base + (0x18 << 1) - 1;*/
/*	ofsp8_400_eth_device.resource[1].start = cs_mem_base + (0x1f << 1);*/
/*	ofsp8_400_eth_device.resource[1].end = cs_mem_base + (0x20 << 1) - 1;*/

/*	err = platform_device_register(&ofsp8_400_eth_device);*/
/*	if (err) {*/
/*		pr_err("%s: Failed to add platform device\n", ETH_DRIVER_NAME);*/
/*		goto err_pdev;*/
/*	}*/

/*	return;*/

/*err_pdev:*/
/*	gpio_free(eth_data.fifo_gpio);*/
/*err_req_gpio_fifo:*/
/*	gpio_free(eth_data.irq_gpio);*/
/*err_req_gpio_irq:*/
	return;
}

/* High speed MMC card */

/*static struct omap2_hsmmc_info mmc[] = {*/
/*	{*/
/*		.mmc		= 1,*/
/*		.caps		= MMC_BUS_WIDTH_4,*/
/*		.gpio_cd	= -EINVAL,*/
/*		.gpio_wp	= MMC_GPIO_WP,*/
/*		.deferred	= true,*/
/*	},*/
/*	{}//	 Terminator */
/*};*/

/* User interface */


/* TPS children */

static struct regulator_consumer_supply ofsp8_400_twl_vmmc1_supplies[]= {
	REGULATOR_SUPPLY("vmmc", "omap_hsmmc.0"),
};

static struct regulator_consumer_supply  ofsp8_400_twl_vsim_supply[] = {
	REGULATOR_SUPPLY("vmmc_aux", "omap_hsmmc.0"),
};

static struct regulator_consumer_supply custom3_twl_vio_supplies[]= {
        REGULATOR_SUPPLY("hsusb0", "ehci-omap.0"),
};

/*static struct gpio_led ofsp8_400_gpio_leds[] = {*/
/*	{*/
/*		.name                   = "ofsp8_400:buzzer",*/
/*		.default_trigger        = "none",*/
/*		.active_low             = true,*/
/*		.gpio                   = BUZZER,*/
/*		.default_state          = LEDS_GPIO_DEFSTATE_OFF,*/
/*	},*/
/*	{*/
/*		.name                   = "ofsp8_400:speaker_mute",*/
/*		.default_trigger        = "none",*/
/*		.active_low             = false,*/
/*		.gpio                   = AUDIO_MUTE_AMPLIFIER,*/
/*		.default_state          = LEDS_GPIO_DEFSTATE_ON,*/
/*	},*/
/*};*/

/*static struct gpio_led_platform_data gpio_led_info = {*/
/*	.leds           = ofsp8_400_gpio_leds,*/
/*	.num_leds       = ARRAY_SIZE(ofsp8_400_gpio_leds),*/
/*};*/

/*static struct platform_device gpio_led_device = {*/
/*	.name   = "leds-gpio",*/
/*	.id     = -1,*/
/*	.dev    = {*/
/*		.platform_data  = &gpio_led_info,*/
/*	},*/
/*};*/

void __init ofsp8_400_gpioled_setup(void)
{
/*	if( fill_gpioled_dev( &gpio_led_device ) != 0 )*/
/*		return;*/

/*	if (platform_device_register(&gpio_led_device) < 0)*/
/*		pr_err("leds_gpio: Unable to register ofsp8 leds\n");*/
}

static int ofsp8_400_twl_gpio_setup(struct device *dev,
		unsigned gpio, unsigned ngpio)
{
/*	mmc[0].gpio_cd = gpio + 0;*/
/*	omap_hsmmc_late_init( get_omap2_hsmmc_info(mmc) );*/

	return 0;
}
/*static struct twl4030_gpio_platform_data ofsp8_400_twl_gpio_data = {*/
/*	.gpio_base	= OMAP_MAX_GPIO_LINES,*/
/*	.irq_base	= TWL4030_GPIO_IRQ_BASE,*/
/*	.irq_end	= TWL4030_GPIO_IRQ_END,*/
/*	.setup		= ofsp8_400_twl_gpio_setup,*/
/*};*/

/*static struct twl4030_codec_data ofsp8_400_twl_codec_data = {	*/
/*};*/

/*static struct twl4030_audio_data ofsp8_400_twl_audio_data = {*/
/*	.audio_mclk = 19200000,*/
/*	.codec = &ofsp8_400_twl_codec_data,*/
/*};*/

/*static struct regulator_init_data ofsp8_400_twl_vmmc1_data = {*/
/*	.constraints = {*/
/*		.min_uV			= 1850000,*/
/*		.max_uV			= 3150000,*/
/*		.valid_modes_mask	= REGULATOR_MODE_NORMAL*/
/*					| REGULATOR_MODE_STANDBY,*/
/*		.valid_ops_mask		= REGULATOR_CHANGE_VOLTAGE*/
/*					| REGULATOR_CHANGE_MODE*/
/*					| REGULATOR_CHANGE_STATUS,*/
/*	},*/
/*	.num_consumer_supplies	= ARRAY_SIZE(ofsp8_400_twl_vmmc1_supplies),*/
/*	.consumer_supplies	= ofsp8_400_twl_vmmc1_supplies,*/
/*};*/



/*// VSIM for MMC1 pins DAT4..DAT7 (2 mA, plus card == max 50 mA) */
/*static struct regulator_init_data ofsp8_400_twl_vsim_data = {*/
/*	.constraints = {*/
/*		.min_uV			= 1800000,*/
/*		.max_uV			= 3000000,*/
/*		.valid_modes_mask	= REGULATOR_MODE_NORMAL*/
/*					| REGULATOR_MODE_STANDBY,*/
/*		.valid_ops_mask		= REGULATOR_CHANGE_VOLTAGE*/
/*					| REGULATOR_CHANGE_MODE*/
/*					| REGULATOR_CHANGE_STATUS,*/
/*	},*/
/*	.num_consumer_supplies	= ARRAY_SIZE(ofsp8_400_twl_vsim_supply),*/
/*	.consumer_supplies	= ofsp8_400_twl_vsim_supply,*/
/*};*/


/*static struct regulator_init_data custom3_twl_vio_data = {		*/
/*       .constraints = {*/
/*               .min_uV                 = 1800000,*/
/*               .max_uV                 = 1850000,*/
/*               .valid_modes_mask       = REGULATOR_MODE_NORMAL*/
/*                                       | REGULATOR_MODE_STANDBY,*/
/*               .valid_ops_mask         = REGULATOR_CHANGE_VOLTAGE*/
/*                                       | REGULATOR_CHANGE_MODE*/
/*                                       | REGULATOR_CHANGE_STATUS,*/
/*       },*/
/*       .num_consumer_supplies  = ARRAY_SIZE(custom3_twl_vio_supplies),*/
/*       .consumer_supplies      = custom3_twl_vio_supplies,*/
/*};*/


/*// Devices on I2C bus */

/*static struct twl4030_platform_data ofsp8_400_twldata = {*/
/*	.gpio		= &ofsp8_400_twl_gpio_data,*/
/*	.audio		= &ofsp8_400_twl_audio_data,*/
/*	.vmmc1		= &ofsp8_400_twl_vmmc1_data,*/
/*	.vsim     = &ofsp8_400_twl_vsim_data,*/
/*	.vio		= &custom3_twl_vio_data,*/
/*};*/

void __init ofsp8_400_i2c_init(void)
{
	int i;
	int maxbusid = 2;
/*	struct i2c_board_info *i2c_boardinfo = ofsp8_400_i2c2_boardinfo;*/
/*	struct i2c_bus_hw_info *hw_bus_info = NULL;*/
/*	struct i2c_bus_bb_info *bb_bus_info = NULL;*/
/*	int numdev = 3;*/

/*	// Initialize omap i2c bus 1 */
/*	omap3_pmic_get_config(&ofsp8_400_twldata,*/
/*			TWL_COMMON_PDATA_USB | TWL_COMMON_PDATA_MADC |*/
/*			TWL_COMMON_PDATA_AUDIO,*/
/*			TWL_COMMON_REGULATOR_VDAC | TWL_COMMON_REGULATOR_VPLL2);*/

/*	ofsp8_400_twldata.vdac->constraints.apply_uV = true;*/
/*	ofsp8_400_twldata.vpll2->constraints.apply_uV = true;*/

/*	omap3_pmic_init("tps65950", &ofsp8_400_twldata);*/
/*	*/
/*	get_max_i2c_bus_id(&maxbusid);*/

/*	for(i = 1; i < (maxbusid + 1); i++) {*/

/*		// bit banging
/*		bb_bus_info = fill_i2c_bus_bb_info(bb_bus_info, i);*/
/*		if(bb_bus_info == NULL)*/
/*			continue;*/
/*		i2c_boardinfo = get_i2c_devices(i2c_boardinfo, i, &numdev);*/
/*		platform_device_register(bb_bus_info->gpio_i2c);*/
/*		i2c_register_board_info(bb_bus_info->id, i2c_boardinfo,*/
/*				numdev);*/
/*	}*/

/*	for(i = 1; i < (maxbusid + 1); i++) {*/

/*		hw_bus_info = fill_i2c_bus_hw_info(&default_i2c_hw_bus, i);*/
/*		if(hw_bus_info == NULL) {*/
/*			continue;*/
/*		}*/
/*		i2c_boardinfo = get_i2c_devices(i2c_boardinfo, i, &numdev);*/
/*		omap_register_i2c_bus(hw_bus_info->id, hw_bus_info->freq,*/
/*				i2c_boardinfo, numdev);*/

/*	}*/
}

/* MUSB */

/*static struct omap_musb_board_data ofsp8_400_musb_board_data = {*/
/*	.interface_type         = MUSB_INTERFACE_ULPI,*/
/*	.mode                   = MUSB_HOST,*/
/*	.power                  = 500,*/
/*	.extvbus                = 1,*/
/*};*/

/*// EHCI USB */

/*static struct usbhs_omap_platform_data usbhs_bdata __initdata = {*/

/*	.port_mode[0] = OMAP_EHCI_PORT_MODE_PHY,*/
/*	.port_mode[1] = OMAP_USBHS_PORT_MODE_UNUSED,*/
/*	.port_mode[2] = OMAP_USBHS_PORT_MODE_UNUSED,*/

/*	.phy_reset  = true,*/
/*	.reset_gpio_port[0]  = USB_GPIO_RESET,*/
/*	.reset_gpio_port[1]  = -EINVAL,*/
/*	.reset_gpio_port[2]  = -EINVAL*/
/*};*/

/*// BACKLIGHT PWM */

/*static struct platform_pwm_backlight_data ofsp8_400_pwm_bl_data = {*/
/*	.pwm_id		= 0,*/
/*	.max_brightness	= 255,*/
/*	.dft_brightness	= 255,*/
/*	// 0x4C4B400=5000000 ns = 5000 ms ^= 200 Hz */
/*	.pwm_period_ns	= 0x4C4B40,*/
/*};*/

/*static struct platform_device ofsp8_400_pwm_bl = {*/
/*	.name	= "pwm-backlight",*/
/*	.dev	= {*/
/*		.platform_data	= &ofsp8_400_pwm_bl_data,*/
/*	}*/
/*};*/

static void ofsp8_400_pwm_switchon(struct work_struct *work){
/*	struct backlight_device *bl;*/
/*	*/
/*	bl = platform_get_drvdata(&ofsp8_400_pwm_bl);*/
/*	if( bl != NULL){*/
/*		bl->props.brightness = 255;*/
/*		bl->ops->update_status(bl);*/
/*	}*/
/*	else*/
/*		pr_err(VIDEO_DRIVER_NAME ": Cannot enable backlight pwm\n");*/
}

struct delayed_work dwork;
static int BACKLIGHT_DELAY = HZ / 20;
static int __init ofsp8_400_pwm_init(void)
{
	int tmp;
	
/*	if( get_pwm_pdev(&ofsp8_400_pwm_bl) == NULL )*/
/*		return -ENODEV;*/

/*	if ( ret_splash == 0 ) {*/
/*		ofsp8_400_pwm_bl_data.dft_brightness = 0;*/
/*	}*/
/*		*/
/*	tmp = platform_device_register(&ofsp8_400_pwm_bl);*/
/*	if ( tmp )*/
/*	    pr_err("BACKLIGHT: Unable to add platform device\n");*/
/*	*/
/*	INIT_DELAYED_WORK(&dwork, ofsp8_400_pwm_switchon);*/
/*	*/
	return tmp;
}


/* DSS2 Video */

static int lcd_enabled;

static int ofsp8_400_panel_enable_lcd(struct omap_dss_device *dssdev)
{
/*	if (lcd_enabled) {*/
/*		pr_err(VIDEO_DRIVER_NAME ": Cannot enable panel"*/
/*				" while other panel is active!");*/
/*		return -EINVAL;*/
/*	}*/

/*	++lcd_enabled;*/

/*	gpio_set_value(ofsp8_400_display_info.gpio_dp, 1);*/
/*	gpio_set_value(ofsp8_400_display_info.gpio_bp, 1);*/

/*	schedule_delayed_work(&dwork, BACKLIGHT_DELAY);*/
  
	return 0;
}

static void ofsp8_400_panel_disable_lcd(struct omap_dss_device *dssdev)
{
/*	struct backlight_device *bl;*/

/*	if (!lcd_enabled)*/
/*		pr_warning(VIDEO_DRIVER_NAME ": panel already disabled!\n");*/
/*	else*/
/*		--lcd_enabled;*/

/*	bl = platform_get_drvdata(&ofsp8_400_pwm_bl);*/
/*	if( bl != NULL){*/
/*		bl->props.brightness = 0;*/
/*		bl->ops->update_status(bl);*/
/*  	}*/
/*	gpio_set_value(ofsp8_400_display_info.gpio_dp, 0);*/
/*	gpio_set_value(ofsp8_400_display_info.gpio_bp, 0);*/
}

static int ofsp8_400_panel_enable_tv(struct omap_dss_device *dssdev)
{
	return 0;
}

static void ofsp8_400_panel_disable_tv(struct omap_dss_device *dssdev)
{
}

//#include "mmi4-displays.h"

/*static struct omap_dss_board_info ofsp8_400_dss_data = {*/
/*	.num_devices	= ARRAY_SIZE(mmi4_dss_devices),*/
/*	.devices	= mmi4_dss_devices,*/
/*	.default_device	= MMI4_DSS_DEFAULT_PANEL,*/
/*};*/

void __init ofsp8_400_display_init(void)
{
/*	int r, disp;*/

/*	if( get_display_info( &ofsp8_400_display_info ) == NULL )*/
/*		return;*/

/*	// Display without Backlight will crash the kernel so we better check. */

/*	if( ofsp8_400_pwm_init() != 0 )*/
/*		return;*/

/*	lcd_enabled = 0;*/
/*	if ( ret_splash == 0 ) {*/
/*                r = gpio_request(ofsp8_400_display_info.gpio_dp, "DISP_POW");*/
/*	        if (r < 0) {*/
/*		        pr_err(VIDEO_DRIVER_NAME ": Unable to get DISP_POW GPIO\n");*/
/*		        return;*/
/*	        }*/

/*	        r = gpio_request(ofsp8_400_display_info.gpio_bp, "BKL_POW");*/
/*	        if (r < 0) {*/
/*	        	pr_err(VIDEO_DRIVER_NAME ": Unable to get BKL_POW GPIO\n");*/
/*	        	goto err_bkl_pow;*/
/*	        }*/
/*	        gpio_direction_output(ofsp8_400_display_info.gpio_dp, 0);*/
/*	        gpio_direction_output(ofsp8_400_display_info.gpio_bp, 0);*/
/*        }*/

/*	for (disp = 0; disp < ARRAY_SIZE(mmi4_dss_devices); disp++) {*/
/*		if (mmi4_dss_devices[disp]->type == OMAP_DISPLAY_TYPE_VENC) {*/
/*			// Analog Video */
/*			mmi4_dss_devices[disp]->platform_disable=*/
/*				ofsp8_400_panel_disable_tv;*/
/*			mmi4_dss_devices[disp]->platform_enable =*/
/*				ofsp8_400_panel_enable_tv;*/
/*		} else {*/
/*			// LCD */
/*			mmi4_dss_devices[disp]->platform_disable=*/
/*				ofsp8_400_panel_disable_lcd;*/
/*			mmi4_dss_devices[disp]->platform_enable =*/
/*				ofsp8_400_panel_enable_lcd;*/
/*		}*/
/*	}*/

/*	omap_display_init(&ofsp8_400_dss_data);*/
/*return;*/

/*err_bkl_pow:*/
/*	gpio_free(ofsp8_400_display_info.gpio_dp);*/
}

/* CAN controller */

static struct resource ofsp8_400_can_resources[] = {
	{
		.flags   = IORESOURCE_MEM | IORESOURCE_MEM_16BIT,
	}, {
		.flags   = IORESOURCE_IRQ | IORESOURCE_IRQ_LOWEDGE,
	},
};

struct sja1000_platform_data ofsp8_400_can_platform_data = {
	.osc_freq	= 24000000,
	.ocr		= OCR_TX1_PULLDOWN | OCR_TX0_PUSHPULL,
	.cdr		= CDR_CBP,
};

static struct platform_device ofsp8_400_can_device = {
	.name = "sja1000_platform",
	.dev = {
		.platform_data = &ofsp8_400_can_platform_data,
	},
	.resource = ofsp8_400_can_resources,
	.num_resources = ARRAY_SIZE(ofsp8_400_can_resources),
};

/*
 * Set the gpmc timings for sja1000. The timings are taken
 * from the data sheet SJA1000.pdf available on: http://nxp.com
 * REVISIT: Level shifters can add at least to the access latency.
 */

static int sja1000_gpmc_retime( u32 cs ) {
	struct gpmc_timings t;
	u32 l;
	
/*	int twal = 20;*/
/*	int tllrl = 30;*/
/*	int tclrl = 10;*/
/*	int twr = 50;*/

/*	int tllwl = 30;*/
/*	int tww = 40;*/
/*	*/
/*	memset(&t, 0, sizeof(t));*/

/*	// ALE Signal handling */
/*	//Signal is inverted */

/*	t.adv_on = twal;*/
/*	t.adv_wr_off = 0;*/
/*	t.adv_rd_off = 0;*/


/*	// Read Timings */

/*	t.access = 120;*/

/*	t.cs_on = t.adv_on + tllrl - tclrl;*/
/*	t.cs_rd_off = t.access + 10;*/
/*	t.rd_cycle = t.cs_rd_off + 10;	*/
/*	*/
/*	t.oe_on  =  t.adv_on + tllrl;*/
/*	t.oe_off =  t.oe_on + twr;     */
/*	*/

/*	// Write timings */
/*	*/
/*	t.wr_access = 100;*/
/*	t.cs_wr_off = t.wr_access + 10;*/
/*	*/
/*	*/
/*	t.we_on  = t.adv_on + tllwl;*/
/*	t.we_off = t.we_on + tww;	*/

/*	t.wr_cycle = t.cs_wr_off + 10; */
/*	*/
/*	if (cpu_is_omap34xx()) {*/
/*		t.wr_data_mux_bus = twal + tllwl;*/
/*	}			*/


/*	// Setup Config1 Register:*/
/*//	Device Size 8 BIT & muxdata enabled */

/*	l = GPMC_CONFIG1_DEVICESIZE(0);   */
/*   	l |= GPMC_CONFIG1_MUXADDDATA;*/
/*        */
/*	gpmc_cs_write_reg(cs, GPMC_CS_CONFIG1, l);*/

/*	return gpmc_cs_set_timings(cs, &t);*/
/*	*/
	
}


struct mmi4_can_data can_data = {
	.irq_gpio = CAN_GPIO_IRQ,
	.cs       = CAN_CS,
};

void __init ofsp8_400_can_init(void)
{
	unsigned long cs_mem_base;
	int err;

/*	if( get_can_dev(&ofsp8_400_can_device, &can_data) == NULL )*/
/*		return;*/
/*	*/
/*	// Setup the GMPC CONFIG_5_i Registers */
/*	sja1000_gpmc_retime( can_data.cs );*/

/*	*/
/*	if (gpmc_cs_request(can_data.cs, SZ_16M, &cs_mem_base) < 0) {*/
/*		pr_err("%s: Failed to request GPMC memory\n", ofsp8_400_can_device.name);*/
/*		return;*/
/*	}*/
/*	*/
/*	ofsp8_400_can_resources[0].start = cs_mem_base;*/
/*	ofsp8_400_can_resources[0].end = cs_mem_base + 0x100 - 1;*/
/*	*/
/*	ofsp8_400_can_resources[1].start = gpio_to_irq(can_data.irq_gpio);*/
/*	ofsp8_400_can_resources[1].end= gpio_to_irq(can_data.irq_gpio);	*/

/*	err = gpio_request(can_data.irq_gpio, "CAN driver irq");*/
/*	if (err) {*/
/*		pr_err("%s: Failed to request GPIO%d for IRQ\n", ofsp8_400_can_device.name,*/
/*				can_data.irq_gpio);*/
/*		goto err_req_gpio_irq;*/
/*	}*/
/*	gpio_direction_input(can_data.irq_gpio);*/

/*	err = platform_device_register(&ofsp8_400_can_device);*/
/*	if (err) {*/
/*		pr_err("%s: Failed to add platform device\n", ofsp8_400_can_device.name);*/
/*		goto err_pdev;*/
/*	}*/
/*	*/
/*	return;*/

/*err_pdev:*/
/*	gpio_free(can_data.irq_gpio);*/
/*err_req_gpio_irq:*/
	return;
}




#ifdef CONFIG_OMAP_MUX
static struct omap_board_mux board_mux[] __initdata = {
	{ .reg_offset = OMAP_MUX_TERMINATOR },
};


/* Default pinmuxing when no dtb present. When changing things, make sure
 * the change is reflected in the ofsp8_400 dts file as well!
 */
static struct omap_board_mux board_mux_default[] __initdata = {
	/* EXTENSION INTERFACE */       
	OMAP3_MUX(CAM_D0,OMAP_MUX_MODE4 | OMAP_PIN_INPUT),	/* GPIO_99 */
	OMAP3_MUX(CAM_D1,OMAP_MUX_MODE4 | OMAP_PIN_INPUT),	/* GPIO_100 */
	OMAP3_MUX(CAM_D2,OMAP_MUX_MODE4 | OMAP_PIN_INPUT),	/* GPIO_101 */
	OMAP3_MUX(CAM_D3,OMAP_MUX_MODE4 | OMAP_PIN_INPUT),	/* GPIO_102 */
	OMAP3_MUX(CAM_D4,OMAP_MUX_MODE4 | OMAP_PIN_INPUT),	/* GPIO_103 */
	OMAP3_MUX(CAM_D5,OMAP_MUX_MODE4 | OMAP_PIN_INPUT),	/* GPIO_104 */
	OMAP3_MUX(CAM_D6,OMAP_MUX_MODE4 | OMAP_PIN_INPUT),	/* GPIO_105 */
	OMAP3_MUX(CAM_D7,OMAP_MUX_MODE4 | OMAP_PIN_INPUT),	/* GPIO_106 */
	OMAP3_MUX(CAM_D8,OMAP_MUX_MODE4 | OMAP_PIN_INPUT),	/* GPIO_107 */
	OMAP3_MUX(CAM_D9,OMAP_MUX_MODE4 | OMAP_PIN_INPUT),	/* GPIO_108 */
	OMAP3_MUX(CAM_XCLKA,OMAP_MUX_MODE4 | OMAP_PIN_INPUT),	/* GPIO_96 */
	OMAP3_MUX(CAM_XCLKB,OMAP_MUX_MODE4 | OMAP_PIN_INPUT),	/* GPIO_111 */
	OMAP3_MUX(CAM_VS,OMAP_MUX_MODE4 | OMAP_PIN_INPUT),	/* GPIO_95 */
	OMAP3_MUX(CAM_HS,OMAP_MUX_MODE4 | OMAP_PIN_INPUT),	/* GPIO_94 */
	OMAP3_MUX(CAM_PCLK,OMAP_MUX_MODE4 | OMAP_PIN_INPUT),	/* GPIO_97 */
	OMAP3_MUX(CAM_PCLK,OMAP_MUX_MODE4 | OMAP_PIN_INPUT),	/* GPIO_179 */
	/* GPIO */
	OMAP3_MUX(MCBSP1_CLKR,OMAP_MUX_MODE4 | OMAP_PIN_OUTPUT),/* GPIO2 = GPIO156 (output only) */
	OMAP3_MUX(MCBSP1_FSR,OMAP_MUX_MODE4 | OMAP_PIN_OUTPUT),	/* GPIO1 = GPIO157 (output only) */
	OMAP3_MUX(MCBSP1_DR,OMAP_MUX_MODE4 | OMAP_PIN_OUTPUT),	/* GPIO3 = GPIO159 (output only) */
	OMAP3_MUX(MCBSP1_DX,OMAP_MUX_MODE4 | OMAP_PIN_INPUT),	/* GPIO4_18 = GPIO158 */
	OMAP3_MUX(MCBSP1_FSX,OMAP_MUX_MODE4 | OMAP_PIN_INPUT),	/* GPIO5_18 = GPIO161 */
	OMAP3_MUX(MCBSP1_CLKX,OMAP_MUX_MODE4 | OMAP_PIN_INPUT),	/* GPIO6_18 = GPIO162 */
	/* USB0 (musb)*/
	OMAP3_MUX(HSUSB0_CLK, OMAP_MUX_MODE0 | OMAP_PIN_INPUT),	/* USB0 CLK */
	OMAP3_MUX(HSUSB0_STP, OMAP_MUX_MODE0 | OMAP_PIN_OUTPUT),/* USB0 STP */
	OMAP3_MUX(HSUSB0_DIR, OMAP_MUX_MODE0 | OMAP_PIN_INPUT),	/* USB0 DIR */
	OMAP3_MUX(HSUSB0_NXT, OMAP_MUX_MODE0 | OMAP_PIN_INPUT),	/* USB0 NXT */
	OMAP3_MUX(HSUSB0_DATA0, OMAP_MUX_MODE0 | OMAP_PIN_INPUT),	/* USB0 D0 */
	OMAP3_MUX(HSUSB0_DATA1, OMAP_MUX_MODE0 | OMAP_PIN_INPUT),	/* USB0 D1 */
	OMAP3_MUX(HSUSB0_DATA2, OMAP_MUX_MODE0 | OMAP_PIN_INPUT),	/* USB0 D2 */
	OMAP3_MUX(HSUSB0_DATA3, OMAP_MUX_MODE0 | OMAP_PIN_INPUT),	/* USB0 D3 */
	OMAP3_MUX(HSUSB0_DATA4, OMAP_MUX_MODE0 | OMAP_PIN_INPUT),	/* USB0 D4 */
	OMAP3_MUX(HSUSB0_DATA5, OMAP_MUX_MODE0 | OMAP_PIN_INPUT),	/* USB0 D5 */
	OMAP3_MUX(HSUSB0_DATA6, OMAP_MUX_MODE0 | OMAP_PIN_INPUT),	/* USB0 D6 */
	OMAP3_MUX(HSUSB0_DATA7, OMAP_MUX_MODE0 | OMAP_PIN_INPUT),	/* USB0 D7 */
	/* USB1 - is muxed and initialized in U-Boot already. Need only to configure the reset pin. */
	OMAP3_MUX(ETK_D13, OMAP_MUX_MODE4 | OMAP_PIN_INPUT),	/* #USB_RESET */
	/* AUDIO */
	OMAP3_MUX(MCBSP_CLKS, OMAP_MUX_MODE0 | OMAP_PIN_INPUT),	/* I2S_MCLK */
	OMAP3_MUX(MCBSP2_FSX, OMAP_MUX_MODE0 | OMAP_PIN_INPUT),	/* I2S_LRCLK */
	OMAP3_MUX(MCBSP2_CLKX, OMAP_MUX_MODE0 | OMAP_PIN_INPUT),/* I2S_BCLK */
	OMAP3_MUX(MCBSP2_DR, OMAP_MUX_MODE0 | OMAP_PIN_INPUT),	/* I2S_DIN */
	OMAP3_MUX(MCBSP2_DX, OMAP_MUX_MODE0 | OMAP_PIN_INPUT),	/* I2S_DOUT */
	/* CAN */
	OMAP3_MUX(GPMC_NCS5, OMAP_MUX_MODE0 | OMAP_PIN_OUTPUT),	/* #GPMC_CS5 CAN */
	OMAP3_MUX(SDMMC2_DAT1, OMAP_MUX_MODE0 | OMAP_PIN_INPUT),/* #CAN_IRQ */
	/* SERIELL */
	OMAP3_MUX(UART1_TX, OMAP_MUX_MODE0 | OMAP_PIN_OUTPUT),	/* TXD1 */
	OMAP3_MUX(UART1_RX, OMAP_MUX_MODE0 | OMAP_PIN_INPUT),	/* TXD1 */
	OMAP3_MUX(UART1_RTS, OMAP_MUX_MODE0 | OMAP_PIN_OUTPUT),	/* RTS1 */
	OMAP3_MUX(UART1_CTS, OMAP_MUX_MODE0 | OMAP_PIN_INPUT),	/* CTS1 */
	/* ETHERNET */
	OMAP3_MUX(GPMC_NCS4, OMAP_MUX_MODE0 | OMAP_PIN_INPUT_PULLUP),	/* #GPMC_CS4 ETH */
	OMAP3_MUX(SDMMC2_CLK, OMAP_MUX_MODE4 | OMAP_PIN_INPUT_PULLUP),  /* #ETH_IRQ */
	OMAP3_MUX(SDMMC2_CMD, OMAP_MUX_MODE4 | OMAP_PIN_INPUT ),	/* ETH_FIFO_SEL */
	/* SD CARD */
	OMAP3_MUX(SDMMC1_DAT4, OMAP_MUX_MODE4 | OMAP_PIN_INPUT),	/* SD_WP */
	OMAP3_MUX(GPMC_CLK, OMAP_MUX_MODE4 | OMAP_PIN_INPUT),		/* #SD_CD */
	/* I2C #2 */
	OMAP3_MUX(I2C2_SCL, OMAP_MUX_MODE0 | OMAP_PIN_INPUT_PULLUP),	/* SCL2 */
	OMAP3_MUX(I2C2_SDA, OMAP_MUX_MODE0 | OMAP_PIN_INPUT_PULLUP),	/* SDA2 */
	/* DIVERSES */
	OMAP3_MUX(SDMMC2_DAT0, OMAP_MUX_MODE4 | OMAP_PIN_OUTPUT),	/* BUZZER */
	OMAP3_MUX(CAM_WEN, OMAP_MUX_MODE4 | OMAP_PIN_INPUT),		/* #TOUCH_IRQ */
	OMAP3_MUX(MCSPI1_CS0, OMAP_MUX_MODE4 | OMAP_PIN_INPUT_PULLDOWN),/* #AMP_SD */
	/* DISPLAY */
	OMAP3_MUX(DSS_DATA0, OMAP_MUX_MODE0 | OMAP_PIN_OUTPUT),	/* LCD_D0 */
	OMAP3_MUX(DSS_DATA1, OMAP_MUX_MODE0 | OMAP_PIN_OUTPUT),	/* LCD_D1 */
	OMAP3_MUX(DSS_DATA2, OMAP_MUX_MODE0 | OMAP_PIN_OUTPUT),	/* LCD_D2 */
	OMAP3_MUX(DSS_DATA3, OMAP_MUX_MODE0 | OMAP_PIN_OUTPUT),	/* LCD_D3 */
	OMAP3_MUX(DSS_DATA4, OMAP_MUX_MODE0 | OMAP_PIN_OUTPUT),	/* LCD_D4 */
	OMAP3_MUX(DSS_DATA5, OMAP_MUX_MODE0 | OMAP_PIN_OUTPUT),	/* LCD_D5 */
	OMAP3_MUX(DSS_DATA18, OMAP_MUX_MODE0 | OMAP_PIN_OUTPUT),/* LCD_D18 */
	OMAP3_MUX(DSS_DATA19, OMAP_MUX_MODE0 | OMAP_PIN_OUTPUT),/* LCD_D19 */
	OMAP3_MUX(DSS_DATA20, OMAP_MUX_MODE0 | OMAP_PIN_OUTPUT),/* LCD_D20 */
	OMAP3_MUX(DSS_DATA21, OMAP_MUX_MODE0 | OMAP_PIN_OUTPUT),/* LCD_D21 */
	OMAP3_MUX(DSS_DATA22, OMAP_MUX_MODE0 | OMAP_PIN_OUTPUT),/* LCD_D22 */
	OMAP3_MUX(DSS_DATA23, OMAP_MUX_MODE0 | OMAP_PIN_OUTPUT),/* LCD_D23 */
	OMAP3_MUX(MCBSP3_DX, OMAP_MUX_MODE4 | OMAP_PIN_OUTPUT),	/* BKL_POW */
	OMAP3_MUX(MCBSP3_DR, OMAP_MUX_MODE4 | OMAP_PIN_OUTPUT),	/* DISP_POW */
	OMAP3_MUX(MCSPI2_CS1, OMAP_MUX_MODE1 | OMAP_PIN_OUTPUT),/* PWM0 = BRIGHTNESS */
	{ .reg_offset = OMAP_MUX_TERMINATOR },
};
#else
# define board_mux	NULL
#define board_mux_default NULL
#endif



/* Remove this Reset Function once the reset signal between OMAP and TWL is available. */
#define TWL4030_WATCHDOG_CFG_REG_OFFS   0x3
static void ofsp8_400_reset(char mode, const char *cmd)
{
	pr_notice("mmi4: Trigger TWL4030 internal watchdog (1s) to reset itself");
//	twl_i2c_write_u8(TWL4030_MODULE_PM_RECEIVER, 1, TWL4030_WATCHDOG_CFG_REG_OFFS);

//	omap_prcm_restart( mode, cmd);
}

static int ofmux2reg( const char *type ){
	if(strcmp( type, "PTD" ) == 0)
		return 0;
		
	if(strcmp( type, "PTU") == 0)
		return OMAP_PULL_UP;
		
	if(strcmp( type, "EN") == 0)
		return OMAP_PULL_ENA;
		
	if(strcmp( type, "DIS") == 0)
		return 0;
		
	if(strcmp( type, "IEN") == 0)
		return OMAP_INPUT_EN;

	if(strcmp( type, "IDIS") == 0)
		return 0;
		
	return 0;
}

static void mux_ue_pinctrl(void){
	struct device_node *dn;
	const char *of_val, *pinmuxname;
	struct property* pp;
	int pinmuxregval, pinmuxregcnt, i;

	dn = of_find_node_by_name(NULL, "ue_pinctrl");
	if( dn != NULL ){
		printk( "mmi4: Pinmuxing according to ue_pinctrl from device tree\n");
		for_each_property_of_node(dn, pp){
			if(strncmp( pp->name, "pin_", 4 ) == 0){
				pinmuxregval = 0;
				pinmuxregcnt = of_property_count_strings(dn, pp->name);
				if (pinmuxregcnt<1) return;
				of_property_read_string_index( dn, pp->name, 0, &pinmuxname );
				for( i=1; i < pinmuxregcnt; i++ ){
					of_property_read_string_index( dn, pp->name, i, &of_val );
					pinmuxregval |= ofmux2reg( of_val );
				}
				printk( KERN_INFO "Muxing ue_pinctrl %s as %s with type 0x%x\n", pp->name, pinmuxname, pinmuxregval );
				omap_mux_init_signal(pinmuxname, pinmuxregval);
			}
		}
	}
}

static void __init ofsp8_400_init(void)
{
	if( ! of_find_node_by_path("/") ){
		printk( "mmi4: No dtb found. Using OFSP8 defaults for pinmux and hwconfig.\n");
	//	omap3_mux_init(board_mux_default, OMAP_PACKAGE_CUS);
	}
	else{
		//omap3_mux_init(board_mux, OMAP_PACKAGE_CUS);
		/* Currently (kernel 3.4) the pinctrl module of dt is not 
		 * yet working, so doing muxing our own way: */
		mux_ue_pinctrl();
	}
	//omap_hsmmc_init( get_omap2_hsmmc_info(mmc) );
//	ofsp8_400_flash_init();
//	ofsp8_400_i2c_init();
//	ofsp8_400_gpioled_setup();
//        ofsp8_400_display_init();
//	omap_serial_init();
//	omap_sdrc_init(NULL, NULL);
//	ofsp8_400_audio_init();
//	ofsp8_400_eth_init();
//	ofsp8_400_can_init();

/*	if(! fill_musb_board_data(&ofsp8_400_musb_board_data))
		usb_musb_init(&ofsp8_400_musb_board_data);

        if(! fill_usbhs_bdata(&usbhs_bdata)) {
                if ( usbhs_bdata.reset_gpio_port[0] == 0 ) {
                        usbhs_bdata.reset_gpio_port[0] = -EINVAL;
                }
		usbhs_init(&usbhs_bdata);
        }
*/
}

__setup("ethaddr=", setup_ethmac);
__setup("splash_enabled=", setup_splash);

MACHINE_START(OMAP3_OFSP8_KERNEL400, "Ultratronik OFSP8 Rev.400 and higher")
	.atag_offset	= 0x100,
	.map_io		= omap3_map_io,
	.reserve	= omap_reserve,
	.init_early	= omap35xx_init_early,
//	.init_irq	= omap3_init_irq,
//	.handle_irq	= omap3_intc_handle_irq,
	.init_machine	= ofsp8_400_init,
//	.timer		= &omap3_secure_timer,
	.dt_compat	= omap3_boards_compat,
//	.restart	= ofsp8_400_reset,
MACHINE_END
