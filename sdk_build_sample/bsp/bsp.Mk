#kernel config and path
#use TARGET_KERNEL_DIR to decide the target kernel to build
#ifeq ($(PLATFORM),ARM64)
export TARGET_KERNEL_DIR := $(TOP_DIR)/bsp/kernel
export TARGET_KERNEL_CONFIG := tegra_defconfig
#uboot config and path
export TARGET_UBOOT_DIR := $(TOP_DIR)/bsp/uboot
export TARGET_UBOOT_CONFIG := p2371-2180_defconfig

export BSP_MODULES += kernel
export BSP_MODULES += uboot
#endif
