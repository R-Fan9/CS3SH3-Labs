obj-m += $(file).o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD)/$(file)/ modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD)/$(file)/ clean
