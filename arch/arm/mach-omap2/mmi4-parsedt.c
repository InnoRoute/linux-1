/*
* Copyright (C) 2013 Ultratronik GmbH
* Author: Andreas Naumann <anaumann@ultratronik.de>
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License version 2 as
* published by the Free Software Foundation.
*/

#include <linux/of_irq.h>
#include <linux/of_platform.h>
#include <linux/gpio.h>
//#include <plat/gpmc.h>
#include <linux/omap-gpmc.h>
#include <linux/platform_data/usb-omap.h>
//#include <plat/usb.h> 
#include <linux/i2c.h>
#include <linux/slab.h>
#include "mmi4-parsedt.h"
//#include <linux/i2c-gpio.h>
#include <linux/platform_data/i2c-gpio.h>
#include <linux/pwm_backlight.h>
#include <linux/leds.h>
//#include <linux/i2c/mcp23017.h> need?

extern int g_tsc2007_gpio;
extern int ofsp8_400_get_tspendown(void);

/*struct tsc2007_platform_data default_tsc2007_struct = {
	.model                  = 2007,
	.x_plate_ohms           = 50,
	.get_pendown_state      = ofsp8_400_get_tspendown,
	.irq_flags		= IRQF_TRIGGER_FALLING,
};
*/

/*struct qt2160_platform_data default_qt2160 = {
	.slider_length = 8,
	.slider_axis = ABS_X,

};
*/

/*struct mcp23017_platform_data default_mcp23017_io_ex = {
	.gpio_base = 220,
};
*/

/* dt helpers */
static u32 device_is_enabled( char* enable_key )
{
u32 ret = 0;
struct device_node *dn = of_find_node_by_name(NULL, "hwconfig");
of_property_read_u32_array(dn, enable_key, &ret, 1);
/* default */
if (! of_have_populated_dt())
	ret = 1;

return ret;
}

static void get_u8_from_dtb( char* key, u8 *val)
{
	int ret ;
	struct device_node *dn = of_find_node_by_name(NULL, "hwconfig");
	if (! of_property_read_u32_array(dn, key, &ret, 1))
		*val = (u8) ret;
}

static void get_u16_from_dtb( char* key, u16 *val)
{
	int ret ;
	struct device_node *dn = of_find_node_by_name(NULL, "hwconfig");
	if (! of_property_read_u32_array(dn, key, &ret, 1))
		*val = (u16) ret;
}

/* Returns val(default value) if key not found in hw_config */
static unsigned get_unsigned_from_dtb( char* key, unsigned val)
{
	int ret ;
	struct device_node *dn = of_find_node_by_name(NULL, "hwconfig");
	if (of_property_read_u32_array(dn, key, &ret, 1))
		return val;
	else
		return ret;
}

static void get_int_from_dtb( char* key, int *val)
{
struct device_node *dn = of_find_node_by_name(NULL, "hwconfig");
of_property_read_u32_array(dn, key, val, 1);
}

char * get_string_from_index(int index, char* key ) {
	struct device_node *dn = of_find_node_by_name(NULL, "hwconfig");
	char *ret = NULL;
	of_property_read_string_index(dn, key, index, (const char **) &ret);

	return ret;
}

u32 get_hexstr2int_from_index(int index, char* key ) {
	struct device_node *dn = of_find_node_by_name(NULL, "hwconfig");
	u32 ret = 0;
	char *str;
	of_property_read_string_index(dn, key, index, (const char **) &str);

	ret = simple_strtol(str, NULL, 16);
	return ret;
}

u32 get_intstr2int_from_index(int index, char* key ) {
	struct device_node *dn = of_find_node_by_name(NULL, "hwconfig");
	u32 ret = 0;
	char *str;
	of_property_read_string_index(dn, key, index, (const char **) &str);

	ret = simple_strtol(str, NULL, 10);
	return ret;
}

/* High speed MMC card */
struct omap2_hsmmc_info* get_omap2_hsmmc_info( struct omap2_hsmmc_info* mmc )
{
if( device_is_enabled( "i_enable_mmc" ))
	return mmc;
else
	return NULL;
}

/* Flash */
struct omap_nand_platform_data* get_nand_pdata( struct omap_nand_platform_data* nand )
{
	if( device_is_enabled( "i_enable_flash" ))
		return nand;
	else
		return NULL;
}

/* Display */
struct display_info *get_display_info( struct display_info* display_info){
	if( device_is_enabled("i_enable_video")) {
		get_int_from_dtb("i_lcd_gpio_dp", &display_info->gpio_dp);
		get_int_from_dtb("i_lcd_gpio_bp", &display_info->gpio_bp);
		get_int_from_dtb("i_lcd_gpio_br", &display_info->gpio_br);
		return display_info;
	}
	else
		return NULL;
}

/* PWM */
static int get_pwm_id(u32 pwm_gpio)
{
	int pwm_id = -1;
	
	switch ( pwm_gpio )
	{
		case 182:
			pwm_id = 0;
			break;
		
		case 179:
			pwm_id = 1;
			break;
			
		case 180:
			pwm_id = 2;
			break;
		
		case 181:
			pwm_id = 3;
			break;
		
		default:
			pr_err("PWM ID: Invalid number\n");
			break;
		}
	return pwm_id;
}

struct platform_device* get_pwm_pdev( struct platform_device *pwm ){
	if( device_is_enabled("i_enable_video")){
		u32 gpio_br = 0;
		get_int_from_dtb("i_lcd_gpio_br", &gpio_br);
		if( gpio_br != 0 )
			((struct platform_pwm_backlight_data*)(pwm->dev.platform_data))->pwm_id = get_pwm_id(gpio_br);
	}
	else
		return NULL;
	
	return pwm;
}

/* Audio */
struct platform_device *get_audio_dev(struct platform_device* audio){
	if( device_is_enabled("i_enable_audio"))
		return audio;
	else
		return NULL;
}

/* CAN */
struct platform_device* get_can_dev( struct platform_device* can, struct mmi4_can_data* can_data){
	if( device_is_enabled("i_enable_can")){
		get_int_from_dtb("i_can_irq_gpio", &can_data->irq_gpio);
		get_int_from_dtb("i_can_cs", &can_data->cs);
		return  can;
	}
	else
		return NULL;
}

/* MUSB */
int fill_musb_board_data( struct omap_musb_board_data
	*board_data) {

if( device_is_enabled("i_enable_musb")) {
	get_u8_from_dtb("i_musb_mode", &board_data->mode);
	get_u16_from_dtb("i_musb_power", &board_data->power );
	board_data->extvbus = get_unsigned_from_dtb("i_musb_extvbus", board_data->extvbus);
	return 0;
} else
	return -ENODATA;
}

int fill_gpioled_dev( struct platform_device* gpio_led_dev ){
	u32 gpioleds_count = 0, i;
	const char *buffer;
	struct gpio_led *gpio_leds;
	struct property* pp;
	struct device_node *dn = of_find_node_by_name(NULL, "hwconfig");

	/* If we find no device tree, use the default. */
	if( dn == NULL )
		return 0;

	/* Before allocating count how much */
	for_each_property_of_node(dn, pp)
	{
		if(strncmp( pp->name, "i_gpio_leds_", 12 ) == 0){
			gpioleds_count++;
		}
	}
	if( !gpioleds_count )
		return -ENODATA;

	/* Now build the structure*/
	gpio_leds = kzalloc(sizeof(struct gpio_led) * gpioleds_count, GFP_KERNEL);
	i = 0;
	for_each_property_of_node(dn, pp){
		if(strncmp( pp->name, "i_gpio_leds_", 12 ) == 0){
			of_property_read_string_index(dn, pp->name, 0, &gpio_leds[i].name);
			of_property_read_string_index(dn, pp->name, 1, &gpio_leds[i].default_trigger);
			of_property_read_string_index(dn, pp->name, 2, &buffer);
			if ( strcmp(buffer, "true") == 0 )
				gpio_leds[i].active_low = 1;
			else
				gpio_leds[i].active_low = 0;
			of_property_read_string_index(dn, pp->name, 3, &buffer);
			gpio_leds[i].gpio = simple_strtol(buffer, NULL, 10);
			of_property_read_string_index(dn, pp->name, 4, &buffer);
			gpio_leds[i].default_state = simple_strtol(buffer, NULL, 10);
			/* printk( KERN_INFO "Adding from %s gpio led device %s, with active low set as %d, and GPIO %d\n",
					pp->name,
					gpio_leds[i].name,
					gpio_leds[i].active_low,
					gpio_leds[i].gpio); */
			i++;
		}
	}
	/* Replace the default gpio_leds with our dynamic version. */
	((struct gpio_led_platform_data*)(gpio_led_dev->dev.platform_data))->leds = gpio_leds;
	((struct gpio_led_platform_data*)(gpio_led_dev->dev.platform_data))->num_leds = gpioleds_count;
	return 0;
}

/* EHCI */
int fill_usbhs_bdata(struct usbhs_omap_platform_data* board_data) {

if( device_is_enabled("i_enable_ehci")) {
	get_int_from_dtb("i_usb_reset_gpio", &board_data->reset_gpio_port[0]);
	get_int_from_dtb("i_ehci_power", &board_data->power);
 	return 0;
} 
else
	return -ENODATA;
}

int fill_eth_platform_device(struct platform_device *eth_plat_dev, struct mmi4_eth_data *eth_data) {
if( device_is_enabled("i_enable_eth")) {
	get_int_from_dtb("i_eth_irq_gpio", &eth_data->irq_gpio);
	get_int_from_dtb("i_eth_cs", &eth_data->cs);
	get_int_from_dtb("i_eth_fifo_gpio", &eth_data->fifo_gpio);
	return 0;
} else
	return -ENODATA;
}


int fill_i2c_platdata(struct i2c_board_info * binfo, struct property *prop) {
/*	if( strcmp (binfo->type, "tsc2007") == 0) {
		binfo->platform_data = &default_tsc2007_struct;
		g_tsc2007_gpio = get_intstr2int_from_index(3, prop->name);
	}*/
	/*if( strncmp (binfo->type, "qt2160-slider", 13) == 0) {
		binfo->platform_data = &default_qt2160;
	}
	*/
  /*  if( strcmp (binfo->type, "mcp23017") == 0) {
		binfo->platform_data = &default_mcp23017_io_ex;
	}    
*/
	return 0;
}

int set_string (char *dest, char* key, int index) {
	struct device_node *dn = of_find_node_by_name(NULL, "hwconfig");
	char *str;
	int uLen;

	of_property_read_string_index(dn, key, index, (const char **) &str);
	uLen = strlen(str) + 1;
	if(uLen <= I2C_NAME_SIZE)
		memcpy(dest, str, uLen);

	return 0;
}

struct i2c_board_info * get_i2c_devices(struct i2c_board_info *i2c_boardinfo, int bus_id, int *devnum) {
	struct device_node *dn = of_find_node_by_name(NULL, "hwconfig");
	struct property* pp;
	int numdev = 0;
	int index = 0;

	if( dn == NULL ) {
		gpio_request(g_tsc2007_gpio,"TOUCH_IRQ");
		gpio_direction_input(g_tsc2007_gpio);
		i2c_boardinfo[2].irq = gpio_to_irq(g_tsc2007_gpio);
		return i2c_boardinfo;
	}

	i2c_boardinfo = NULL;

	for_each_property_of_node(dn, pp)
	{
		if(strncmp( pp->name, "i_i2c_device_", 13 ) == 0) {
			int busnum = get_intstr2int_from_index(2, pp->name);
			if(busnum == bus_id) {
				numdev++;
			}
		}
	}
	if(numdev)
		i2c_boardinfo = kzalloc(sizeof(struct i2c_board_info)
				* numdev, GFP_KERNEL);
	index = 0;
	for_each_property_of_node(dn, pp) {
		if (strncmp( pp->name, "i_i2c_device_", 13 ) == 0) {
			int busnum = get_intstr2int_from_index(2, pp->name);
			if(busnum == bus_id) {

				/* fill struct */
				set_string(i2c_boardinfo[index].type,	pp->name, 0);
				i2c_boardinfo[index].addr =
					get_hexstr2int_from_index(1, pp->name);
				i2c_boardinfo[index].irq =
					get_intstr2int_from_index(3, pp->name);
				fill_i2c_platdata(&i2c_boardinfo[index], pp);
				if(i2c_boardinfo[index].irq) {
					gpio_request(i2c_boardinfo[index].irq, i2c_boardinfo[index].type);
					gpio_direction_input(i2c_boardinfo[index].irq);
					i2c_boardinfo[index].irq = gpio_to_irq(i2c_boardinfo[index].irq);
				}
				index++;
			}
		}
	}
	*devnum = numdev;
	return i2c_boardinfo;
}

struct i2c_bus_bb_info * fill_i2c_bus_bb_info(struct i2c_bus_bb_info *bus_info, int bus_id) {
	struct device_node *dn = of_find_node_by_name(NULL, "hwconfig");
	struct property* pp;
	
	if( dn == NULL ) {
		if(bus_info == NULL)
			return NULL;
		
		if(bus_info->id == bus_id)
			return bus_info;
		else
			return NULL;
	}

	bus_info = NULL;

	for_each_property_of_node(dn, pp) {
		if (strncmp( pp->name, "i_i2c_bus_bb", 12) == 0) {
			if(get_intstr2int_from_index(0,pp->name) == bus_id) {
				struct i2c_gpio_platform_data *gpio_i2c_pdata;

				bus_info = kzalloc (sizeof(struct i2c_bus_bb_info),
						GFP_KERNEL);
				bus_info->gpio_i2c = kzalloc (sizeof(struct platform_device), GFP_KERNEL);
				gpio_i2c_pdata =
					bus_info->gpio_i2c->dev.platform_data;
				gpio_i2c_pdata = kzalloc(sizeof(struct
							i2c_gpio_platform_data) ,
						GFP_KERNEL);
				bus_info->id =  get_intstr2int_from_index(0,
						pp->name);
				bus_info->gpio_i2c->name = "i2c-gpio";
				bus_info->gpio_i2c->id = bus_info->id;

				gpio_i2c_pdata->udelay = get_intstr2int_from_index(1, pp->name);
//				gpio_i2c_pdata->scl_pin = get_intstr2int_from_index(2, pp->name);
//				gpio_i2c_pdata->sda_pin = get_intstr2int_from_index(3, pp->name);
				gpio_i2c_pdata->timeout = HZ;
				bus_info->gpio_i2c->dev.platform_data =
					gpio_i2c_pdata;
			}
		}
	}
	return bus_info;
}

struct i2c_bus_hw_info * fill_i2c_bus_hw_info(struct i2c_bus_hw_info *bus_info, int bus_id) {
	struct device_node *dn = of_find_node_by_name(NULL, "hwconfig");
	struct property* pp;
	
	if( dn == NULL ) {
		if(bus_info == NULL)
			return NULL;
	
		if(bus_info->id == bus_id)
			return bus_info;
		else
			return NULL;
	}

	bus_info = NULL;

	for_each_property_of_node(dn, pp) {
		if (strncmp( pp->name, "i_i2c_bus_hw", 12) == 0) {
			if(get_intstr2int_from_index(0,pp->name) == bus_id) {
				bus_info = kzalloc (sizeof(struct i2c_bus_hw_info),
						GFP_KERNEL);
				bus_info->id =
					get_intstr2int_from_index(0, pp->name);
				bus_info->freq =
					get_intstr2int_from_index(1, pp->name);
			}
		}
	}

		return bus_info;
}

int get_max_i2c_bus_id(int *ret_max) {
	struct device_node *dn = of_find_node_by_name(NULL, "hwconfig");
	struct property* pp;
	u32 max = 0;
	
	if( dn == NULL )
		return 0;

	for_each_property_of_node(dn, pp) {
		if (strncmp( pp->name, "i_i2c_bus_", 10) == 0) {
			int id = get_intstr2int_from_index(0, pp->name);
			if(max < id);
				max = id;
		}
	}
	*ret_max = max;
	return 0;
}
