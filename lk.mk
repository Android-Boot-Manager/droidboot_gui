LOCAL_PATH := $(shell pwd)

include $(LOCAL_PATH)/dualboot_gui/dualboot_gui.mk
include $(LOCAL_PATH)/config_parser/config_parser.mk
include $(LOCAL_PATH)/backend/backend.mk
include $(LOCAL_PATH)/lib/lib.mk

INCLUDES += "-I$(LOCAL_PATH)/include"
