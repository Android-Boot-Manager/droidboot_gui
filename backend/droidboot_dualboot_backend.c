#include <lvgl.h>
#include <ext4.h>

#include <droidboot_drivers.h>
#include <droidboot_screens.h>
#include <droidboot_config_parser.h>

// droidboot_platforms_common.h is part of droidboot_platform and contains all functions used by droidboot gui
#include <droidboot_platforms/common/droidboot_platform_common.h>

void droidboot_boot_linux_from_ext4(struct boot_entry *entry)
{
    off_t kernel_raw_size = 0;
	off_t ramdisk_size = 0;
	off_t dtb_size = 0;
	unsigned int dev_null;
	size_t rb;
	ext4_file fp;
	char *linux;
    char *initrd;
    char *dtb;
    char *options;
    unsigned char *kernel_raw;
    unsigned char *ramdisk_raw;
    linux = malloc(strlen("/boot/") + strlen(entry->linux) + 1);
	initrd = malloc(strlen("/boot/") + strlen(entry->initrd) + 1);
    dtb = malloc(strlen("/boot/") + strlen(entry->dtb) + 1);
    options = malloc(strlen(entry->options) + 1);
    strcpy(linux, "/boot/");
	strcat(linux, entry->linux);
	strcpy(initrd, "/boot/");
	strcat(initrd, entry->initrd);
    strcpy(dtb, "/boot/");
	strcat(dtb, entry->dtb);
	strcpy(options, entry->options);
	video_printf("booting from ext2 partition 'system'\n");

    if(ext4_fopen (&fp, linux, "r")!=0)
    {
        video_printf("Failed to open linux( %s\n", linux);
    }
    ext4_fseek(&fp, 0, SEEK_END);
	kernel_raw_size = ext4_ftell(&fp);
	video_printf("%s size %d\n", linux, kernel_raw_size);
	ext4_fseek(&fp, 0, SEEK_SET);  /* same as rewind(f); */
	if(!kernel_raw_size) {
		video_printf("kernbel get size failed, path: %s\n, ramdisk path:%s",linux, initrd);
		return -1;
	}
	
    video_printf("kernel get size done\n");
    if(droidboot_get_kernel_load_addr()==NULL)
        kernel_raw = malloc(kernel_raw_size);
    else
        kernel_raw = droidboot_get_kernel_load_addr();
    video_printf("malloc done\n");
	if(ext4_fread(&fp, kernel_raw, kernel_raw_size, NULL) < 0) {
		video_printf("kernel load failed\n");
		return -1;
	}
	video_printf("kernel load ok\n");
	ext4_fclose(&fp);
    video_printf("kernel load ok\n");
    droidboot_dump_hex(DROIDBOOT_LOG_TRACE, (void *)kernel_raw, 16);

    ext4_fopen (&fp, initrd, "r");
    ext4_fseek(&fp, 0, SEEK_END);
	ramdisk_size = ext4_ftell(&fp);
	ext4_fseek(&fp, 0, SEEK_SET);  /* same as rewind(f); */
	if (!ramdisk_size) {
	video_printf("ramdisk get size failed\n");
		return -1;
	}
    video_printf("ramdisk get size ok\n");
    if(droidboot_get_ramdisk_load_addr()==NULL){
        ramdisk_raw = malloc(ramdisk_size);
        if(droidboot_append_ramdisk_to_kernel())
        {
           ramdisk_raw = kernel_raw+kernel_raw_size;
        }
    } else
        ramdisk_raw = droidboot_get_ramdisk_load_addr();

	if(ext4_fread(&fp, ramdisk_raw, ramdisk_size, &rb) < 0) {
	video_printf("ramdisk load failed\n");
        return -1;
	}
	ext4_fclose(&fp);
	droidboot_dump_hex(DROIDBOOT_LOG_TRACE, (void *)ramdisk_raw, 16);
    video_printf("ramdisk load ok\n");

    // time to umount fs
    ext4_cache_write_back("/boot/", 0);
    ext4_umount("/boot/");
	droidboot_platform_boot_linux_from_ram(kernel_raw, kernel_raw_size, ramdisk_raw, ramdisk_size, NULL, NULL, options);
}
