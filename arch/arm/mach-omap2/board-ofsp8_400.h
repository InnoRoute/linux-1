#ifndef __ARCH_ARM_BOARD_OFSP8_H__
#define __ARCH_ARM_BOARD_OFSP8_H__

#include "mmi4-parsedt.h"

#define	NAND_BLOCK_SIZE		SZ_128K
#define	NAND_DRIVER_NAME	"omap2-nand"
#define	NAND_CS			0

#define	ETH_GPIO_IRQ		130
#define	ETH_GPIO_FIFO		131
#define	ETH_DRIVER_NAME		"ax88796"
#define	ETH_CS			4
#define	ETH_CS_SIZE		SZ_16M

#define	AUDIO_MUTE_AMPLIFIER		174

#define	CAN_GPIO_IRQ		133
#define	CAN_CS			5

#define	MMC_GPIO_WP		126
#define	MMC_GPIO_CD		59

#define	USB_GPIO_RESET		27

#define	VIDEO_GPIO_BKL_POW     	140
#define	VIDEO_GPIO_DISP_POW    	141
#define	VIDEO_GPIO_BKL_BR			182
#define	VIDEO_DRIVER_NAME	"omapdss"

#define	TS_GPIO_IRQ		167

#define	I2C_GPIO_SDA		183
#define	I2C_GPIO_SCL		168

#define	BUZZER			132

#define MACH_TYPE_OMAP3_OFSP8_KERNEL400 6660

int g_tsc2007_gpio = 167;

/*union i2c_bus_platform_data;
struct tsc2007_platform_data ofsp8_400_tscdata;
struct qt2160_platform_data ofsp8_400_qt2160_info;
*/
/*union i2c_bus_platform_data {
	struct tsc2007_platform_data ofsp8_400_tscdata;
	struct qt2160_platform_data ofsp8_400_qt2160_info;
};
*/
int ofsp8_400_get_tspendown(void);


#endif /* __ARCH_ARM_BOARD_OFSP8_H__ */
