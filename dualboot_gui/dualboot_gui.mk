LOCAL_DIR := $(GET_LOCAL_DIR)

MODULES +=

ifdef OBJS
OBJS += \
	$(LOCAL_DIR)/dualboot_menu.o \
	$(LOCAL_DIR)/droidboot_boot_logo.o
else
MODULE_SRCS += \
	$(LOCAL_DIR)/dualboot_menu.c \
	$(LOCAL_DIR)/droidboot_boot_logo.c
endif
