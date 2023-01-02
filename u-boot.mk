# SPDX-License-Identifier: GPL-2.0+
#

obj-y += common/
obj-y += dualboot_gui/
obj-y += config_parser/
obj-y += backend/
obj-y += lib/
obj-y += droidboot_platforms/
obj-y += storage/

KBUILD_CFLAGS +=-I$(VPATH)/lib/droidboot/include -I$(VPATH)/lib/droidboot -I$(VPATH)/lib/droidboot/droidboot_platforms/libc-hack -I$(VPATH)/lib/droidboot/droidboot_platforms/libc-hack/u-boot -I$(VPATH)/lib/droidboot/lib/lvgl -I$(VPATH)/lib/droidboot/lib/ext4/include -I$(VPATH)/lib/droidboot/droidboot_platforms/u-boot -DPLATFORM_UBOOT=1
