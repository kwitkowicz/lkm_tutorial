#include <linux/init.h>
#include <linux/module.h>

static int __init dummy_init(void)
{
	return 0;
}

static void __exit dummy_exit(void)
{

}

module_init(dummy_init);
module_exit(dummy_exit);
