#include <linux/module.h>

static int __init init_mdl(void) {
    printk(KERN_INFO "Hello, loading");
    return 0;
}

static void __exit cleanup_mdl(void) {
    printk(KERN_INFO "Leaving");
}

module_init (init_mdl);
module_exit (cleanup_mdl);
MODULE_LICENSE("GPL");
