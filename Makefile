
obj-m = snd-soc-inmp441.o

all:
	make -C /usr/lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /usr/lib/modules/$(shell uname -r)/build M=$(PWD) clean

install:
	xz -z snd-soc-inmp441.ko
	cp snd-soc-inmp441.ko.xz /usr/lib/modules/$(shell uname -r)/kernel/sound/soc/codecs/
	depmod
	modinfo snd-soc-inmp441