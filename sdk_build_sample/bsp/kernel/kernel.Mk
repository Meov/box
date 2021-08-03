LACAL_PATH :=$(call local-dir)

#judge wether we use this kernel
ifeq ($(TARGET_KERNEL_DIR),$(LACAL_PATH))
#clear all the FLAGS
include $(CLEAR_MFILE)

LOCAL_FILE := $(LACAL_PATH)/arch/arm64/boot/zImage

TARGET_FILE := $(KERNEL_TARGET_OUT_DIR)/arch/arm64/boot/zImage

ARGS := ARCH=arm64 O=$(KERNEL_TARGET_OUT_DIR) LOCALVERSION=-tegra CROSS_COMPILE=$(CROSS_COMPILE)

MAKE_CMD := make -C $(TARGET_KERNEL_DIR) $(ARGS)

.PHONY: kernel
kernel:kernel.install

.PHONY: kernel.install
kernel.install : kernel.img
	$(silent) -rm -rf $(KERNEL_TARGET_OUT_DIR)/kernel

.PHONY : kernel.img
kernel.img:$(TARGET_FILE)
$(TARGET_FILE):zImage.clean kernel.config
	$(silent) -rm -rf $(TARGET_FILE)
	$(silent) $(MAKE_CMD) zImage
	$(silent) -rm -rf $(KERNEL_TARGET_OUT_DIR)/arch/arm64/boot/*.dtbs
	$(silent) $(MAKE_CMD) dtbs
#	$(silent) cp -prf $(LACAL_PATH)/arch/arm64/boot/*.dtbs $(KERNEL_TARGET_OUT_DIR)


.PHONY: zImage.clean
zImage.clean:
	$(silent) echo "Kernel rm $(TARGET_FILE)..."
	$(silent) rm -f $(TARGET_FILE)
#	$(silent) rm -f $(LOCAL_PATH)/arch/arm64/boot/zImage

.PHONY : kernel.config
kernel.config : $(TARGET_KERNEL_CONFIG)/.config
#make defconfig
$(TARGET_KERNEL_CONFIG)/.config:$(LACAL_PATH)/arch/arm64/configs/$(TARGET_KERNEL_CONFIG)
	$(silent) echo "Configure kernel with $(TARGET_KERNEL_CONFIG)..."
	$(silent) $(MAKE_CMD) $(TARGET_KERNEL_CONFIG)

.PHONY : kernel.make
kernel.make:
	@echo "Building kernel..."
	$(silent) $(MAKE_CMD)

.PHONY : kernel.clean
kernel.clean:
	@echo "Cleaning kernel..."
	$(silent) $(MAKE_CMD) clean
.PHONY : kernel.distclean
kernel.distclean:
	@echo "Distcleaning kernel..."
	$(silent) $(MAKE_CMD) distclean

.PHONY:kernel.menuconfig
kernel.menuconfig:
	@echo "kernel menuconfig..."
	$(silent) $(MAKE_CMD) menuconfig

.PHONY:dtbs
dtbs:
	@echo "make dtbs..."
	$(silent) -rm -rf $(KERNEL_TARGET_OUT_DIR)/arch/arm/boot/*.dtbs
	$(silent) $(MAKE_CMD) dtbs
	#$(silent) cp -prf $(LACAL_PATH)/arch/arm/boot/*.dtbs $(KERNEL_TARGET_OUT_DIR)
endif
