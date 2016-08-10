#ifndef ECHO_IOCTLS_H
#define ECHO_IOCTLS_H

#include <linux/ioctl.h>

// use 'e' as magic number
#define ECHO_IOC_MAGIC	'e'

#define ECHO_IOC_CLEAR	_IO(ECHO_IOC_MAGIC, 0)
#define ECHO_IOC_LOWERCASE	_IO(ECHO_IOC_MAGIC, 1)
#define ECHO_IOC_UPPERCASE	_IO(ECHO_IOC_MAGIC, 2)
#define ECHO_IOC_REVERSE	_IO(ECHO_IOC_MAGIC, 3)

#endif
