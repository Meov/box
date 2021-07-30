# support multi modeles depend on same file, only delete for the last module
# support different actions for specail customer, use act.datang := CIM, and include $(PRBTCDT_MFILE)
# we also store a files list files_list.$(module) in $(B2PRBT_BACKUP_DIR), and we read files list form it
# when making prbt2b
#known BUG: for modem_ko, the modem_ko.clean not execute in bclean
# makefile: one command one shell process, command parse by makefile first. var replacement, then call shell

b2prbt_name := $(call prbt_module,$(LOCAL_MODULE))
ALL_B2PRBT_MODULES += $(b2prbt_name)

# '/' need zhuanyi in awk
TMP_TOP_DIR := $(subst /,\/,$(TOP_DIR))
RELATIVE_PATH := $(shell echo $(LOCAL_PATH) | awk '{sub(/^$(TMP_TOP_DIR)\//,"",$$0);print $$0}')
#RELATIVE_PATH := $(subst $(TOP_DIR)/,,$(LOCAL_PATH))
RELATIVE_PATH := $(call relative_path,$(TOP_DIR),$(LOCAL_PATH))
CLEAN_FILES_LOG := $(TARGET_OUT_DIR)/prbt_clean_files_$(START_DATE).info
RECOVERY_FILES_LOG := $(TARGET_OUT_DIR)/b_recovery_files_$(START_DATE).info

$(foreach file,$(LOCAL_SRC_FILES),$(eval $(call add_file_ref,$(call file_ref_var,$(LOCAL_PATH)/$(file)))))

$(b2prbt_name): PRIVATE_LOCAL_MODULE := $(LOCAL_MODULE)
$(b2prbt_name): PRIVATE_ACTIONS := $(ACTIONS)
$(b2prbt_name): PRIVATE_ACT := $(ACT)
$(b2prbt_name): PRIVATE_LINKED_TARGET := $(LINKED_TARGET)
$(b2prbt_name): PRIVATE_TARGET := $(notdir $(LINKED_TARGET))
$(b2prbt_name): PRIVATE_LINKED_TARGET_PATH := $(dir $(LINKED_TARGET))
$(b2prbt_name): PRIVATE_RELATIVE_PATH := $(RELATIVE_PATH)
$(b2prbt_name): PRIVATE_LOCAL_PATH := $(LOCAL_PATH)
$(b2prbt_name): PRIVATE_LOCAL_SRC_FILES := $(LOCAL_SRC_FILES)
$(b2prbt_name): PRIVATE_LOCAL_SRC_FILES_REF_CNT_VAR := $(foreach file,$(LOCAL_SRC_FILES),$(call file_ref_var,$(LOCAL_PATH)/$(file)))
$(b2prbt_name): PRIVATE_CUSTOMER_DEFINED_INSTALL_RULES := $(CUSTOMER_DEFINED_INSTALL_RULES)
$(b2prbt_name): PRIVATE_CDIR_ARGS := $(CDIR_ARGS)
$(b2prbt_name): PRIVATE_LOCAL_DEPEND_MODULES := $(LOCAL_DEPEND_MODULES)

.PHONY: $(b2prbt_name) $(b2prbt_name).copy $(b2prbt_name).bclean

$(b2prbt_name): $(b2prbt_name).copy $(b2prbt_name).bclean
	@ echo "$@ := yes.$(PRIVATE_ACT)" >> $(OPTIONAL_PREBUILT_MFILE);
	@ echo Convert $(call prbt_module2raw_module,$@) to prbt done...

ifeq ($(strip $(findstring compile,$(ACTIONS))),compile)
#$(b2prbt_name).copy: $(LOCAL_DEPEND_MODULES)
$(b2prbt_name).copy:
	@for _md in $(PRIVATE_LOCAL_DEPEND_MODULES);do make -C $(TOP_DIR) $${_md};done
else
$(b2prbt_name).copy:
endif
# if source dest is same. what to do
ifeq ($(strip $(findstring install,$(ACTIONS))),install)
ifeq ($(strip $(CUSTOMER_DEFINED_INSTALL_RULES)),)
	@ if [ "$(PRIVATE_LINKED_TARGET)" != "$(PRIVATE_LOCAL_PATH)/$(PRIVATE_TARGET)" ]; then \
	  cp -pf $(PRIVATE_LINKED_TARGET) $(PRIVATE_LOCAL_PATH)/; \
	  fi;
else
	@$(call $(PRIVATE_CUSTOMER_DEFINED_INSTALL_RULES),$(PRIVATE_CDIR_ARGS))
endif
endif

ifeq ($(strip $(findstring compile,$(ACTIONS))),compile)
$(b2prbt_name).bclean: $(LOCAL_DEPEND_MODULES).clean
else
$(b2prbt_name).bclean:
endif
ifeq ($(strip $(findstring move,$(ACTIONS))),move)
	@ mkdir -p $(dir $(CLEAN_FILES_LOG));if [ ! -e $(CLEAN_FILES_LOG) ];then touch $(CLEAN_FILES_LOG);else echo -n -e "\n" >>$(CLEAN_FILES_LOG); fi
	@ echo $(PRIVATE_LOCAL_MODULE),`date` >>$(CLEAN_FILES_LOG)
#	@ echo REF_VAR $(PRIVATE_LOCAL_SRC_FILES_REF_CNT_VAR)
#	@ echo reference count $(foreach var,$(PRIVATE_LOCAL_SRC_FILES_REF_CNT_VAR),$(value $(var)))
	@ LIST_FILE=$(B2PRBT_BACKUP_DIR)/$(PRIVATE_RELATIVE_PATH)/files_list.$(PRIVATE_LOCAL_MODULE) \
	  FILES=($(PRIVATE_LOCAL_SRC_FILES));\
	  FILES_REF_CNT=($(foreach var,$(PRIVATE_LOCAL_SRC_FILES_REF_CNT_VAR),$(value $(var))));\
	  FILES_NUM=$(words $(PRIVATE_LOCAL_SRC_FILES));\
	  TMP_FILES_ID=`echo $$FILES_NUM | awk '{y=$$0-1;print y}'`;\
      -rm -rf $${LIST_FILE}; \
	  for num in `seq 0 $$TMP_FILES_ID`; do \
	  TMP_DIR=`echo $(B2PRBT_BACKUP_DIR)/$(PRIVATE_RELATIVE_PATH)/$${FILES[$$num]} | \
	  awk '{sub(/\/[^\/]+$$/,"",$$0);print $$0}'`; \
	  BASE_NAME=`echo $${FILES[$$num]} | \
	  awk '{i=split($$0,A,/\//);print A[i]}'`; \
      echo "$$TMP_DIR/$$BASE_NAME reference count $${FILES_REF_CNT[$$num]}" &>>$(CLEAN_FILES_LOG); \
	  G_REF_CNT_FILE=g_ref_cnt_$${BASE_NAME}; \
	  L_REF_CNT_FILE=l_ref_cnt_$${BASE_NAME}; \
	  mkdir -pv $$TMP_DIR; \
	  if [ ! -e $${TMP_DIR}/$${G_REF_CNT_FILE} ];then \
          echo "$${FILES_REF_CNT[$$num]}" > $${TMP_DIR}/$${G_REF_CNT_FILE}; \
          echo "$${FILES_REF_CNT[$$num]}" > $${TMP_DIR}/$${L_REF_CNT_FILE}; \
	  fi; \
	  REF_CNT=`cat $${TMP_DIR}/$${L_REF_CNT_FILE} | awk '{if ($$0 > 0) y=$$0 - 1; else y=0;print y}'`;\
      if [ -z "$$REF_CNT" ]; then \
      echo "$$TMP_DIR/$$BASE_NAME reference count NULL" &>>$(CLEAN_FILES_LOG); \
      exit 1; \
	  fi; \
      echo $${BASE_NAME} REF_CNT $${REF_CNT};\
      echo $${REF_CNT} > $${TMP_DIR}/$${L_REF_CNT_FILE};\
      echo "$${TMP_DIR}/$${L_REF_CNT_FILE} `cat $${TMP_DIR}/$${L_REF_CNT_FILE}`";\
      echo "$$TMP_DIR/$$BASE_NAME reference count $$REF_CNT" &>>$(CLEAN_FILES_LOG); \
      if [ "$$REF_CNT" -eq "0" ]; then \
	  mv -vf $(PRIVATE_LOCAL_PATH)/$${FILES[$$num]} $$TMP_DIR/$$BASE_NAME &>>$(CLEAN_FILES_LOG); \
	  ls -l $(PRIVATE_LOCAL_PATH)/$${FILES[$$num]} &>>$(CLEAN_FILES_LOG); \
	  ls -l $$TMP_DIR/$$BASE_NAME &>>$(CLEAN_FILES_LOG); \
	  rm -rf $${TMP_DIR}/$${L_REF_CNT_FILE}; \
	  fi; \
      echo -n "$${FILES[$$num]} " 1>>$${LIST_FILE}; \
	  done
endif
#	  for file in $(PRIVATE_LOCAL_SRC_FILES); do \

prbt2b_name := $(call b_module,$(LOCAL_MODULE))

ALL_PRBT2B_MODULES += $(prbt2b_name)

$(prbt2b_name): PRIVATE_LOCAL_MODULE := $(LOCAL_MODULE)
$(prbt2b_name): PRIVATE_ACTIONS := $(ACTIONS)
$(prbt2b_name): PRIVATE_LINKED_TARGET := $(LINKED_TARGET)
$(prbt2b_name): PRIVATE_TARGET := $(notdir $(LINKED_TARGET))
$(prbt2b_name): PRIVATE_LINKED_TARGET_PATH := $(dir $(LINKED_TARGET))
$(prbt2b_name): PRIVATE_RELATIVE_PATH := $(RELATIVE_PATH)
$(prbt2b_name): PRIVATE_LOCAL_PATH := $(LOCAL_PATH)
$(prbt2b_name): PRIVATE_LOCAL_SRC_FILES := $(LOCAL_SRC_FILES)
$(prbt2b_name): PRIVATE_CUSTOMER_DEFINED_UNINSTALL_RULES := $(CUSTOMER_DEFINED_UNINSTALL_RULES)
$(prbt2b_name): PRIVATE_CDUR_ARGS := $(CDUR_ARGS)

.PHONY: $(prbt2b_name) $(prbt2b_name).uninstall

$(prbt2b_name).uninstall:
ifeq ($(strip $(findstring install,$(ACTIONS))),install)
ifeq ($(strip $(CUSTOMER_DEFINED_UNINSTALL_RULES)),)
	@ -rm -rf $(PRIVATE_LOCAL_PATH)/$(PRIVATE_TARGET)
else
	@$(call $(PRIVATE_CUSTOMER_DEFINED_UNINSTALL_RULES),$(CDUR_ARGS))
endif
endif

$(prbt2b_name): $(prbt2b_name).uninstall
	@ mkdir -p $(dir $(RECOVERY_FILES_LOG));if [ ! -e $(RECOVERY_FILES_LOG) ];then touch $(RECOVERY_FILES_LOG);else echo -n -e "\n" >>$(RECOVERY_FILES_LOG); fi;
	@ echo $(PRIVATE_LOCAL_MODULE),`date` >>$(RECOVERY_FILES_LOG)
	@ FILES=`cat $(B2PRBT_BACKUP_DIR)/$(PRIVATE_RELATIVE_PATH)/files_list.$(PRIVATE_LOCAL_MODULE) 2>/dev/null`; \
	  echo FILES $$FILES; \
	  for file in $$FILES; do \
	  TMP_B2PRBT_DIR=`echo $(B2PRBT_BACKUP_DIR)/$(PRIVATE_RELATIVE_PATH)/$$file | \
	  awk '{sub(/\/[^\/]+$$/,"",$$0);print $$0}'`; \
	  TMP_DIR=`echo $(PRIVATE_LOCAL_PATH)/$$file | \
	  awk '{sub(/\/[^\/]+$$/,"",$$0);print $$0}'`; \
	  BASE_NAME=`echo $$file | \
	  awk '{i=split($$0,A,/\//);print A[i]}'`; \
	  G_REF_CNT_FILE=g_ref_cnt_$${BASE_NAME}; \
	  L_REF_CNT_FILE=l_ref_cnt_$${BASE_NAME}; \
	  if [ ! -e $${TMP_B2PRBT_DIR}/$${L_REF_CNT_FILE} ];then \
          cat $${TMP_B2PRBT_DIR}/$${G_REF_CNT_FILE} > $${TMP_B2PRBT_DIR}/$${L_REF_CNT_FILE}; \
	  fi; \
	  REF_CNT=`cat $${TMP_B2PRBT_DIR}/$${L_REF_CNT_FILE} | awk '{if ($$0 > 0) y=$$0 - 1; else y=0;print y}'`;\
      if [ -z "$$REF_CNT" ]; then \
      echo "$$TMP_DIR/$$BASE_NAME reference count NULL" &>>$(RECOVERY_FILES_LOG); \
      exit 1; \
	  fi; \
      echo $$REF_CNT > $${TMP_B2PRBT_DIR}/$${L_REF_CNT_FILE};\
      echo "$$TMP_DIR/$$BASE_NAME reference count $$REF_CNT" &>>$(RECOVERY_FILES_LOG); \
      if [ "$$REF_CNT" -eq "0" ]; then \
	  mv -vf $(B2PRBT_BACKUP_DIR)/$(PRIVATE_RELATIVE_PATH)/$$file $$TMP_DIR/$$BASE_NAME \
	  &>>$(RECOVERY_FILES_LOG); \
	  fi; \
	  done
	@ cat $(OPTIONAL_PREBUILT_MFILE) | awk '{key=sprintf("%s.%s",$(PRIVATE_LOCAL_MODULE),"prbt"); \
        i=index($$0,key);if (i > 0) sub(/yes\..*M/,"no", $$0);print $$0}' \
        >> $(OPTIONAL_PREBUILT_MFILE).new
	@ -rm -rf $(OPTIONAL_PREBUILT_MFILE)
	@ mv $(OPTIONAL_PREBUILT_MFILE).new $(OPTIONAL_PREBUILT_MFILE)
	@ echo Convert $(call b_module2raw_module,$@) to b done...
