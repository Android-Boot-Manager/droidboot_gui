DROIDBOOT_GUI_LOCAL_PATH := $(GET_LOCAL_DIR)

include $(DROIDBOOT_GUI_LOCAL_PATH)/common/common.mk
include $(DROIDBOOT_GUI_LOCAL_PATH)/dualboot_gui/dualboot_gui.mk
include $(DROIDBOOT_GUI_LOCAL_PATH)/config_parser/config_parser.mk
include $(DROIDBOOT_GUI_LOCAL_PATH)/backend/backend.mk
include $(DROIDBOOT_GUI_LOCAL_PATH)/lib/lib.mk
include $(DROIDBOOT_GUI_LOCAL_PATH)/droidboot_platforms/lk.mk

INCLUDES += "-I$(DROIDBOOT_GUI_LOCAL_PATH)"
INCLUDES += "-I$(DROIDBOOT_GUI_LOCAL_PATH)/include" 
