# snd-soc-inmp441

InvenSense INMP441 I2S Codec Driver on Raspberry Pi。

INMP441是一个数字音频输入模块，引脚直接输出数字信号，所以只要简单实现编码器组件即可。

为了方便和数字输出模块同时使用，驱动作为一个 `simple-audio-card` 声卡组件

PS:

- 已经在Raspberry Pi Model B Rev 2测试通过

- 后续计划加入SD引脚模式和LED引脚支持

## 编译驱动

### 环境准备

这里指在树莓派上编译，不涉及跨平台编译部分

```sh
apt install build-essential git bc bison flex libssl-dev make
apt install raspberrypi-kernel-headers
```

### 编译

在 `snd-soc-inmp441.c `目录，执行命令 

````sh
make clean && make
````



### 安装

在 `snd-soc-inmp441.c `目录，执行命令 
```
make install
```

按照树莓派要求，将编译好的`snd-soc-inmp441.ko` 压缩成 `snd-soc-inmp441.ko.xz`,放在 /usr/lib/modules/$(shell uname -r)/kernel/sound/soc/codecs/ 目录下，并执行 depmod 构建 更新modules.alias, modules.dep 等，最后会显示modinfo，表示模块正常



## 编译设备树

### 环境准备

```
apt install device-tree-compiler
```

### 编译

#### 单独使用inmp441

```sh
dtc -@ -I dts -O dtb -o /boot/overlays/inmp441.dtbo inmp441-overlay.dts
```

#### 和max98357a一起使用

```sh
dtc -@ -I dts -O dtb -o /boot/overlays/inmp441-max98357a.dtbo inmp441-max98357a-overlay.dts
```

注意这里可能有警告提示，请忽略，raspberry官方源码也有这个警告

```sh
Warning (unit_address_vs_reg): /fragment@2/__overlay__/simple-audio-card,dai-link@0: node has a unit name, but no reg or ranges property
Warning (unit_address_vs_reg): /fragment@2/__overlay__/simple-audio-card,dai-link@1: node has a unit name, but no reg or ranges property
```

### 安装

修改 `/boot/firmware/config.txt`，添加以下内容

#### 单独使用inmp441

```txt
dtoverlay=inmp441
```

#### 和max98357a一起使用

```sh
dtoverlay=inmp441-max98357a
```

`reboot`重启生效

## 测试

1. 检查驱动存在 `/sys/devices/platform/soc/subsystem/drivers`

2. 检查设备存在 `/sys/devices/platform/soc/subsystem/devices`

3. 检查设备状态 `/proc/device-tree/inmp441/status`

4. 列举输入输出设备 `aplay -l`, `arecord -l`，都包含inmp441

5. 测试录音，注意arecord输出card 1，device 1, 参数使用 `hw:1,1`

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