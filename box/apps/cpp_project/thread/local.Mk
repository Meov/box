LOCAL_PATH := $(call local-dir)
TARGET_DO_DIR := $(TARGET_APP_DO_DIR)
LOCAL_VERSION := 1.00.00
include $(CLEAR_MFILE)
LOCAL_MODULE := muilt_thread_test
LOCAL_SRC_FILES := main.cpp 
LOCAL_MODULE_PATH := $(TARGET_APP_OUT)/apps
LOCAL_CPPFLAGS:= -std=c++11
LOCAL_LDLIBS := -lpthread
include $(EXECUTABLE_MFILE)
LOCAL_DEPEND_MODULES := muilti_thread_test 
#include $(BUILDING2PREBUILT_MFILE)
