# External build helper for the sunxi DRM SPI output prototype.
# HDMI remains the vendor BSP driver and is not built by this Makefile.
#
# This builds the object for compile/API validation. Functional integration
# requires the matching BSP patch because the sunxi DRM master keeps its
# component-driver list in sunxi_drm_drv.c.

obj-m += sunxi_drm_spi.o

KDIR ?= /lib/modules/$(shell uname -r)/build
SUNXI_BSP ?= $(CURDIR)/../../bsp/drivers/drm

ccflags-y += -I$(SUNXI_BSP)
ccflags-y += -I$(SUNXI_BSP)/sunxi_device/hardware
ccflags-y += -I$(SUNXI_BSP)/sunxi_device/hardware/lowlevel_de
ccflags-y += -I$(SUNXI_BSP)/sunxi_device/hardware/lowlevel_lcd
ccflags-y += -I$(SUNXI_BSP)/phy
ccflags-y += -I$(SUNXI_BSP)/lib/inc
ccflags-y += -I$(CURDIR)/../../bsp/drivers/video/sunxi
ccflags-y += -I$(CURDIR)/../../bsp/include

all:
	$(MAKE) -C $(KDIR) M=$(CURDIR) modules

clean:
	$(MAKE) -C $(KDIR) M=$(CURDIR) clean
