#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/of_platform.h>
#include <linux/of_address.h>
    

static struct of_device_id match_table[] = {
     {
             .compatible = "ax88796_dt",
     },
     {0}
};
static int ax88796_dt_probe(struct platform_device *dev)
{
//        struct uio_info *uioinfo;
        struct resource *r = &dev->resource[0];
        
//        uioinfo->name = dev->dev.of_node->name /* name from device tree: "my_device" */
//        uioinfo->mem[0].addr = r->start; /* device address from device tree */
//        uioinfo->mem[0].size = resource_size(r); /* size from device tree */
        printk(KERN_ERR "moep!\n");
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
module_param_string(of_id, uio_of_genirq_match[0].compatible, 128, 0);
MODULE_PARM_DESC(of_id, "Openfirmware id of the device to be handled by uio");

module_platform_driver(ax88796_dt_platform_driver);
MODULE_AUTHOR("Marian Ulbricht");
MODULE_DESCRIPTION("ax88796 devicetree wrapper");
MODULE_LICENSE("GPL v2");
MODULE_ALIAS("platform:" "ax88796_dt");
