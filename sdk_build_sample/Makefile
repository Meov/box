SHELL = /bin/bash
export SDK_VERSION := 0.0.1
export TOP_DIR=$(shell pwd 2>/dev/null)
export START_DATE := $(shell date +%Y_%m_%d_%T 2>/dev/null | awk '{gsub(":","_",$$1);print $$1}' 2>/dev/null)

OS_TYPE := $(shell uname -m 2>/dev/null)

#echo color set reference https://www.cnblogs.com/wlzy/p/5933853.html
help:
	@echo -e "\e[32;1m"======================================= HELP INFO =======================================
	@echo SDK version : $(SDK_VERSION)
	@echo Using: "make app" to build apps
	@echo Using: "make kernel" to build kernel
	@echo ======================================= HELP  END ===========================================
	@echo -e "\e[33;0m"  #cancel color properties

#cross compile tools chose start
#NOTE:
#if building "embeded apps, kernel rootfs or uboot" cross compile SHOULD BE set here

#e.g. "gcc-linaro-7.3.1-2018.05-x86_64_aarch64-linux-gnu" is used in my project
#cd build_tools/build_chains
#wget http://releases.linaro.org/components/toolchain/binaries/7.3-2018.05/aarch64-linux-gnu/gcc-linaro-7.3.1-2018.05-x86_64_aarch64-linux-gnu.tar.xz
#to down load cross compiler

#change this to get your own cross compiler
BUILD_TOOL_CHAINS :=$(TOP_DIR)/build_tools/build_chains/gcc-linaro-7.3.1-2018.05-x86_64_aarch64-linux-gnu
exist = $(shell if [ -d $(BUILD_TOOL_CHAINS) ]; then echo "exist"; else echo "notexist"; fi;)
$(info $(exist) $(BUILD_TOOL_CHAINS))

ifeq ($(strip $(exist)), exist)
export CROSS_COMPILE := $(BUILD_TOOL_CHAINS)/bin/aarch64-linux-gnu-
else 
#$(error CROSS_COMPILER  NOT FOUND in $(TOP_DIR)/build/tool_chains)
$(info CROSS COMPILER NOT FOUND DEFUALT "GCC" IS USING)
endif

ifneq ($(CROSS_COMPILE), )
$(info Using cross compiler: $(CROSS_COMPILE))
endif

ifneq ($(strip $(OS_TYPE)), x86_64)
$(error unsupported compile platform! only x86_64 supportted!)
endif

#cross compile tools chose end

# build app defualt. modify this using "make TARGET=BSP" in "TOP_DIR"
export TARGET := APP 

#==================================== app path define  ==========================================
# path of make file scripts 
export MFILES_DIR := $(TOP_DIR)/build_tools/makefile_scripts

# path of SDK output 
export OUT_DIR := $(TOP_DIR)/out

# path of apps output
export APP_OUT_DIR := $(OUT_DIR)/app_out
OUT_DIRS_LIST += $(APP_OUT_DIR)

# path of all the apps executive files
export TARGET_APP_OUT := $(APP_OUT_DIR)/apps_target
OUT_DIRS_LIST += $(TARGET_APP_OUT)

# path of all the .o files of apps
export TARGET_APP_DO_DIR := $(APP_OUT_DIR)/obj
OUT_DIRS_LIST += $(TARGET_APP_DO_DIR)

# path of shared libs apps dependent on
export TARGET_COMMON_APP_LDLIBS_DIR := $(APP_OUT_DIR)/obj/libs
OUT_DIRS_LIST += $(TARGET_COMMON_APP_LDLIBS_DIR)

#================================== app path define end =========================================
#==================================== bsp path define  ==========================================
# path of bsp output
export BSP_OUT_DIR := $(OUT_DIR)/bsp_out
OUT_DIRS_LIST += $(BSP_OUT_DIR)

# path of bsp executive files (kernel uboot and rootfs)
export TARGET_BSP_OUT := $(BSP_OUT_DIR)/bsp_target
OUT_DIRS_LIST += $(TARGET_BSP_OUT)

# path of kernel image
export KERNEL_TARGET_OUT_DIR := $(TARGET_BSP_OUT)/kernel
OUT_DIRS_LIST +=$(KERNEL_TARGET_OUT_DIR)

# path of .o files MAY BE UNUSFUL in kernel building
export TARGET_BSP_DO_DIR := $(BSP_OUT_DIR)/obj
OUT_DIRS_LIST += $(TARGET_BSP_DO_DIR)

# path of libs kernel dependen UNUSFUL in kernel building kernel does not relay on any libraries!
export TARGET_COMMON_BSP_LDLIBS_DIR := $(BSP_OUT_DIR)/obj/libs
OUT_DIRS_LIST += $(TARGET_COMMON_BSP_LDLIBS_DIR)

#==================================== bsp path define end =========================================

ifeq ($(strip  $(TARGET)), BSP)
	TARGET_COMMON_LDLIBS_DIR := $(TARGET_COMMON_BSP_LDLIBS_DIR)
else 
	TARGET_COMMON_LDLIBS_DIR := $(TARGET_COMMON_APP_LDLIBS_DIR)
endif

# create target out dirs
$(foreach DIR, $(OUT_DIRS_LIST), $(shell mkdir -p $(DIR)))

#defined in bsp.Mk
export BSP_MODULES :=

#defined in app.Mk
export APP_MODULES :=

$(info Buding target $(TARGET))

include $(MFILES_DIR)/build_config.MK
include $(MFILES_DIR)/define.MK

APP_MODULES := $(sort $(APP_MODULES))
BSP_MODULES := $(sort $(BSP_MODULES))

# include all the .Mk files in subdirs
subdir_makefiles := $(shell find $(TOP_DIR)/ -name "*.Mk" | sort  2>/dev/null)
$(foreach mk, $(subdir_makefiles), $(info including $(mk) ...)$($(eval -include $(mk))))

#most importent command 
#Makefile will find all the $(APP_MODULES) defined in app/app.Mk to build it is the way this Makefile working
.PHONY :app app.clean app.install
app:$(APP_MODULES)
	$(call color_echo,32,40,"SDK building $@ $(APP_MODULES) done... ")

.PHONY :bsp bsp.clean bsp.install
bsp:$(BSP_MODULES)
	@echo $(TARGET_KERNEL_DIR)
	@echo SDK version : $(BSP_MODULES)
	$(call color_echo,32,40,"SDK building $@ $(BSP_MODULES) done... ")

all_test:
	@echo $(CROSS_COMPILE)
	@echo $(OS_TYPE)
	@echo $(OUT_DIRS_LIST)
	@echo BSP_MOHULES : $(BSP_MODULES)
	@echo $(TARGET_KERNEL_DIR)

.PHONY :clean
.PHONY :distclean
# "$(RM)" equals to "rm -f"
clean:
	@$(RM) -r $(OUT_DIR)
	@for dir in $(APP_SUBDIR); do\
		$(MAKE) -C $$dir clean || exit 1;\
	done

distclean:
	@for dir in $(APP_SUBDIR); do\
		$(MAKE) -C $$dir distclean || exit 1;\
	done
