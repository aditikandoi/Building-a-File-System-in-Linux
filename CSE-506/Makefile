# INC=/lib/modules/$(shell uname -r)/build/arch/x86/include

# stbfsctl: stbfsctl.c
# 	gcc -Wall -I$(INC)/generated/uapi -I$(INC)/uapi stbfsctl.c -o stbfsctl

# clean:
# 	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
# 	rm -f stbfsctl
obj-m += async_ops_module.o

all: async_ops_module_tag async_ops_tag

async_ops_module_tag:
	make -Wall -Werror -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules

async_ops_tag:
	gcc -Wall -Werror async_ops.c -o async_ops

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean
	rm -f async_ops
