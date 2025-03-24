# snd-soc-inmp441
InvenSense INMP441 I2S Codec Driver on Raspberry Pi

`dtc -@ -I dts -O dtb -o /boot/overlays/inmp441.dtbo inmp441-overlay.dts`

or 

`dtc -@ -I dts -O dtb -o /boot/overlays/inmp441-max98357a.dtbo inmp441-max98357a-overlay.dts`

you can use `ics43432`