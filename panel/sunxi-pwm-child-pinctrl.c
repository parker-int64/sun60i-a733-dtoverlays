// SPDX-License-Identifier: GPL-2.0+
/*
 * Minimal platform driver for Allwinner PWM child devices
 *
 * Workaround for BSP PWM driver pinctrl bug:
 *   sunxi_pwm_pin_set_state() calls devm_pinctrl_put() immediately after
 *   pinctrl_select_state(), which reverts the mux for devices that have
 *   no driver probe (like "allwinner,sunxi-pwm17").
 *
 * This module probes these PWM child devices, which causes the kernel's
 * pinctrl_bind_pins() to hold a pinctrl reference. Then when the BSP
 * driver does its buggy get/select/put cycle, the mux stays set.
 *
 * Compile on target:
 *   make -C /lib/modules/$(uname -r)/build M=$PWD modules
 *
 * Load:
 *   sudo insmod sunxi-pwm-child-pinctrl.ko
 *
 * Then enable PWM as usual:
 *   echo 1 > /sys/class/pwm/pwmchip10/pwm7/enable
 */

#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of.h>

static int sunxi_pwm_child_probe(struct platform_device *pdev)
{
	dev_info(&pdev->dev, "pinctrl holder probed\n");
	/* We do nothing - just probing here causes pinctrl_bind_pins()
	 * to be called, which holds the pinctrl reference.
	 */
	return 0;
}

static const struct of_device_id sunxi_pwm_child_match[] = {
	{ .compatible = "allwinner,sunxi-pwm0" },
	{ .compatible = "allwinner,sunxi-pwm1" },
	{ .compatible = "allwinner,sunxi-pwm2" },
	{ .compatible = "allwinner,sunxi-pwm3" },
	{ .compatible = "allwinner,sunxi-pwm4" },
	{ .compatible = "allwinner,sunxi-pwm5" },
	{ .compatible = "allwinner,sunxi-pwm6" },
	{ .compatible = "allwinner,sunxi-pwm7" },
	{ .compatible = "allwinner,sunxi-pwm8" },
	{ .compatible = "allwinner,sunxi-pwm9" },
	{ .compatible = "allwinner,sunxi-pwm10" },
	{ .compatible = "allwinner,sunxi-pwm11" },
	{ .compatible = "allwinner,sunxi-pwm12" },
	{ .compatible = "allwinner,sunxi-pwm13" },
	{ .compatible = "allwinner,sunxi-pwm14" },
	{ .compatible = "allwinner,sunxi-pwm15" },
	{ .compatible = "allwinner,sunxi-pwm16" },
	{ .compatible = "allwinner,sunxi-pwm17" },
	{ .compatible = "allwinner,sunxi-pwm18" },
	{ .compatible = "allwinner,sunxi-pwm19" },
	{ .compatible = "allwinner,sunxi-pwm20" },
	{ .compatible = "allwinner,sunxi-pwm21" },
	{ .compatible = "allwinner,sunxi-pwm22" },
	{ .compatible = "allwinner,sunxi-pwm23" },
	{ .compatible = "allwinner,sunxi-pwm24" },
	{ .compatible = "allwinner,sunxi-pwm25" },
	{ .compatible = "allwinner,sunxi-pwm26" },
	{ .compatible = "allwinner,sunxi-pwm27" },
	{ .compatible = "allwinner,sunxi-pwm28" },
	{ .compatible = "allwinner,sunxi-pwm29" },
	{ },
};
MODULE_DEVICE_TABLE(of, sunxi_pwm_child_match);

static struct platform_driver sunxi_pwm_child_driver = {
	.driver = {
		.name = "sunxi-pwm-child-pinctrl",
		.of_match_table = sunxi_pwm_child_match,
	},
	.probe = sunxi_pwm_child_probe,
};
module_platform_driver(sunxi_pwm_child_driver);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("PWM Backlight Workaround");
MODULE_DESCRIPTION("Hold pinctrl reference for Allwinner PWM child devices");
