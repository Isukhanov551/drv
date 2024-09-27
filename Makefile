obj-m += simplechar.o

PWD := $(CURDIR)

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) W=no-compiler-check modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) W=no-compiler-check clean