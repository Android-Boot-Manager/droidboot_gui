#include <lvgl.h>
#include <ext4.h>
#include <droidboot_drivers.h>
#include <droidboot_error.h>
#include <droidboot_gpt.h>
#include <droidboot_config.h>
#include <droidboot_platforms/common/droidboot_platform_common.h>

bool sdfail;

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

EXT4_BLOCKDEV_STATIC_INSTANCE(droidboot_abm_settings_dev, 4096, 0, droidboot_platform_settings_dev_open,
		 droidboot_lwext_sd_dev_bread, droidboot_lwext_sd_dev_bwrite, droidboot_platform_settings_dev_close, 0, 0);

droidboot_ret droidboot_driver_init(){
    droidboot_ret ret = DROIDBOOT_EOK;
    sdfail=false;
    
    static lv_indev_drv_t indev_drv1;
    lv_indev_drv_init(&indev_drv1);      /*Basic initialization*/
    indev_drv1.type =LV_INDEV_TYPE_KEYPAD;                /*See below.*/
    indev_drv1.read_cb =droidboot_key_read;            /*See below.*/
    /*Register the driver in LVGL and save the created input device object*/
    lv_indev_t * droidboot_indev = lv_indev_drv_register(&indev_drv1);
    
    lv_group_t * droidboot_group = lv_group_create(); 
    lv_group_set_default(droidboot_group);
    lv_indev_set_group(droidboot_indev, droidboot_group);
    
    // Launch lvgl threads
	droidboot_lvgl_threads_init();
	if(droidboot_parse_gpt_on_sd()==DROIDBOOT_EOK){
	    droidboot_abm_settings_dev.part_offset = abm_settings_offset * droidboot_sd_blklen();
        droidboot_abm_settings_dev.bdif->ph_bcnt = abm_settings_blkcnt;
        droidboot_abm_settings_dev.part_size = abm_settings_blkcnt*droidboot_sd_blklen();
        ext4_device_register(&droidboot_abm_settings_dev, "abm_settings");
        droidboot_log(DROIDBOOT_LOG_INFO, "Registered abm settings, offset %d, bcnt: %llu\n", abm_settings_offset, abm_settings_blkcnt);
        droidboot_log(DROIDBOOT_LOG_TRACE, "going to mount abm settings\n");
        int r=ext4_mount("abm_settings", "/boot/", false);
        if(r!=DROIDBOOT_EOK){
            sdfail=true;
        }
        droidboot_log(DROIDBOOT_LOG_TRACE, "Ext4 mount returns: %d\n", r);
	} else {
	    sdfail=true;
	}

    return ret;
}

bool droidboot_get_sd_fail()
{
    return sdfail;
}
