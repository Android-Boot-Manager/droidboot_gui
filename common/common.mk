LOCAL_DIR := $(GET_LOCAL_DIR)

MODULES +=

OBJS += \
	$(LOCAL_DIR)/droidboot_init.o \
	$(LOCAL_DIR)/droidboot_logging.o \
	$(LOCAL_DIR)/droidboot_main.o \
	$(LOCAL_DIR)/droidboot_screens.o \
	$(LOCAL_DIR)/droidboot_helpers.o \
	$(LOCAL_DIR)/droidboot_stdfunc.o
