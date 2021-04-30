#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/of_platform.h>
#include <linux/of_address.h>
#include <net/ax88796.h>
#include <linux/platform_data/gpmc-omap.h>
#include <linux/omap-gpmc.h>

#define	ETH_GPIO_IRQ		130
#define	ETH_GPIO_FIFO		131
#define	ETH_DRIVER_NAME		"ax88796"
#define	ETH_CS			4
#define	ETH_CS_SIZE		SZ_16M  
struct mmi4_eth_data{
	int irq_gpio;
	int fifo_gpio;
	int cs;
};

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

static struct of_device_id match_table[] = {
     {
             .compatible = "ax88796_dt",
     },
     {0}
};
int fill_eth_platform_device(struct platform_device *eth_plat_dev, struct mmi4_eth_data *eth_data) {
	eth_data->irq_gpio=130;
	eth_data->cs=4;
	eth_data->fifo_gpio=0;
	return 0;

}
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
struct mmi4_eth_data eth_data = {
	.irq_gpio = ETH_GPIO_IRQ,
	.fifo_gpio = ETH_GPIO_FIFO,
	.cs = ETH_CS,
};

void __init ofsp8_400_eth_init(void)
{
	int err;
	unsigned long cs_mem_base;
	if(fill_eth_platform_device(&ofsp8_400_eth_device, &eth_data))
		return;

	/* ax88796 driver does not support clk-framework, so clk_get
	  and clk_enable should be called here. But as this is
	  called before clk-initialization this is not possible.
	  So we relay on other hardware to keep l3_clk alive.
	 */

	err = gpio_request(eth_data.irq_gpio, ETH_DRIVER_NAME " irq");
	if (err) {
		pr_err("%s: Failed to request GPIO%d for IRQ\n", ofsp8_400_eth_device.name,
				eth_data.irq_gpio);
		goto err_req_gpio_irq;
	}
	gpio_direction_input(eth_data.irq_gpio);
        if ( eth_data.fifo_gpio > 0 )
	        err = gpio_request(eth_data.fifo_gpio, ETH_DRIVER_NAME " fifo");
        else
                pr_notice ("Ethernet FIFO GPIO is not defined\n");
	if (err) {
		pr_err("%s: Failed to request GPIO%d for "
				"FIFO-select\n", ofsp8_400_eth_device.name,
				eth_data.fifo_gpio);
		goto err_req_gpio_fifo;
	}
        if ( eth_data.fifo_gpio > 0 )
	        gpio_direction_output(eth_data.fifo_gpio, 0);

	ofsp8_400_eth_device.resource[2].start = gpio_to_irq(eth_data.irq_gpio);
	ofsp8_400_eth_device.resource[2].end =	gpio_to_irq(eth_data.irq_gpio);

	gpmc_cs_request(eth_data.cs, SZ_16M, &cs_mem_base);

	ofsp8_400_eth_device.resource[0].start = cs_mem_base;
	ofsp8_400_eth_device.resource[0].end = cs_mem_base + (0x18 << 1) - 1;
	ofsp8_400_eth_device.resource[1].start = cs_mem_base + (0x1f << 1);
	ofsp8_400_eth_device.resource[1].end = cs_mem_base + (0x20 << 1) - 1;

	err = platform_device_register(&ofsp8_400_eth_device);
	if (err) {
		pr_err("%s: Failed to add platform device\n", ETH_DRIVER_NAME);
		goto err_pdev;
	}

	return;

err_pdev:
	gpio_free(eth_data.fifo_gpio);
err_req_gpio_fifo:
	gpio_free(eth_data.irq_gpio);
err_req_gpio_irq:
	return;
}
static int ax88796_dt_probe(struct platform_device *dev)
{
//        struct uio_info *uioinfo;
        struct resource *r = &dev->resource[0];
        
//        uioinfo->name = dev->dev.of_node->name /* name from device tree: "my_device" */
//        uioinfo->mem[0].addr = r->start; /* device address from device tree */
//        uioinfo->mem[0].size = resource_size(r); /* size from device tree */
        printk(KERN_ERR "moep!\n");
        printk(KERN_ERR "name:%s addr:0x%llx size:%i\n",dev->dev.of_node->name,r->start,resource_size(r));
        ofsp8_400_eth_init();
     return 0;
}

static int ax88796_dt_remove(struct platform_device *dev){


return 0;
}
        
        
        
static struct platform_driver ax88796_dt_platform_driver = {
        .probe = ax88796_dt_probe,
        .remove = ax88796_dt_remove,
        .driver = {
                .name = "ax88796_dt",
                .owner = THIS_MODULE,
                .of_match_table = of_match_ptr(match_table),
        },
};


MODULE_DEVICE_TABLE(of, match_table);
module_param_string(of_id, match_table[0].compatible, 128, 0);
MODULE_PARM_DESC(of_id, "Openfirmware id of the device to be handled by uio");

module_platform_driver(ax88796_dt_platform_driver);
MODULE_AUTHOR("Marian Ulbricht");
MODULE_DESCRIPTION("ax88796 devicetree wrapper");
MODULE_LICENSE("GPL v2");
MODULE_ALIAS("platform:" "ax88796_dt");
