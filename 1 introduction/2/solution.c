#include <linux/module.h>
#include <linux/slab.h>
#include <linux/device.h>

#include "checker.h"

int init_mdl(void)
{
    call_me("Hello from my module!");

    return 0;
}

void cleanup_mdl(void)
{
    printk(KERN_INFO "Leaving");
}

module_init(init_mdl);
module_exit(cleanup_mdl);

MODULE_LICENSE("GPL");
