# If KERNELRELEASE is defined, we've been invoked from the 
# kernel build system and can use its language. 
obj-m := s5pv210_si4702.o 
s5pv210_si4702-objs := si4702_driver.o 

ifneq ($(KERNELRELEASE),) 
#KERNELDIR ?= /lib/modules/$(shell uname -r)/build 
KERNELDIR = $(KERNELRELEASE)
else 
KERNELDIR =  /home/zhangda/kernel/unsp210_linux_2.6.35
endif

$(PWD) := $(shell pwd)

default: 
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules 
	rm -rf *.o *.mod.c *.order *.symvers
clean:
	rm -rf *.ko



