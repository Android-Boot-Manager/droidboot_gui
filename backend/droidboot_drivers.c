#include <lvgl.h>
#include <ext4.h>
#include <droidboot_drivers.h>
#include <droidboot_error.h>
#include <droidboot_gpt.h>
#include <droidboot_config.h>
#include <droidboot_mapped.h>
#include <droidboot_platforms/common/droidboot_platform_common.h>

bool sdfail;

#ifdef DROIDBOOT_NO_SD_ENCRYPTED_SUPPORT
file_metadata abm_settings_metadata;
#endif

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

#ifdef DROIDBOOT_NO_SD_ENCRYPTED_SUPPORT
static int droidboot_lwext_logical_dev_bread(struct ext4_blockdev *bdev, void *buf, uint32_t blk_id, uint32_t blk_cnt)
{
    droidboot_read_mapped_file_blocks(&abm_settings_metadata, blk_id, blk_cnt, buf);
    return DROIDBOOT_EOK;
}
static int droidboot_lwext_logical_dev_bwrite(struct ext4_blockdev *bdev, const void *buf, uint32_t blk_id, uint32_t blk_cnt)
{
 //   droidboot_read_mapped_file_blocks(bdev->bdif->p_user, blk_id, blk_cnt, buf);
    return DROIDBOOT_EOK;
}
#endif

EXT4_BLOCKDEV_STATIC_INSTANCE(droidboot_abm_settings_dev, 512, 0, droidboot_platform_settings_dev_open,
		 droidboot_lwext_sd_dev_bread, droidboot_lwext_sd_dev_bwrite, droidboot_platform_settings_dev_close, 0, 0);

EXT4_BLOCKDEV_STATIC_INSTANCE(droidboot_userdata_settings_dev, 4096, 0, droidboot_platform_settings_dev_open,
		 droidboot_lwext_sd_dev_bread, droidboot_lwext_sd_dev_bwrite, droidboot_platform_settings_dev_close, 0, 0);

#ifdef DROIDBOOT_NO_SD_ENCRYPTED_SUPPORT
EXT4_BLOCKDEV_STATIC_INSTANCE(droidboot_metadata_dev, 4096, 0, droidboot_platform_settings_dev_open,
		 droidboot_lwext_sd_dev_bread, droidboot_lwext_sd_dev_bwrite, droidboot_platform_settings_dev_close, 0, 0);
#endif

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
	if(droidboot_parse_gpt_on_sd()==DROIDBOOT_EOK && abm_settings_offset!=0){
	    droidboot_abm_settings_dev.part_offset = abm_settings_offset * droidboot_sd_blklen();
        droidboot_abm_settings_dev.bdif->ph_bsize = droidboot_sd_blklen();
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
    }
#ifdef DROIDBOOT_NO_SD_ENCRYPTED_SUPPORT
    else if(userdata_offset!=0 && metadata_offset!=0){
        droidboot_metadata_dev.part_offset = metadata_offset * droidboot_sd_blklen();
        droidboot_metadata_dev.bdif->ph_bsize = droidboot_sd_blklen();
        droidboot_metadata_dev.bdif->ph_bcnt = metadata_blkcnt;
        droidboot_metadata_dev.part_size = metadata_blkcnt*droidboot_sd_blklen();
        ext4_device_register(&droidboot_metadata_dev, "metadata");
        droidboot_log(DROIDBOOT_LOG_INFO, "Registered metadata, offset %d, bcnt: %llu\n", metadata_offset, metadata_blkcnt);
        droidboot_log(DROIDBOOT_LOG_TRACE, "going to mount metadata\n");
        ret = ext4_mount("metadata", "/metadata/", false);
        if(ret!=DROIDBOOT_EOK){
            sdfail=true;
            droidboot_log(DROIDBOOT_LOG_ERROR, "Ext4 mount returns: %d\n", ret);
            return ret;
        }

        abm_settings_metadata = droidboot_read_file_metadata("/metadata/abm_settings.map", userdata_offset);
        droidboot_abm_settings_dev.part_offset = 0;
        droidboot_abm_settings_dev.bdif->ph_bsize = droidboot_sd_blklen();
        droidboot_abm_settings_dev.bdif->ph_bcnt = abm_settings_metadata.size / abm_settings_metadata.block_size;
        droidboot_abm_settings_dev.part_size = abm_settings_metadata.size;
        droidboot_abm_settings_dev.bdif->p_user = &abm_settings_metadata;
        droidboot_abm_settings_dev.bdif->bread = &droidboot_lwext_logical_dev_bread;
        droidboot_abm_settings_dev.bdif->bwrite = &droidboot_lwext_logical_dev_bwrite;
        ext4_device_register(&droidboot_abm_settings_dev, "abm_settings");
        droidboot_log(DROIDBOOT_LOG_INFO, "Registered abm settings, offset %d, bcnt: %llu\n", abm_settings_offset, abm_settings_blkcnt);
        droidboot_log(DROIDBOOT_LOG_TRACE, "going to mount abm settings\n");
        int r=ext4_mount("abm_settings", "/boot/", false);
        if(r!=DROIDBOOT_EOK){
            sdfail=true;
        }
        droidboot_log(DROIDBOOT_LOG_TRACE, "Ext4 mount returns: %d\n", r);
    }
#endif
#ifdef DROIDBOOT_NO_SD_SUPPORT
    else if(userdata_offset!=0){
        droidboot_userdata_settings_dev.part_offset = userdata_offset * droidboot_sd_blklen();
        droidboot_userdata_settings_dev.bdif->ph_bsize = droidboot_sd_blklen();
        droidboot_userdata_settings_dev.bdif->ph_bcnt = userdata_blkcnt;
        droidboot_userdata_settings_dev.part_size = abm_settings_blkcnt*droidboot_sd_blklen();
        ext4_device_register(&droidboot_userdata_settings_dev, "abm_settings");
        droidboot_log(DROIDBOOT_LOG_INFO, "Registered userdata, offset %d, bcnt: %llu\n", userdata_offset, userdata_blkcnt);
        droidboot_log(DROIDBOOT_LOG_TRACE, "going to mount userdata\n");
        int r=ext4_mount("abm_settings", "/boot/", false);
        if(r!=DROIDBOOT_EOK){
            sdfail=true;
        }
        droidboot_log(DROIDBOOT_LOG_TRACE, "Ext4 mount returns: %d\n", r);
    }
#endif
	else {
	    sdfail=true;
	}

    return ret;
}

bool droidboot_get_sd_fail()
{
    return sdfail;
}
