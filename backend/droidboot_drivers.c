#include <lvgl.h>
#include <ext4.h>
#include <droidboot_drivers.h>
#include <droidboot_error.h>
#include <droidboot_gpt.h>
#include <droidboot_config.h>
#include <droidboot_platforms/common/droidboot_platform_common.h>


// Next functions are only for lwext4, usually just wrappers
static int droidboot_lwext_sd_dev_bread(struct ext4_blockdev *bdev, void *buf, uint32_t blk_id, uint32_t blk_cnt)
{
    dridboot_sd_read_block(buf, blk_id, blk_cnt);
    return DROIDBOOT_EOK;
}

static int droidboot_lwext_sd_dev_bwrite(struct ext4_blockdev *bdev, const void *buf,
			  uint32_t blk_id, uint32_t blk_cnt){
    dridboot_sd_write_block(buf, blk_id, blk_cnt);
    return DROIDBOOT_EOK;
}

EXT4_BLOCKDEV_STATIC_INSTANCE(droidboot_abm_settings_dev, 512, 0, droidboot_platform_settings_dev_open,
		 droidboot_lwext_sd_dev_bread, droidboot_lwext_sd_dev_bwrite, droidboot_platform_settings_dev_close, 0, 0); 

static char *entry_to_str(uint8_t type)
{
	switch (type) {
	case EXT4_DE_UNKNOWN:
		return "[unk] ";
	case EXT4_DE_REG_FILE:
		return "[fil] ";
	case EXT4_DE_DIR:
		return "[dir] ";
	case EXT4_DE_CHRDEV:
		return "[cha] ";
	case EXT4_DE_BLKDEV:
		return "[blk] ";
	case EXT4_DE_FIFO:
		return "[fif] ";
	case EXT4_DE_SOCK:
		return "[soc] ";
	case EXT4_DE_SYMLINK:
		return "[sym] ";
	default:
		break;
	}
	return "[???]";
}

droidboot_ret droidboot_driver_init(){
    droidboot_ret ret = DROIDBOOT_EOK;
    
    // Run platform init
    ret = droidboot_platform_init();
    if (ret!=DROIDBOOT_EOK)
        return ret;
	
	// Set up keys
	/*lv_indev_drv_t indev_drv;
	lv_indev_drv_init(&indev_drv);*/      /* Basic initialization */
    //indev_drv.type = LV_INDEV_TYPE_KEYPAD;
    //indev_drv.read_cb = droidboot_key_read;
    
    /* Register the driver in LVGL and save the created input device object */
   // droidboot_lvgl_indev = lv_indev_drv_register(&indev_drv);
    
    // Launch lvgl threads
	droidboot_lvgl_threads_init();
	if(droidboot_parse_gpt_on_sd()==DROIDBOOT_EOK){
	    droidboot_abm_settings_dev.part_offset = abm_settings_offset * 512;
        droidboot_abm_settings_dev.bdif->ph_bcnt = abm_settings_blkcnt;
        droidboot_abm_settings_dev.part_size = abm_settings_blkcnt*512;
        ext4_device_register(&droidboot_abm_settings_dev, "abm_settings");
        droidboot_log(DROIDBOOT_LOG_INFO, "Registered abm settings, offset %d, bcnt: %llu\n", abm_settings_offset, abm_settings_blkcnt);
        if(DROIDBOOT_LOG_LEVEL==0){
            droidboot_log(DROIDBOOT_LOG_TRACE, "going to mount abm settings\n");
            int r=ext4_mount("abm_settings", "/boot/", false);
            droidboot_log(DROIDBOOT_LOG_TRACE, "Ext4 mount returns: %d\n", r);
            if(r==DROIDBOOT_EOK){
                char sss[255];
	            ext4_dir d;
	            const ext4_direntry *de;

	            droidboot_log(DROIDBOOT_LOG_TRACE, "ls %s\n", "/boot");

	            ext4_dir_open(&d, "/boot/");
	            de = ext4_dir_entry_next(&d);

	            while (de) {
		            memcpy(sss, de->name, de->name_length);
		            sss[de->name_length] = 0;
		            droidboot_log(DROIDBOOT_LOG_TRACE, "  %s%s\n", entry_to_str(de->inode_type), sss);
		            de = ext4_dir_entry_next(&d);
	            }
	            ext4_dir_close(&d);
	           // ext4_umount("/boot/");
	        }
        }
	}

    return ret;
}
