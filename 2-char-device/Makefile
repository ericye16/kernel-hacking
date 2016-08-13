obj-m += echo.o

KERNELDIR ?= /lib/modules/$(shell uname -r)/build

all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules EXTRA_CLAGS="-DDEBUG_OUTPUT"

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
