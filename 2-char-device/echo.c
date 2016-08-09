#include <linux/init.h>
#include <linux/module.h>

#include <linux/fs.h>
#include <linux/miscdevice.h>

#include <linux/slab.h>

#include <asm/uaccess.h>

MODULE_LICENSE("Dual BSD/GPL");

#define ECHO_MAX_BUF_LENGTH 255

static struct {
	char *string;
	size_t length;
} echo_data;

static ssize_t echo_read(struct file* f, char __user *buf, size_t count,
		loff_t *off) {
	int len = echo_data.length;
	if (len == 0) {
		return 0;
	}

	if (count < len)
		return -EINVAL;
	if (*off != 0)
		return 0;

	if (copy_to_user(buf, echo_data.string, len))
		return -EINVAL;

	*off = len;

	return len;
}

static ssize_t echo_write(struct file *f, const char __user *buf, size_t size,
		loff_t *off) {
	if (size > ECHO_MAX_BUF_LENGTH) {
		return -EINVAL;
	}

	if (*off != 0)
		return 0;

	if (echo_data.string) {
		kfree(echo_data.string);
		echo_data.string = NULL;
	}

	echo_data.string = kmalloc(size, GFP_KERNEL);
	if (!echo_data.string) {
		return -ENOMEM;
	}

	if (copy_from_user(echo_data.string, buf, size)) {
		return -EINVAL;
	}
	echo_data.length = size;

	*off = size;
	return size;
}

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.read = echo_read,
	.write = echo_write,
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
	if (echo_data.string) {
		kfree(echo_data.string);
		echo_data.string = NULL;
	}
	misc_deregister(&miscdev);
	printk(KERN_DEBUG "Goodbye, cruel world\n");
}

module_init(hello_init);
module_exit(hello_exit);
