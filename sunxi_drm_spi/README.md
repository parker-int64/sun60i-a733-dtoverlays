# sunxi DRM SPI/DBI output prototype

This is an initial vendor-BSP integration for an SPI MIPI-DBI panel. It is
intentionally different from upstream `panel-mipi-dbi`: it does not register a
second DRM card or a second fbdev. The SPI output is a component of the vendor
`sunxi-drm` master, so the Display Engine owns the KMS state and the PVR
display path can continue to use the vendor framebuffer.

The target board uses only the vendor HDMI output and this SPI output. The
vendor `sunxi_drm_hdmi.c` is intentionally not copied or replaced here; HDMI
continues to be built by the BSP's existing `CONFIG_AW_DRM_HDMI_TX` path. The
integration patch only adds the SPI component and the common CRTC callback it
needs, so it does not alter DSI/RGB/LVDS/EDP drivers.

The current implementation is a bring-up prototype:

- one SPI/DBI output component;
- one fixed mode from `panel-timing`;
- RGB565, XRGB8888 and ARGB8888 primary-plane input;
- full-frame CPU conversion into a DMA-safe buffer;
- MIPI DCS window setup and `WRITE_MEMORY_START` transfer;
- controller DMA is requested through the normal SPI API (`spi_sync` inside
  the kernel MIPI-DBI helper); whether the SoC SPI controller uses DMA depends
  on its driver and transfer size;
- no independent `panel-mipi-dbi` DRM card and no independent fbdev.

The prototype intentionally does not claim full multi-plane composition. The
first version takes the committed primary plane. A production version should
use the existing sunxi writeback connector when overlays/cursor need to be
flattened before SPI transfer, and should move the transfer to an asynchronous
DMA worker with a page-flip fence.

## BSP integration

Apply `sunxi_drm_spi-bsp.patch` at the kernel tree root. Enable:

```text
CONFIG_AW_DRM=y
CONFIG_AW_DRM_DE=y
CONFIG_AW_DRM_TCON=y
CONFIG_AW_DRM_SPI=y
CONFIG_AW_DRM_HDMI_TX=y
CONFIG_DRM_MIPI_DBI=y
CONFIG_SPI=y
```

The standalone Makefile builds the SPI object for API/header validation. An
external module alone cannot join the already-built `sunxi-drm` component
master: the master's component match list is compiled into
`sunxi_drm_drv.c`. Apply the BSP patch and rebuild the vendor DRM module (or
the full kernel) for functional use. Do not build a second copy of the vendor
HDMI driver as an external module; that would register duplicate HDMI
platform-driver symbols.

## Device-tree shape

The SPI output platform node references an already-created SPI child and the
TCON used to select a CRTC. The SPI child must not be bound by
`panel-mipi-dbi` at the same time.

```dts
&spi3 {
	status = "okay";
	spi-max-frequency = <80000000>;

	lcd_spi: display@0 {
		reg = <0>;
		compatible = "sunxi,spi-dbi-device";
		spi-max-frequency = <80000000>;
		spi-cpol;
		spi-cpha;
	};
};

sunxi_spi_lcd: sunxi-drm-spi {
	compatible = "allwinner,sunxi-drm-spi";
	spi = <&lcd_spi>;
	tcon = <&tcon_lcd0>;
	tcon-id = <0>;
	reset-gpios = <&pio 0 0 GPIO_ACTIVE_LOW>;
	dc-gpios = <&pio 0 1 GPIO_ACTIVE_HIGH>;
	power-supply = <&reg_lcd_power>;
	io-supply = <&reg_lcd_io>;

	panel-timing {
		clock-frequency = <9000000>;
		hactive = <320>;
		vactive = <240>;
		hfront-porch = <10>;
		hback-porch = <10>;
		hsync-len = <10>;
		vfront-porch = <10>;
		vback-porch = <10>;
		vsync-len = <10>;
	};

	/* command, parameter-count, parameters; 0x00/1 means delay (ms) */
	init-sequence = /bits/ 8 <0x11 0x00 0x00 0x01 0x78 0x29 0x00>;
};
```

The phandle names and GPIOs are examples only. Use the exact TCON node and
regulators from the board DTS and the panel controller's real initialization
sequence. Remove/disable the old `panel-mipi-dbi-spi` node before enabling this
one, otherwise two drivers will compete for the same SPI device.

## Debugging

Useful messages are limited to probe/bind/enable failures and rate-limited
flush/format failures:

```sh
dmesg -w | grep -E 'sunxi-drm-spi|SUNXI.*SPI|sunxi.*drm'
cat /sys/class/drm/card*/name
modetest -D /dev/dri/cardX -c -p
```
