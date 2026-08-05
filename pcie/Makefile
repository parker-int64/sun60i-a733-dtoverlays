DTC ?= dtc
DTC_FLAGS ?= -@

DTSO := sun60i-a733-pcie-power.dtso
DTBO := $(DTSO:.dtso=.dtbo)

.PHONY: all overlay check-overlay clean help

all: overlay

overlay: $(DTBO)

$(DTBO): $(DTSO)
	$(DTC) $(DTC_FLAGS) -I dts -O dtb -o "$@" "$<"

check-overlay: $(DTBO)
	$(DTC) -I dtb -O dts -o /dev/null "$(DTBO)"

clean:
	$(RM) "$(DTBO)"

help:
	@echo "make overlay        Build $(DTBO)"
	@echo "make check-overlay  Build and decode-check the overlay"
	@echo "make clean          Remove the generated overlay"
