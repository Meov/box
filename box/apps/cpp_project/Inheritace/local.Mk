LOCAL_PATH := $(call local-dir)
TARGET_DO_DIR := $(TARGET_APP_DO_DIR)
LOCAL_VERSION := 1.00.00
include $(CLEAR_MFILE)
LOCAL_MODULE := Inheritace_main
LOCAL_SRC_FILES := main.cpp
LOCAL_MODULE_PATH := $(TARGET_APP_OUT)/apps
include $(EXECUTABLE_MFILE)
LOCAL_DEPEND_MODULES := Inheritace_main

#build muitiple apps just add scripts
LOCAL_VERSION := 1.00.00
LOCAL_MODULE := Inheritace_simple
LOCAL_SRC_FILES := simple.cpp
LOCAL_MODULE_PATH := $(TARGET_APP_OUT)/apps
include $(EXECUTABLE_MFILE)
LOCAL_DEPEND_MODULES := Inheritace_simple
