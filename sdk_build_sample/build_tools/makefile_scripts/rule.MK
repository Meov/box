ifeq ($(TARGET),APP)
include $(APP_PROJECT_DIR)/app.MK

else ifeq ($(TARGET_BUILD),APDD)
include $(APDD_PROJECT_DIR)/apdd.MK

else
include $(APP_PROJECT_DIR)/app.MK
include $(APDD_PROJECT_DIR)/apdd.MK

endif

#SUBDIR+=$(TOP_DIR)/app 
#SUBDIR+=$(TOP_DIR)/apdd

