ifneq ($(KERNELRELEASE),)

obj-m += drm_mipi_dbi.o
obj-m += panel-mipi-dbi.o
obj-m += sunxi-pwm-child-pinctrl.o

else

ARCH ?= arm64
CROSS_COMPILE ?=
KDIR ?= /lib/modules/$(shell uname -r)/build

DTC ?= dtc
DTC_FLAGS ?= -@

DTSO := sun60i-a733-panel-mipi-dbi-spi.dtso
DTBO := $(DTSO:.dtso=.dtbo)

.PHONY: all modules overlay check-overlay check-kdir clean help

all: modules overlay

modules: check-kdir
	$(MAKE) -C "$(KDIR)" \
		M="$(CURDIR)" \
		ARCH="$(ARCH)" \
		CROSS_COMPILE="$(CROSS_COMPILE)" \
		modules

overlay: $(DTBO)

$(DTBO): $(DTSO)
	$(DTC) $(DTC_FLAGS) -I dts -O dtb -o "$@" "$<"

check-overlay: $(DTBO)
	$(DTC) -I dtb -O dts -o /dev/null "$(DTBO)"

check-kdir:
	@test -d "$(KDIR)" || { \
		echo "error: kernel build tree not found: $(KDIR)" >&2; \
		echo "usage: make KDIR=/path/to/linux-6.6 ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu-" >&2; \
		exit 1; \
	}

clean:
	@if test -d "$(KDIR)"; then \
		$(MAKE) -C "$(KDIR)" \
			M="$(CURDIR)" \
			ARCH="$(ARCH)" \
			CROSS_COMPILE="$(CROSS_COMPILE)" \
			clean; \
	fi
	$(RM) "$(DTBO)"

help:
	@echo "make modules KDIR=/path/to/linux-6.6 [CROSS_COMPILE=aarch64-linux-gnu-]"
	@echo "make overlay        Build $(DTBO)"
	@echo "make check-overlay  Build and decode-check the overlay"
	@echo "make all KDIR=/path/to/linux-6.6 [CROSS_COMPILE=aarch64-linux-gnu-]"
	@echo "make clean [KDIR=/path/to/linux-6.6]"

endif
