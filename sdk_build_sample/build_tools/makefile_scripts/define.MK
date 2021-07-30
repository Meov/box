
# Figure out where we are.
define local-dir
$(strip \
  $(eval LOCAL_MODULE_MAKEFILE := $$(lastword $$(MAKEFILE_LIST))) \
  $(if $(filter $(CLEAR_MFILE),$(LOCAL_MODULE_MAKEFILE)), \
	$(error LOCAL_PATH must be set before including $$(CLEAR_MFILE)) \
   , \
	$(patsubst %/,%,$(dir $(LOCAL_MODULE_MAKEFILE))) \
   ) \
 )
endef

# b2prbt files removed reference count
define add_file_ref
ifeq ($(strip $($(1))),)
$(1) := 1
else
$(1) := $(shell echo $($(1)) | awk '{y = $$0 + 1;print y}')
endif
endef

define del_file_ref
ifeq ($(strip $($(1))),0)
$(1) := 0
else
$(1) := $(shell echo $($(1)) | awk '{y = $$0 - 1;print y}')
endif
endef

define get_file_ref
$(value $(1))
endef

#input full path name
define file_ref_var
$(subst /,+,$(patsubst /%,%,$(1)))_REF_CNT
endef

define relative_path
$(patsubst /%,%,$(subst $(1),,$(2)))
endef

# exclude *.Mk Mk should be in the 1-level dir
define pwd_files
$(filter-out %.Mk . ..,$(shell ls -a $(1) 2>/dev/null))
endef

# $(1):TOP_DIR $(2)LOCAL_PATH
# careful TOP_DIR == LOCAL_PATH
define b2prbt_pwd_files
$(call pwd_files,$(B2PRBT_BACKUP_DIR)/$(call relative_path,$(1),$(2)))
endef

define prbt_module
$(strip $(1)).prbt
endef

define is_prbt_removed
$(if $(findstring .M,$(value $(call prbt_module,$(1)))),yes)
endef

# prbt2b only convert already converted to prbt modules, judging according var module.prbt in optional_prbt.mk
ifeq ($(strip $(MAKECMDGOALS)),prbt2b)
define is_module_prbt
$(if $(findstring yes,$(value $(call prbt_module,$(1)))),no,yes)
endef
else
define is_module_prbt
$(strip $(findstring yes,$(value $(call prbt_module,$(1)))))
endef
endif

define prbt_module2raw_module
$(strip $(subst .prbt,,$(1)))
endef

define b_module
$(strip $(1)).b
endef

define b_module2raw_module
$(strip $(subst .b,,$(1)))
endef

define b_module2prbt_module
$(strip $(call prbt_module,$(call b_module2raw_module,$(1))))
endef

define prbt_module2b_module
$(strip $(call b_module,$(call prbt_module2raw_module,$(1))))
endef

#$(1) text color 3X
#$(2) background color 4X
#$(3) string to echo
# 0: black 1:red 2:green 3:yellow 4:deep blue 5:purple 6:blue
define color_echo
@ echo -e "\033[$(1);$(2)m$(3)\033[0m"
endef

###########################################################
## Commands for munging the dependency files GCC generates
###########################################################
# $(1): the input .d file
# $(2): the output .P file
define compile-d-to-p-args
$(hide) cp $(1) $(2); \
	sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
	-e '/^$$/ d' -e 's/$$/ :/' < $(1) >> $(2); \
	rm -f $(1)
endef

define compile-d-to-p
$(call compile-d-to-p-args,$(@:%.o=%.d),$(@:%.o=%.P))
endef

define normalize-libraries
$(foreach so,$(filter %.so,$(1)),-l$(patsubst lib%.so,%,$(notdir $(so))))\
$(filter-out %.so,$(1))
endef

define fuck
endef

define generate_fullpath_shared_libs
$(TARGET_COMMON_LDLIBS_DIR)/$(1).so
endef

define generate_fullpath_static_libs
$(TARGET_DO_DIR)/STATIC_LIBS/$(1)/LINKED/$(1).a
endef

define compile-c-to-o-no-deps
$(hide) mkdir -p $(dir $@)
$(hide) $(PRIVATE_CC) \
	$(addprefix -I ,$(PRIVATE_C_INCLUDES)) \
	$(shell cat $(PRIVATE_IMPORT_C_INCLUDE_DIRS) 2>/dev/null) \
	$(addprefix -isystem , $(filter-out $(PRIVATE_C_INCLUDES),$(PRIVATE_C_OBJECTS_INCLUDES))) \
	$(PRIVATE_C_OBJECTS_CFLAGS) \
	$(1) \
	-c \
	-MD -MF $(patsubst %o,%d,$@) \
	$< \
	-o $@
endef

define compile-c-to-o
$(call compile-c-to-o-no-deps, $(PRIVATE_CFLAGS))
$(compile-d-to-p)
endef


define compile-c++-to-o
$(hide) mkdir -p $(dir $@)
$(hide) $(PRIVATE_CXX) \
	$(addprefix -I ,$(PRIVATE_C_INCLUDES)) \
	$(shell cat $(PRIVATE_IMPORT_C_INCLUDE_DIRS) 2>/dev/null) \
	$(addprefix -isystem , $(filter-out $(PRIVATE_C_INCLUDES),$(PRIVATE_C_OBJECTS_INCLUDES))) \
	$(PRIVATE_CPP_OBJECTS_CFLAGS) \
	$(PRIVATE_CFLAGS) \
	$(PRIVATE_CPPFLAGS) \
	-c \
	-MD -MF $(patsubst %o,%d,$@) \
	$< \
	-o $@
endef

define compile-o-to-execu
$(hide) mkdir -p $(dir $@)
$(hide) $(PRIVATE_CXX) \
	-Wl,-rpath-link=$(TARGET_COMMON_LDLIBS_DIR) \
	-Wl,-rpath,\$$ORIGIN/../lib \
	$(PRIVATE_ALL_OBJECTS) \
	$(PRIVATE_PREBUILT_OBJ_FILES) \
	$(PRIVATE_LDFLAGS) \
	$(PRIVATE_LDLIBS) \
	-Wl,--whole-archive \
	$(call normalize-libraries,$(PRIVATE_STATIC_LIBRARIES)) \
	-Wl,--no-whole-archive \
	$(call normalize-libraries,$(PRIVATE_SHARED_LIBRARIES)) \
	-L $(TARGET_COMMON_LDLIBS_DIR) \
	-o $@
endef

define _concat-if-arg2-not-empty
$(if $(2),$(hide) $(1) $(2))
endef

#wordlist get string not char
define split-long-arguments
$(hide) $(1) $(wordlist 1,500,$(2))
$(call _concat-if-arg2-not-empty,$(1),$(wordlist 501,1000,$(2)))
$(call _concat-if-arg2-not-empty,$(1),$(wordlist 1001,1500,$(2)))
$(call _concat-if-arg2-not-empty,$(1),$(wordlist 1501,2000,$(2)))
$(call _concat-if-arg2-not-empty,$(1),$(wordlist 2001,2500,$(2)))
$(call _concat-if-arg2-not-empty,$(1),$(wordlist 2501,3000,$(2)))
$(call _concat-if-arg2-not-empty,$(1),$(wordlist 3001,99999,$(2)))
endef

TARGET_GLOBAL_ARFLAGS := crsP

define _extract-and-include-single-target-whole-static-lib
@echo "preparing StaticLib: $(PRIVATE_MODULE_NAME) [including $(1)]"
$(hide) ldir=$(PRIVATE_TARGET_DO_DIR)/WHOLE/$(basename $(notdir $(1)))_objs;\
    rm -rf $$ldir; \
    mkdir -p $$ldir; \
    filelist=; \
    for f in `$(PRIVATE_AR) t $(1)`; do \
        $(PRIVATE_AR) p $(1) $$f > $$ldir/$$f; \
        filelist="$$filelist $$ldir/$$f"; \
    done ; \
    $(PRIVATE_AR) $(TARGET_GLOBAL_ARFLAGS) $(PRIVATE_ARFLAGS) $@ $$filelist

endef

define extract-and-include-target-whole-static-libs
$(foreach lib,$(PRIVATE_STATIC_LIBRARIES), \
    $(call _extract-and-include-single-target-whole-static-lib, $(lib)))
endef

define compile-o-to-static-libraries
$(hide) mkdir -p $(dir $@)
@rm -f $@
$(extract-and-include-target-whole-static-libs)
$(call split-long-arguments,$(PRIVATE_AR) $(TARGET_GLOBAL_ARFLAGS) $(PRIVATE_ARFLAGS) $@,$(filter %.o, $^))
endef

define compile-o-to-static-libraries-old
$(hide) mkdir -p $(dir $@)
$(hide) $(PRIVATE_CXX) \
	$(PRIVATE_ALL_OBJECTS) \
	$(PRIVATE_PREBUILT_OBJ_FILES) \
	$(PRIVATE_LDFLAGS) \
	$(PRIVATE_LDLIBS) \
	-Wl,--whole-archive \
	$(call normalize-libraries,$(PRIVATE_STATIC_LIBRARIES)) \
	-Wl,--no-whole-archive \
	$(call normalize-libraries,$(PRIVATE_SHARED_LIBRARIES)) \
	-L $(TARGET_COMMON_LDLIBS_DIR) \
	-o $@
endef

define compile-o-to-shared-libraries
$(hide) mkdir -p $(dir $@)
$(hide) $(PRIVATE_CXX) \
	-shared -Wl,-soname,$(notdir $@) \
	-Wl,-rpath-link=$(TARGET_COMMON_LDLIBS_DIR) \
	-Wl,-rpath,\$$ORIGIN/../lib \
	$(PRIVATE_ALL_OBJECTS) \
	$(PRIVATE_PREBUILT_OBJ_FILES) \
	$(PRIVATE_LDFLAGS) \
	$(PRIVATE_LDLIBS) \
	-Wl,--whole-archive \
	$(call normalize-libraries,$(PRIVATE_STATIC_LIBRARIES)) \
	-Wl,--no-whole-archive \
	$(call normalize-libraries,$(PRIVATE_SHARED_LIBRARIES)) \
	-L $(TARGET_COMMON_LDLIBS_DIR) \
	-o $@
endef
