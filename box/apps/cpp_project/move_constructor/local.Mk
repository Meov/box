LOCAL_PATH := $(call local-dir)
TARGET_DO_DIR := $(TARGET_APP_DO_DIR)
LOCAL_VERSION := 1.00.00
include $(CLEAR_MFILE)
LOCAL_MODULE := move_constructor
LOCAL_SRC_FILES := main.cpp
LOCAL_MODULE_PATH := $(TARGET_APP_OUT)/apps
LOCAL_FLAGS := -O2 -g -Wall -std=c++11

include $(EXECUTABLE_MFILE)
LOCAL_DEPEND_MODULES := move_constructor
#include $(BUILDING2PREBUILT_MFILE)
