#kernel config and path
#use TARGET_KERNEL_DIR to decide the target kernel to build
ifeq ($(PLATFORM),ARM64)
export TARGET_KERNEL_DIR := $(TOP_DIR)/apdd/bsp/kernel/kernel_jetson/kernel/kernel-4.9
export TARGET_KERNEL_CONFIG := tegra_defconfig

#uboot config and path
export TARGET_BOOT_DIR :=
export TARGET_BOOT_CONFIG :=
endif
