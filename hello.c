#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Krzysiek Witkowicz");
MODULE_DESCRIPTION("Hello World from the linux kernel module");
MODULE_VERSION("1.0");

static int __init hello_init(void)
{
	   printk(KERN_INFO "Hello World from the first Kernel Module!\n");
	   return 0;
}

static void __exit hello_exit(void)
{
	   printk(KERN_INFO "Goodbye from the Kernel Module!\n");
}

module_init(hello_init);
module_exit(hello_exit);
