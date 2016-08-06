#include <linux/init.h>
#include <linux/module.h>

#include <linux/fs.h>
#include <linux/miscdevice.h>

#include <asm/uaccess.h>

MODULE_LICENSE("Dual BSD/GPL");

static ssize_t echo_read(struct file* f, char __user *buf, size_t count,
		loff_t *off) {
	char *hello_str = "Hi this is THE KERNEL speaking.\n";
	int len = strlen(hello_str);

	if (count < len)
		return -EINVAL;
	if (*off != 0)
		return 0;

	if (copy_to_user(buf, hello_str, len))
		return -EINVAL;

	*off = len;

	return len;
}

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.read = echo_read,
};

static struct miscdevice miscdev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "echo",
	.fops =	&fops,
};

static int hello_init(void)
{
	int ret;
	printk(KERN_DEBUG "Hello World!\n");
	ret = misc_register(&miscdev);
	if (ret) {
		printk(KERN_ERR "Unable to register echo misc device.\n");
	}
	return 0;
}

static void hello_exit(void)
{
	misc_deregister(&miscdev);
	printk(KERN_DEBUG "Goodbye, cruel world\n");
}

module_init(hello_init);
module_exit(hello_exit);
