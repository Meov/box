export_includes := $(target_do_dir)/export_includes

$(export_includes): PRIVATE_EXPORT_C_INCLUDE_DIRS := $(LOCAL_EXPORT_C_INCLUDE_DIRS)
$(export_includes) : $(LOCAL_MODULE_MAKEFILE)
	$(silent) echo Export inlcudes file: $< -- $@
	$(silent) mkdir -p $(dir $@) && rm -rf $@
ifneq ($(strip $(LOCAL_EXPORT_C_INCLUDE_DIRS)),)
	$(silent) for inc in $(PRIVATE_EXPORT_C_INCLUDE_DIRS); do \
		      echo -I $$inc >> $@; \
		      done
else
	$(silent) touch $@
endif


local_module_built := $(LOCAL_MODULE).built
local_module_install := $(LOCAL_MODULE).install
local_module_clean := $(LOCAL_MODULE).clean
local_module_depend := $(LOCAL_MODULE).depend
local_module_config := $(LOCAL_MODULE).config
local_module_minfo := $(LOCAL_MODULE).minfo

.PHONY: $(LOCAL_MODULE) $(local_module_install) $(local_module_clean) $(local_module_depend) $(local_module_config) $(local_module_minfo)

$(LOCAL_MODULE): PRIVATE_MODULE_NAME := $(LOCAL_MODULE)
ifneq ($(strip $(LOCAL_VERSION)),)
$(LOCAL_MODULE): PRIVATE_LOCAL_VERSION := $(LOCAL_VERSION)
else
$(LOCAL_MODULE): PRIVATE_LOCAL_VERSION := $(SDK_VERSION)
endif
$(LOCAL_MODULE): PRIVATE_INSTALL := $(local_module_install)
$(LOCAL_MODULE): PRIVATE_CLEAN := $(local_module_clean)
$(LOCAL_MODULE): PRIVATE_DEPEND := $(local_module_depend)
$(LOCAL_MODULE): PRIVATE_CONFIG := $(local_module_config)
$(LOCAL_MODULE): PRIVATE_MODULE_CLASS := $(LOCAL_MODULE_CLASS)
$(LOCAL_MODULE): PRIVATE_LINKED_TARGET := $(LINKED_TARGET)
$(LOCAL_MODULE): PRIVATE_MODULE_PATH := $(LOCAL_MODULE_PATH)
$(LOCAL_MODULE): PRIVATE_DO_DIR := $(target_do_dir) 
$(LOCAL_MODULE): PRIVATE_DEPEND_STATIC_LIBRARIES := $(LOCAL_STATIC_LIBRARIES)
$(LOCAL_MODULE): PRIVATE_DEPEND_SHARED_LIBRARIES := $(LOCAL_SHARED_LIBRARIES)
$(LOCAL_MODULE): PRIVATE_MODULE_PRECONFIG_SCRIPTS := $(LOCAL_MODULE_PRECONFIG_SCRIPTS) 

$(LOCAL_MODULE): $(local_module_install) | $(export_includes)
	$(call color_echo,35,40,"SDK building $(PRIVATE_MODULE_NAME) done ...")

local_module_depend_libs := $(LOCAL_STATIC_LIBRARIES)
local_module_depend_libs += $(LOCAL_SHARED_LIBRARIES)

linked_targets_number := $(words $(LINKED_TARGET))
# only one target
ifeq ($(strip $(linked_targets_number)),1)
ifneq ($(strip $(LOCAL_INSTALLED_NAME)),)
$(LOCAL_MODULE): PRIVATE_INSTALLED_TARGET_NAME := $(notdir $(LOCAL_INSTALLED_NAME))
else
$(LOCAL_MODULE): PRIVATE_INSTALLED_TARGET_NAME := $(notdir $(LINKED_TARGET))
endif
endif

$(LINKED_TARGET): $(local_module_depend) $(local_module_config)

$(local_module_built): $(LINKED_TARGET)

$(local_module_install): PRIVATE_MODULE_PATH := $(LOCAL_MODULE_PATH)
$(local_module_install): PRIVATE_LINKED_TARGET := $(LINKED_TARGET)
ifeq ($(strip $(linked_targets_number)),1)
ifneq ($(strip $(LOCAL_INSTALLED_NAME)),)
$(local_module_install): PRIVATE_INSTALLED_TARGET_NAME := $(notdir $(LOCAL_INSTALLED_NAME))
else
$(local_module_install): PRIVATE_INSTALLED_TARGET_NAME := $(notdir $(LINKED_TARGET))
endif
endif

$(local_module_install): $(local_module_built)
ifneq ($(strip $(LOCAL_MODULE_PATH)),NULL)
	$(hide) mkdir -p $(PRIVATE_MODULE_PATH)
ifneq ($(strip $(LINKED_TARGET)),)
ifeq ($(strip $(linked_targets_number)),1)
	$(hide) -rm -rf $(PRIVATE_MODULE_PATH)/$(PRIVATE_INSTALLED_TARGET_NAME);
	$(hide) cp -prf $(PRIVATE_LINKED_TARGET) $(PRIVATE_MODULE_PATH)/$(PRIVATE_INSTALLED_TARGET_NAME);
else
	$(hide) for file in $(PRIVATE_LINKED_TARGET); do \
	        -rm -rf $(PRIVATE_MODULE_PATH)/$$file; \
		    cp -prf $$file $(PRIVATE_MODULE_PATH)/; \
		    done
endif
endif
endif
	$(hide) echo "$(PRIVATE_MODULE_NAME)@V$(PRIVATE_LOCAL_VERSION),$$(whoami)@$$(hostname),$$(date)">$(PRIVATE_MODULE_PATH)/.version.$(PRIVATE_MODULE_NAME)  2>/dev/null

$(local_module_depend): $(LOCAL_DEPEND_MODULES) $(local_module_depend_libs)

$(local_module_config):
ifneq ($(strip $(PRIVATE_MODULE_PRECONFIG_SCRIPTS)),)
	$(hide) chmod +x $(PRIVATE_MODULE_PRECONFIG_SCRIPTS)
	$(hide) $(PRIVATE_MODULE_PRECONFIG_SCRIPTS)
else
	$(hide) echo "nothing need to be done for $@..."
endif

$(local_module_clean): PRIVATE_DO_DIR := $(target_do_dir) 
$(local_module_clean): PRIVATE_LINKED_TARGET := $(LINKED_TARGET)
$(local_module_clean):
	$(hide) rm -rf $(PRIVATE_LINKED_TARGET) 
	$(hide) rm -rf $(PRIVATE_DO_DIR) 

#makefile debug info
$(local_module_minfo): PRIVATE_MODULE_PATH := $(LOCAL_MODULE_PATH)
$(local_module_minfo): PRIVATE_LINKED_TARGET := $(LINKED_TARGET)
$(local_module_minfo):
	$(hide) echo $(PRIVATE_MODULE_PATH)
	$(hide) echo $(PRIVATE_LINKED_TARGET)
