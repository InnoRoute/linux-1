#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/of_platform.h>
#include <linux/of_address.h>
#include <linux/platform_data/gpmc-omap.h>
#include <linux/omap-gpmc.h>
#include <linux/gpio.h>










void max6373_init(void)
{
	int err;
	unsigned long cs_mem_base;
//	if(fill_platform_device(&max6373_device, &eth_data))
//		return;



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
static int max6373_dt_probe(struct platform_device *dev)
{
//        struct uio_info *uioinfo;
        struct resource *r = &dev->resource[0];
        
//        uioinfo->name = dev->dev.of_node->name /* name from device tree: "my_device" */
//        uioinfo->mem[0].addr = r->start; /* device address from device tree */
//        uioinfo->mem[0].size = resource_size(r); /* size from device tree */
        printk(KERN_ERR "init max6373 by devicetree\n");
        printk(KERN_ERR "name:%s addr:0x%llx size:%i\n",dev->dev.of_node->name,r->start,resource_size(r));
        //ofsp8_400_eth_init();
     return 0;
}

static int max6373_dt_remove(struct platform_device *dev){


return 0;
}
        
        
        
static struct platform_driver ax88796_dt_platform_driver = {
        .probe = max6373_dt_probe,
        .remove = max6373_dt_remove,
        .driver = {
                .name = "max6373_dt",
                .owner = THIS_MODULE,
                .of_match_table = of_match_ptr(match_table),
        },
};


MODULE_DEVICE_TABLE(of, match_table);
module_param_string(of_id, match_table[0].compatible, 128, 0);
MODULE_PARM_DESC(of_id, "tbd");

module_platform_driver(ax88796_dt_platform_driver);
MODULE_AUTHOR("Marian Ulbricht");
MODULE_DESCRIPTION("max6373 devicetree wrapper");
MODULE_LICENSE("GPL v2");
MODULE_ALIAS("platform:" "max6373_dt");
