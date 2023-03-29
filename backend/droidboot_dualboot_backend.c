#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <lvgl.h>
#include <ext4.h>

#include <droidboot_drivers.h>
#include <droidboot_screens.h>
#include <droidboot_config_parser.h>
#include <droidboot_dtb.h>

// droidboot_platforms_common.h is part of droidboot_platform and contains all functions used by droidboot gui
#include <droidboot_platforms/common/droidboot_platform_common.h>

void droidboot_boot_linux_from_ext4(struct boot_entry *entry)
{
    off_t kernel_raw_size = 0;
    off_t ramdisk_size = 0;
    off_t dtb_size = 0;
    off_t dtbo_size = 0;
    unsigned int dev_null;
    size_t rb;
    ext4_file fp;
    char *kernel;
    char *initrd;
    char *dtb;
    char *dtbo;
    char *options;
    unsigned char *kernel_raw;
    unsigned char *ramdisk_raw;
    unsigned char *dtb_raw;
    unsigned char *dtbo_raw;
    kernel = malloc(strlen("/boot/") + strlen(entry->kernel) + 1);
    initrd = malloc(strlen("/boot/") + strlen(entry->initrd) + 1);
    dtb = malloc(strlen("/boot/") + strlen(entry->dtb) + 1);
    dtbo = malloc(strlen("/boot/") + strlen(entry->dtbo) + 1);
    options = malloc(strlen(entry->options) + 1);
    strcpy(kernel, "/boot/");
    strcat(kernel, entry->kernel);
    strcpy(initrd, "/boot/");
    strcat(initrd, entry->initrd);
    strcpy(dtb, "/boot/");
    strcat(dtb, entry->dtb);
    if(entry->dtbo!=NULL){
        strcpy(dtbo, "/boot/");
        strcat(dtbo, entry->dtbo);
    } else dtbo=NULL;
    strcpy(options, entry->options);
    droidboot_log(DROIDBOOT_LOG_TRACE, "booting from ext4 partition 'abm_settings'\n");
    droidboot_log(DROIDBOOT_LOG_TRACE, "Going to boot, kernel:%s, initrd: %s, dtb: %s, dtbo:%s\n", kernel, initrd, dtb, dtbo);
    if(ext4_fopen (&fp, kernel, "r")!=0)
        droidboot_log(DROIDBOOT_LOG_ERROR, "Failed to open linux( %s\n", kernel);

    ext4_fseek(&fp, 0, SEEK_END);
	kernel_raw_size = ext4_ftell(&fp);
	droidboot_log(DROIDBOOT_LOG_INFO, "%s size %d\n", kernel, kernel_raw_size);
	ext4_fseek(&fp, 0, SEEK_SET); /* same as rewind(f); */
	if(!kernel_raw_size) {
		droidboot_log(DROIDBOOT_LOG_ERROR, "kernnel get size failed, path: %s\n, ramdisk path:%s",kernel, initrd);
		return;
	}
	
    droidboot_log(DROIDBOOT_LOG_TRACE, "kernel get size done\n");
    if(droidboot_get_kernel_load_addr()==NULL)
        kernel_raw = malloc(kernel_raw_size);
    else{
		droidboot_log(DROIDBOOT_LOG_TRACE, "loading kernel at addr %d\n", droidboot_get_kernel_load_addr());
        kernel_raw = droidboot_get_kernel_load_addr();
	}

	if(ext4_fread(&fp, kernel_raw, kernel_raw_size, NULL) < 0) {
	    droidboot_log(DROIDBOOT_LOG_ERROR, "kernel load failed\n");
		return;
	}
	droidboot_log(DROIDBOOT_LOG_TRACE, "kernel load ok\n");
	ext4_fclose(&fp);
    droidboot_dump_hex(DROIDBOOT_LOG_TRACE, (void *)kernel_raw, 16);

    droidboot_pre_ramdisk_load(kernel_raw, kernel_raw_size);

    ext4_fopen (&fp, initrd, "r");
    ext4_fseek(&fp, 0, SEEK_END);
	ramdisk_size = ext4_ftell(&fp);
	ext4_fseek(&fp, 0, SEEK_SET); /* same as rewind(f); */
	if (!ramdisk_size) {
	    droidboot_log(DROIDBOOT_LOG_ERROR, "ramdisk get size failed\n");
		return;
	}
    droidboot_log(DROIDBOOT_LOG_TRACE, "ramdisk get size ok\n");
    if(droidboot_get_ramdisk_load_addr()==NULL){
        ramdisk_raw = malloc(ramdisk_size);
        if(droidboot_append_ramdisk_to_kernel())
        {
           ramdisk_raw = kernel_raw+kernel_raw_size;
        }
    } else {
		droidboot_log(DROIDBOOT_LOG_INFO, "loading ramdisk at addr %d\n", droidboot_get_ramdisk_load_addr());
        ramdisk_raw = droidboot_get_ramdisk_load_addr();
	}

	if(ext4_fread(&fp, ramdisk_raw, ramdisk_size, &rb) < 0) {
	    droidboot_log(DROIDBOOT_LOG_ERROR, "ramdisk load failed\n");
        return;
	}
	ext4_fclose(&fp);
	droidboot_dump_hex(DROIDBOOT_LOG_TRACE, (void *)ramdisk_raw, 16);
    droidboot_log(DROIDBOOT_LOG_TRACE, "ramdisk load ok\n");

    if(droidboot_get_dtb_load_addr()!=NULL){
        dtb_raw=droidboot_get_dtb_load_addr();
        droidboot_log(DROIDBOOT_LOG_ERROR, "going to load dtb\n");
        ext4_fopen (&fp, dtb, "r");
        ext4_fseek(&fp, 0, SEEK_END);
	    dtb_size = ext4_ftell(&fp);
	    ext4_fseek(&fp, 0, SEEK_SET); /* same as rewind(f); */
	    if (!dtb_size) {
	        droidboot_log(DROIDBOOT_LOG_ERROR, "dtb get size failed, path: %s\n", dtb);
		    return;
	    }
		if(droidboot_get_dtb_load_addr()==1){
			dtb_raw=malloc(dtb_size);
		}
	    if(ext4_fread(&fp, dtb_raw, dtb_size, &rb) < 0) {
	        droidboot_log(DROIDBOOT_LOG_ERROR, "dtb load failed\n");
           return;
	    }
	    droidboot_log(DROIDBOOT_LOG_TRACE, "dtb load done\n");
		droidboot_dump_hex(DROIDBOOT_LOG_TRACE, (void *)dtb_raw, 16);
    } else {
        dtb_size=0;
    }

    if(droidboot_get_dtbo_load_addr()!=NULL && dtbo!=NULL){
        dtbo_raw=droidboot_get_dtbo_load_addr();
        droidboot_log(DROIDBOOT_LOG_ERROR, "going to load dtbo\n");
        ext4_fopen (&fp, dtbo, "r");
        ext4_fseek(&fp, 0, SEEK_END);
	    dtbo_size = ext4_ftell(&fp);
	    ext4_fseek(&fp, 0, SEEK_SET); /* same as rewind(f); */
	    if (!dtbo_size) {
	        droidboot_log(DROIDBOOT_LOG_ERROR, "dtbo get size failed, path: %s\n", dtbo);
		    return;
	    }
		if(droidboot_get_dtbo_load_addr()==1){
			dtbo_raw=malloc(dtbo_size);
		}
	    if(ext4_fread(&fp, dtbo_raw, dtbo_size, &rb) < 0) {
	       droidboot_log(DROIDBOOT_LOG_ERROR, "dtbo load failed\n");
           return;
	    }
	    droidboot_log(DROIDBOOT_LOG_TRACE, "dtbo load done\n");
		droidboot_dump_hex(DROIDBOOT_LOG_TRACE, (void *)dtbo_raw, 16);
    } else {
        dtbo_size=0;
    }


    // time to umount fs
    ext4_cache_write_back("/boot/", 0);
    ext4_umount("/boot/");
	droidboot_log(DROIDBOOT_LOG_TRACE, "Going to boot linux\n");
	droidboot_platform_boot_linux_from_ram(kernel_raw, kernel_raw_size, ramdisk_raw, ramdisk_size, dtb_raw, dtb_size, dtbo_raw, dtbo_size, options);
}
