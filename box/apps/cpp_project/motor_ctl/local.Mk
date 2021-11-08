LOCAL_PATH := $(call local-dir)
TARGET_DO_DIR := $(TARGET_APP_DO_DIR)
LOCAL_VERSION := 1.00.00
include $(CLEAR_MFILE)
LOCAL_MODULE := motor_ctr
SRCS_DIR1 := $(wildcard $(LOCAL_PATH)/src/*.cpp)
SRCS_DIR2 := $(wildcard $(LOCAL_PATH)/src/common/*.cpp)
dir1:=$(notdir $(SRCS_DIR1))
dir2:=$(notdir $(SRCS_DIR2))
SRCS=$(patsubst %.cpp,src/%.cpp,$(dir1))
SRCS+=$(patsubst %.cpp,src/common/%.cpp,$(dir2))
LOCAL_C_INCLUDES := $(LOCAL_PATH)/src $(LOCAL_PATH)/src/common

LOCAL_SRC_FILES = $(SRCS) 
LOCAL_MODULE_PATH := $(TARGET_APP_OUT)/app
LOCAL_CPPFLAGS:=-std=c++11
LOCAL_LDLIBS :=-lpthread
include $(EXECUTABLE_MFILE)
LOCAL_DEPEND_MODULES := motor_ctr
