obj-m += blc_dev.o

blc_dev-m += blc_main.o
blc_dev-m += request.o

PWD := $(CURDIR)

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) W=no-compiler-check modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) W=no-compiler-check clean
