LOCAL_PATH := $(call local-dir)
TARGET_DO_DIR := $(TARGET_APP_DO_DIR)
include $(CLEAR_MFILE)
LOCAL_MODULE := timeslice
LOCAL_SRC_FILES := list.c timeslice.c main.c 
LOCAL_MODULE_PATH := $(TARGET_APP_OUT)/apps
LOCAL_LDLIBS := -lpthread
include $(EXECUTABLE_MFILE)
LOCAL_DEPEND_MODULES := timeslice 
#include $(BUILDING2PREBUILT_MFILE)
