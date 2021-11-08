LOCAL_PATH := $(call local-dir)
TARGET_DO_DIR := $(TARGET_APP_DO_DIR)
LOCAL_VERSION := 1.00.00
include $(CLEAR_MFILE)
LOCAL_MODULE_PATH := $(TARGET_APP_OUT)/apps
MAKE_APP_CMD :=
ARGS := O=$(LOCAL_MODULE_PATH)
MAKE_APP_CMD := make -C $(LOCAL_PATH) $(ARGS)

.PHONY: app_base
app_base:app_base.build 

.PHONY: app_base.build
.PHONY : app_base.clean

app_base.build:
	@echo "building app base..."
	$(MAKE_APP_CMD)

app_base.clean:
	@echo "cleaning app base..."
	$(silent) $(MAKE_APP_CMD) clean

