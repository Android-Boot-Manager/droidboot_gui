[LibraryClasses]
  droidbootLib|droidbootLib.inf
  droidbootPlatformsLib|droidboot_platforms/droidbootPlatformsLib.inf
  KernelFdtDxe|droidboot_platforms/uefi/common_linuxloader/kernelfdt/KernelFdtDxe.inf
  droidbootRenegadeLib|droidboot_platforms/uefi/common_linuxloader/lib/droidbootRenegadeLib.inf
  droidbootLibcHack|droidboot_platforms/libc-hack/uefi/droidbootLibcHack.inf

[Components]
  droidbootLib.inf
  droidboot_platforms/droidbootPlatformsLib.inf
  droidboot_platforms/uefi/common_linuxloader/kernelfdt/KernelFdtDxe.inf
  droidboot_platforms/uefi/common_linuxloader/lib/droidbootRenegadeLib.inf
  droidboot_platforms/libc-hack/uefi/droidbootLibcHack.inf
