ifeq ($(strip $(LOCAL_MODULE)),)
ifeq ($(strip $(LOCAL_MODULE_STEM)),)
$(error LOCAL_MODULE & LOCAL_MODULE_STEM is not defined in $(LOCAL_PATH)!!!)
else
LOCAL_MODULE := $(LOCAL_MODULE_STEM)
endif
endif

ifeq ($(strip $(LOCAL_SRC_FILES)),)
$(error LOCAL_SRC_FILES is not defined in $(LOCAL_PATH)!!!)
endif

ifeq ($(strip $(LOCAL_MODULE_CLASS)),)
LOCAL_MODULE_CLASS := PREBUILT
endif

ifeq ($(strip $(LOCAL_MODULE_STEM)),)
LOCAL_MODULE_STEM := $(LOCAL_MODULE)
endif

target_name := $(LOCAL_MODULE_STEM)$(LOCAL_MODULE_SUFFIX)

ifeq ($(strip $(call is_prbt_removed,$(LOCAL_MODULE))),yes)
target_name :=
endif
############################################
#we need a target
#if we have no target, do nothing
############################################
ifneq ($(strip $(target_name)),)

target_do_dir := $(TARGET_DO_DIR)/$(LOCAL_MODULE_CLASS)/$(LOCAL_MODULE)

ifeq ($(strip $(LOCAL_INSTALLED_NAME)),)
LOCAL_INSTALLED_NAME := $(LOCAL_SRC_FILES)
endif
linked_target := $(addprefix $(target_do_dir)/,$(LOCAL_INSTALLED_NAME))

LINKED_TARGET := $(strip $(linked_target))

ifeq ($(strip $(LOCAL_MODULE_PATH)),)
LOCAL_MODULE_PATH := $(TARGET_OUT_SYSTEM_DIR)/etc
endif

src_files_number := $(words $(LOCAL_SRC_FILES))

prb_static_libs := $(filter %.a,$(LOCAL_SRC_FILES))

define filter_libxx_so_x
$(filter %.so, \
$(foreach file,$(1), \
$(join $(findstring lib,$(file)),$(findstring .so,$(file))) \
) \
)
endef

prb_shared_libs := $(call filter_libxx_so_x,$(LOCAL_SRC_FILES))

ifneq ($(strip $(prb_static_libs)),)
ifneq ($(strip $(src_files_number)),1)
$(error More than one prebuilt static libs in $(LOCAL_PATH), $(local_src_files)...)
endif
target_do_dir := $(TARGET_DO_DIR)/STATIC_LIBS/$(LOCAL_MODULE)
ifeq ($(strip $(LOCAL_INSTALLED_NAME)),)
LOCAL_INSTALLED_NAME := $(prb_static_libs)
endif
LINKED_TARGET := $(target_do_dir)/LINKED/$(notdir $(LOCAL_INSTALLED_NAME))
#LOCAL_MODULE_PATH := NULL
endif

ifneq ($(strip $(prb_shared_libs)),)
ifneq ($(strip $(src_files_number)),1)
$(error More than one prebuilt shared libs in $(LOCAL_PATH), $(local_src_files)...)
endif
target_do_dir := $(TARGET_DO_DIR)/SHARED_LIBS/$(LOCAL_MODULE)
ifeq ($(strip $(LOCAL_INSTALLED_NAME)),)
LOCAL_INSTALLED_NAME := $(LOCAL_SRC_FILES)
endif
LINKED_TARGET := $(TARGET_COMMON_LDLIBS_DIR)/$(notdir $(LOCAL_INSTALLED_NAME))
ifeq ($(strip $(LOCAL_MODULE_PATH)),)
LOCAL_MODULE_PATH := $(TARGET_OUT_SYSTEM_DIR)/lib
endif
endif

src_files := $(addprefix $(LOCAL_PATH)/,$(LOCAL_SRC_FILES))
$(LINKED_TARGET): PRIVATE_SRC_FILES := $(src_files)

ifneq ($(strip $(LINKED_TARGET)),)

ifneq ($(strip $(src_files)),)
$(src_files): $(LOCAL_PATH)/%:
	$(silent) echo Nothing to do for $@...
endif

ifeq ($(strip $(src_files_number)),1)
$(LINKED_TARGET):
	$(silent) mkdir -p $(dir $@)
	$(silent) -rm -rf $@
	$(silent) cp -prf $(PRIVATE_SRC_FILES) $@
else
$(LINKED_TARGET): $(target_do_dir)/%: $(LOCAL_PATH)/%
	$(silent) mkdir -p $(dir $@)
	$(silent) -rm -rf $@
	$(silent) cp -prf $< $@
endif

endif

include $(MFILES_DIR)/module_target.MK

endif
