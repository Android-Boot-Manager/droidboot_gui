LOCAL_DIR := $(GET_LOCAL_DIR)

MODULES +=

ifdef OBJS
OBJS += $(LOCAL_DIR)/droidboot_gpt.o
OBJS += $(LOCAL_DIR)/droidboot_mapped.o
else
MODULE_SRCS += $(LOCAL_DIR)/droidboot_gpt.c
MODULE_SRCS += $(LOCAL_DIR)/droidboot_mapped.c
endif
