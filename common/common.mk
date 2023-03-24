LOCAL_DIR := $(GET_LOCAL_DIR)

MODULES +=

ifdef OBJS
OBJS += \
	$(LOCAL_DIR)/droidboot_init.o \
	$(LOCAL_DIR)/droidboot_logging.o \
	$(LOCAL_DIR)/droidboot_main.o \
	$(LOCAL_DIR)/droidboot_screens.o \
	$(LOCAL_DIR)/droidboot_helpers.o \
	$(LOCAL_DIR)/droidboot_stdfunc.o \
	$(LOCAL_DIR)/droidboot_theme.o
else
MODULE_SRCS += \
	$(LOCAL_DIR)/droidboot_init.c \
	$(LOCAL_DIR)/droidboot_logging.c \
	$(LOCAL_DIR)/droidboot_main.c \
	$(LOCAL_DIR)/droidboot_screens.c \
	$(LOCAL_DIR)/droidboot_helpers.c \
	$(LOCAL_DIR)/droidboot_stdfunc.c \
	$(LOCAL_DIR)/droidboot_theme.c
endif
