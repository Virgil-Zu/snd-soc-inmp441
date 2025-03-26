// SPDX-License-Identifier: GPL-2.0-only
/*
 * I2S MEMS microphone driver for InvenSense INMP441
 * MEMS-based microphones.
 *
 * - Non configurable.
 * - I2S interface, 64 BCLs per frame, 32 bits per channel, 24 bit data
 * - test on Raspberry Pi Model B Rev 2
 *
 * Copyright (c) 2025 Virgil.Zu bestjade.net
 */
 
 #include <linux/module.h>
 #include <linux/platform_device.h>
 #include <sound/simple_card.h>
 #include <sound/pcm_params.h>
 
 
 static struct snd_soc_dai_driver inmp441_dai_driver = {
    .name = "INMP441",
    .capture = {
        .stream_name = "HiFi Capture",
        .formats = SNDRV_PCM_FMTBIT_S24_LE | SNDRV_PCM_FMTBIT_S32, // > 24bit
        .rates = SNDRV_PCM_RATE_8000_48000,
        .channels_min = 1,
        .channels_max = 2,
    },
 };
 
 static const struct snd_soc_component_driver inmp441_component_driver = {
    .idle_bias_on		= 1, // keep power
    .use_pmdown_time    = 1,
    .endianness		    = 1,
 };
 
 static int inmp441_platform_probe(struct platform_device *pdev)
 {
    return devm_snd_soc_register_component(&pdev->dev,
                        &inmp441_component_driver,
                        &inmp441_dai_driver,
                        1);
 }
 
 #ifdef CONFIG_OF
 static const struct of_device_id inmp441_device_id[] = {
    { .compatible = "invensense,inmp441" },  // match device tree compatible
    {},
 };
 MODULE_DEVICE_TABLE(of, inmp441_device_id); // aoto update modules.alias
 #endif
 
 static struct platform_driver inmp441_platform_driver = {
    .driver = {
        .name = "inmp441",
        .of_match_table = of_match_ptr(inmp441_device_id),
    },
    .probe = inmp441_platform_probe,
 };
 module_platform_driver(inmp441_platform_driver);
 
 MODULE_DESCRIPTION("InvenSense INMP441 I2S Codec Driver");
 MODULE_AUTHOR("Virgil.Zu <cn.zhuweijun@gmail.com>");
 MODULE_LICENSE("GPL v2");
 