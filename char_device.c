#include <linux/device.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Krzysiek Witkowicz");
MODULE_DESCRIPTION("Example of character device driver");
MODULE_VERSION("1.0");

#define MY_DEVICE_NAME "mychar"
#define MY_DEVICE_CLASS "mychar_class"

static int major_number;
static struct class* mychar_dev_class = NULL;
static struct device* mychar_dev = NULL;
static char msg[256] = {0};
static short msg_length;

static int     mychar_dev_open(struct inode *, struct file *);
static ssize_t mychar_dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t mychar_dev_write(struct file *, const char *, size_t, loff_t *);
static int     mychar_dev_release(struct inode *, struct file *);

static struct file_operations fo = {
	.open = mychar_dev_open,
	.read = mychar_dev_read,
	.write = mychar_dev_write,
	.release = mychar_dev_release,
};

static int __init mychar_dev_init(void){
	printk(KERN_INFO "mychar_dev: initializing \n");
	major_number = register_chrdev(0, MY_DEVICE_NAME, &fo);
	if(major_number < 0){
		printk(KERN_ALERT "mychar_dev: problem with register my char device \n");
		return major_number;
	}
	printk(KERN_INFO "mychar_dev: device registered, major number is %d \n", major_number);
	
	mychar_dev_class = class_create(THIS_MODULE, MY_DEVICE_CLASS);
	if(IS_ERR(mychar_dev_class)){
		unregister_chrdev(major_number, MY_DEVICE_NAME);
		printk(KERN_ALERT "mychar_dev: fail while register device class");
		return PTR_ERR(mychar_dev_class);

	}
	printk(KERN_INFO "mychar_dev: class created\n");

	mychar_dev = device_create(mychar_dev_class, NULL, MKDEV(major_number, 0), NULL, MY_DEVICE_NAME);
	if (IS_ERR(mychar_dev)){  
		class_destroy(mychar_dev_class);
		unregister_chrdev(major_number, MY_DEVICE_NAME);
		printk(KERN_ALERT "Failed to create the device\n");
		return PTR_ERR(mychar_dev);
	}
	printk(KERN_INFO "mychar_dev: device created\n");

	return 0;

}

static int mychar_dev_open(struct inode *inode, struct file *file){
	printk(KERN_INFO "mychar_dev: device opened\n");
	return 0;
}

static int mychar_dev_release(struct inode *inode, struct file *file){
	printk(KERN_INFO "mychar_dev: device closed\n");
	return 0;
}

static ssize_t mychar_dev_read(struct file * filep, char * buff, size_t buff_len, loff_t * offset){
	long ret = 0;
	ret = copy_to_user(buff, msg, msg_length);
	if(ret==0){
		printk(KERN_INFO "mychar_dev: %d bytes send to client\n",msg_length);
		return(msg_length=0);
	}
	else{
		printk(KERN_ALERT "mychar_dev: failed while send data\n");
		return -EFAULT;
	}
}

static ssize_t mychar_dev_write(struct file * filep, const char * buff, size_t buff_len, loff_t * offset){
	sprintf(msg, "%s",buff);
	msg_length = strlen(msg);
	printk(KERN_INFO "mychar_dev: Received %d characters from client\n",msg_length);
	return buff_len;

}

static void __exit mychar_dev_exit(void){
	device_destroy(mychar_dev_class, MKDEV(major_number, 0));
	class_unregister(mychar_dev_class);
	class_destroy(mychar_dev_class);
	unregister_chrdev(major_number, MY_DEVICE_NAME);
	printk(KERN_INFO "Goodbye from the mychar\n");
}

module_init(mychar_dev_init);
module_exit(mychar_dev_exit);
