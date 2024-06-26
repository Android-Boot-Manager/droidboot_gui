[LibraryClasses]
  droidbootLib|droidbootLib.inf
  droidbootPlatformsLib|droidboot_platforms/droidbootPlatformsLib.inf
  KernelFdtDxe|droidboot_platforms/uefi/common_linuxloader/kernelfdt/KernelFdtDxe.inf
  droidbootRenegadeLib|droidboot_platforms/uefi/common_linuxloader/lib/droidbootRenegadeLib.inf
  droidbootLibcHack|droidboot_platforms/libc-hack/uefi/droidbootLibcHack.inf
  Ext4Lib|lib/ext4/Ext4Lib.inf
  LvglLib|lib/lvgl/LvglLib.inf
  UfdtLib|lib/libufdt/UfdtLib.inf
  MinigzLib|lib/minigz/MinigzLib.inf
  UefiWrapper|droidboot_platforms/uefi/common/uefi-wrapper/UefiWrapper.inf
  DroidbootInitBootLinux|droidboot_platforms/uefi/common_linuxloader/linux-boot/DroidbootInitBootLinux.inf

[Components]
  droidbootLib.inf
  droidboot_platforms/droidbootPlatformsLib.inf
  droidboot_platforms/uefi/common_linuxloader/kernelfdt/KernelFdtDxe.inf
  droidboot_platforms/uefi/common_linuxloader/lib/droidbootRenegadeLib.inf
  droidboot_platforms/uefi/common_linuxloader/linux-boot/DroidbootInitBootLinux.inf
  droidboot_platforms/libc-hack/uefi/droidbootLibcHack.inf
  lib/ext4/Ext4Lib.inf
  lib/lvgl/LvglLib.inf
  lib/libufdt/UfdtLib.inf
  lib/minigz/MinigzLib.inf
  droidboot_platforms/uefi/common/uefi-wrapper/UefiWrapper.inf
