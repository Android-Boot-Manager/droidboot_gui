LOCAL_DIR := $(GET_LOCAL_DIR)

MODULES +=

ifdef OBJS
OBJS += \
	$(LOCAL_DIR)/droidboot_drivers.o \
	$(LOCAL_DIR)/droidboot_dualboot_backend.o
else
MODULE_SRCS += \
	$(LOCAL_DIR)/droidboot_drivers.c \
	$(LOCAL_DIR)/droidboot_dualboot_backend.c
endif
