# snd-soc-inmp441

InvenSense INMP441 I2S Codec Driver on Raspberry Pi。

The INMP441 is a digital audio input module that outputs digital signals directly through its pins, requiring only a simple codec component implementation.

To facilitate simultaneous use with digital output modules, the driver is implemented as `simple-audio-card` component.

PS:

- Successfully tested on Raspberry Pi Model B Rev 2
- Future plans include adding support for SD pin mode and LED pins

## Compiling the Driver

### Environment 

This refers to compilation on the Raspberry Pi itself, excluding cross-platform compilation

```sh
apt install build-essential bc bison flex libssl-dev make
apt install raspberrypi-kernel-headers
```

### Compile

In the directory containing `snd-soc-inmp441.c`, execute the command:

````
make clean && make
````



### Installation

In the source directory , execute the command:
```
make install
```

Following Raspberry Pi requirements, the compiled `snd-soc-inmp441.ko` will be compressed into `snd-soc-inmp441.ko.xz` and placed in the `/usr/lib/modules/$(shell uname -r)/kernel/sound/soc/codecs/` directory. The `depmod` command will then be executed to update `modules.alias`, `modules.dep`, etc. Finally, `modinfo` will be displayed to confirm the module is functioning correctly.



## Compiling the Device Tree

### Environment 

```
apt install device-tree-compiler
```

### Compile

#### Using INMP441 Alone

```sh
dtc -@ -I dts -O dtb -o /boot/overlays/inmp441.dtbo inmp441-overlay.dts
```

#### Using INMP441 with MAX98357A

```sh
dtc -@ -I dts -O dtb -o /boot/overlays/inmp441-max98357a.dtbo inmp441-max98357a-overlay.dts
```

Note: Warnings may appear here, which can be safely ignored (the official Raspberry Pi source code also includes these warnings):

```
Warning (unit_address_vs_reg): /fragment@2/__overlay__/simple-audio-card,dai-link@0: node has a unit name, but no reg or ranges property
Warning (unit_address_vs_reg): /fragment@2/__overlay__/simple-audio-card,dai-link@1: node has a unit name, but no reg or ranges property
```

### Installation

Modify `/boot/firmware/config.txt` and add the following content:

#### Using INMP441 Alone

```txt
dtoverlay=inmp441
```

#### Using INMP441 with MAX98357A

```
dtoverlay=inmp441-max98357a
```

`reboot`Reboot to apply changes:

## Testing

1. Check driver existence `/sys/devices/platform/soc/subsystem/drivers`

2. Check device existence `/sys/devices/platform/soc/subsystem/devices`

3. Check device status `/proc/device-tree/inmp441/status`

4. List input/output devices `aplay -l`, `arecord -l`，(both should include INMP441)

5. Test recording (note: arecord outputs card 1, device 1, use parameters `hw:1,1`):


```sh
~# ls /sys/devices/platform/soc/subsystem/drivers | grep inmp441
inmp441
~# ls /sys/devices/platform/soc/subsystem/devices | grep inmp441
inmp441
~# cat /proc/device-tree/inmp441/status
okay
~# aplay -l
**** List of PLAYBACK Hardware Devices ****
card 0: Headphones [bcm2835 Headphones], device 0: bcm2835 Headphones [bcm2835 Headphones]
  Subdevices: 8/8
  Subdevice #0: subdevice #0
  Subdevice #1: subdevice #1
  Subdevice #2: subdevice #2
  Subdevice #3: subdevice #3
  Subdevice #4: subdevice #4
  Subdevice #5: subdevice #5
  Subdevice #6: subdevice #6
  Subdevice #7: subdevice #7
card 1: INMP441MAX98357 [INMP441-MAX98357A], device 0: 20203000.i2s-HiFi HiFi-0 [20203000.i2s-HiFi HiFi-0]
  Subdevices: 1/1
  Subdevice #0: subdevice #0
card 2: vc4hdmi [vc4-hdmi], device 0: MAI PCM i2s-hifi-0 [MAI PCM i2s-hifi-0]
  Subdevices: 1/1
  Subdevice #0: subdevice #0
~# arecord -l
**** List of CAPTURE Hardware Devices ****
card 1: INMP441MAX98357 [INMP441-MAX98357A], device 1: 20203000.i2s-INMP441 INMP441-1 [20203000.i2s-INMP441 INMP441-1]
  Subdevices: 1/1
  Subdevice #0: subdevice #0
~# arecord -D hw:1,1 -f S32_LE -r 16000 -c 2 -d 5 test.wav
Recording WAVE 'test.wav' : Signed 32 bit Little Endian, Rate 16000 Hz, Stereo
~#
```