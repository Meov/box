LOCAL_PATH := $(call local-dir)
TARGET_DO_DIR := $(TARGET_APP_DO_DIR)
LOCAL_VERSION := 1.00.00
include $(CLEAR_MFILE)
LOCAL_MODULE := string_sample_cpp
LOCAL_SRC_FILES := mystring.cpp main.cpp
LOCAL_MODULE_PATH := $(TARGET_APP_OUT)/app

include $(EXECUTABLE_MFILE)
LOCAL_DEPEND_MODULES := string_sample_cpp
#include $(BUILDING2PREBUILT_MFILE)
