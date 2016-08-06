#include <linux/init.h>
#include <linux/module.h>
// lifted from http://www.makelinux.net/ldd3/chp-2-sect-2
MODULE_LICENSE("Dual BSD/GPL");

static int hello_init(void)
{
    printk(KERN_DEBUG "Hello World!\n");
    return 0;
}

static void hello_exit(void)
{
    printk(KERN_DEBUG "Goodbye, cruel world\n");
}

module_init(hello_init);
module_exit(hello_exit);
