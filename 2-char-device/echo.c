#include <linux/init.h>
#include <linux/module.h>
#include <linux/ctype.h>

#include <linux/fs.h>
#include <linux/miscdevice.h>

#include <linux/slab.h>

#include <asm/uaccess.h>
#include "echo_ioctls.h"

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

static long echo_clear(void) {
	if (echo_data.string) {
		kfree(echo_data.string);
		echo_data.string = NULL;
	}
	echo_data.length = 0;
	return 0;
}

static long echo_upper(void) {
	if (echo_data.string) {
		int i;
		for (i = 0; i < echo_data.length; i++) {
			echo_data.string[i] = toupper(echo_data.string[i]);
		}
	}
	return 0;
}

static long echo_lower(void) {
	if (echo_data.string) {
		int i;
		for (i = 0; i < echo_data.length; i++) {
			echo_data.string[i] = tolower(echo_data.string[i]);
		}
	}
	return 0;
}

static long ioctl_ops(struct file *filp, unsigned int cmd,
		unsigned long arg) {
	long retval = 0;
	printk(KERN_DEBUG "command is 0x%x", cmd);
	switch (cmd) {
		case ECHO_IOC_CLEAR:
			retval = echo_clear();
			break;
		case ECHO_IOC_LOWERCASE:
			retval = echo_lower();
			break;
		case ECHO_IOC_UPPERCASE:
			retval = echo_upper();
			break;
		case ECHO_IOC_REVERSE:
			break;
		default:
			printk(KERN_ERR "invalid command 0x%x", cmd);
			return -ENOTTY;
	}
	return retval;
}

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.read = echo_read,
	.write = echo_write,
	.unlocked_ioctl = &ioctl_ops,
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
