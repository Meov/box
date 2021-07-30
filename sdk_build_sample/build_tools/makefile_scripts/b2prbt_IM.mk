ifeq ($(strip $(LOCAL_MODULE)),)
$(error LOCAL_MODULE no define...)
endif

ifeq ($(strip $(LINKED_TARGET)),)
$(error LINKED_TARGET no define...)
endif

#ifeq ($(strip $(LOCAL_SRC_FILES)),)
#$(error LOCAL_SRC_FILES no define...)
#endif

ifneq ($(filter %.Mk,$(strip $(LOCAL_SRC_FILES))),)
$(error LOCAL_SRC_FILES "$(LOCAL_SRC_FILES)" include %.Mk...)
endif

ACT := IM
ACTIONS := install \
           move

include $(MFILES_DIR)/b2prbt.mk
