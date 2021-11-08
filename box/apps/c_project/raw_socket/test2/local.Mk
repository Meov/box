LOCAL_PATH := $(call local-dir)
TARGET_DO_DIR := $(TARGET_APP_DO_DIR)
LOCAL_VERSION := 1.00.00
include $(CLEAR_MFILE)
LOCAL_MODULE := raw_socket_test_ipdump
LOCAL_SRC_FILES := main.c
LOCAL_MODULE_PATH := $(TARGET_APP_OUT)/apps

include $(EXECUTABLE_MFILE)
LOCAL_DEPEND_MODULES := raw_socket_test_ipdump 
#include $(BUILDING2PREBUILT_MFILE)
