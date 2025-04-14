# snd-soc-inmp441

InvenSense INMP441 I2S Codec Driver for Raspberry Pi.

The INMP441 is a digital microphone module that outputs digital signals directly through its pins, so we only need to implement a simple codec component.

For easier integration with digital output modules, this driver is implemented as a `simple-audio-card` component.

Note:

- Tested and working on Raspberry Pi Model B Rev 2
- Future updates may include SD pin mode and LED pin support

## Building the Driver

### Prerequisites

We're building directly on the Raspberry Pi (no cross-compilation).

```sh
apt install build-essential git bc bison flex libssl-dev make
apt install raspberrypi-kernel-headers
```

### Compilation

In the directory containing `snd-soc-inmp441.c`, run:

````
make clean && make
````

### Installation

In the same directory, execute:

```
make install
```

This will:

1. Compress the compiled `snd-soc-inmp441.ko` into `snd-soc-inmp441.ko.xz`
2. Place it in `/usr/lib/modules/$(uname -r)/kernel/sound/soc/codecs/`
3. Run `depmod` to update module dependencies `modules.alias`, `modules.dep` 
4. Run `modinfo` output to confirm successful installation



## Device Tree Compilation

### Prerequisites

```
apt install device-tree-compiler
```

### Compilation Options

#### Standalone INMP441 Configuration

```sh
dtc -@ -I dts -O dtb -o /boot/overlays/inmp441.dtbo inmp441-overlay.dts
```

#### Combined INMP441 + MAX98357A Configuration

```sh
dtc -@ -I dts -O dtb -o /boot/overlays/inmp441-max98357a.dtbo inmp441-max98357a-overlay.dts
```

Note: You may see warnings like these (safe to ignore, same warnings appear in official Raspberry Pi sources):

```
Warning (unit_address_vs_reg): /fragment@2/__overlay__/simple-audio-card,dai-link@0: node has a unit name, but no reg or ranges property
Warning (unit_address_vs_reg): /fragment@2/__overlay__/simple-audio-card,dai-link@1: node has a unit name, but no reg or ranges property
```

### Installation

Edit `/boot/firmware/config.txt`:

#### Standalone Configuration

```txt
dtoverlay=inmp441
```

#### Combined Configuration

```
dtoverlay=inmp441-max98357a
```

Reboot to apply changes.

## Testing

1. Verify driver exists in `/sys/devices/platform/soc/subsystem/drivers`
2. Check device appears in `/sys/devices/platform/soc/subsystem/devices`
3. Verify device status: `/proc/device-tree/inmp441/status`
4. List audio devices: `aplay -l`, `arecord -l` (both should show inmp441)
5. Test recording (note: use `hw:1,1` as INMP441 typically appears as card 1, device 1)

Example test sequence:


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