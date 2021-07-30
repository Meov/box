ACT := $(act.$(CUSTOMER))

ifeq ($(strip $(ACT)),)
$(warning act.customer no define...)
else
include $(MFILES_DIR)/b2prbt_$(ACT).mk
endif
