#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Krzysiek Witkowicz");
MODULE_DESCRIPTION("Passing command line arguments to LKM");
MODULE_VERSION("1.0");

static char * string = "How are you?";
static int integer = 0;
static short int strange_short = 666;
static long int my_array[5] = {};
static int array_num = 0;

module_param(string, charp, 0000 );
MODULE_PARM_DESC(string, "My string");
module_param(integer, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(integer, "My integer");
module_param_named(normal_short, strange_short, short, 0644);
MODULE_PARM_DESC(strange_short, "My named parameter");
module_param_array(my_array, long, &array_num, 0000);
MODULE_PARM_DESC(my_array, "My array of longs");

static int __init cli_init(void)
{
	   int i = 0;
	   printk(KERN_INFO "My parameters are: \n");
	   printk(KERN_INFO "string %s\n", string);
	   printk(KERN_INFO "integer %d\n",integer);
	   printk(KERN_INFO "short %hd\n",strange_short);
	   printk(KERN_INFO "table got %d elements and contains:\n",array_num);
	   for (i = 0; i < (sizeof (my_array) / sizeof (long int)); i++)
	   {
		   printk(KERN_INFO "\telement[%d] = %ld\n", i, my_array[i]);
					
	   }
	   return 0;
}

static void __exit cli_exit(void)
{
	   printk(KERN_INFO "Goodbye from the Kernel Module!\n");
}

module_init(cli_init);
module_exit(cli_exit);
