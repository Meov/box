LOCAL_PATH := $(call local-dir)
TARGET_DO_DIR := $(TARGET_APP_DO_DIR)
include $(CLEAR_MFILE)
LOCAL_VERSION := 1.00.00
LOCAL_MODULE := libtest_shared
LOCAL_SRC_FILES := src/sharedtest.c
LOCAL_C_INCLUDES := $(LOCAL_PATH)/inc
LOCAL_FLAGS := -O2 -g -Wall
LOCAL_FLAGS += -I $(LOCAL_PATH)/inc
LOCAL_LDLIBS := -lpthread
LOCAL_DEPEND_MODULES := libtest_shared
LOCAL_MODULE_PATH := $(TARGET_APP_OUT)/libs
include $(SHARED_LIB_MFILE)
#include $(STATIC_LIB_MFILE)
