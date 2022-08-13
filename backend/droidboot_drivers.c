#include <lvgl.h>
#include <droidboot_drivers.h>
#include <droidboot_error.h>
#include <droidboot_platforms/common/droidboot_platform_common.h>
#include <ext4.h>

EXT4_BLOCKDEV_STATIC_INSTANCE(droidboot_settings_dev, 512, 0, droidboot_platform_settings_dev_open,
		droidboot_platform_settings_dev_bread, droidboot_platform_settings_dev_bwrite, droidboot_platform_settings_dev_close, 0, 0); 

droidboot_ret droidboot_driver_init(){
    droidboot_ret ret = DROIDBOOT_EOK;
    
    // Run platform init
    //ret = droidboot_platform_init();
    //if (ret!=DROIDBOOT_EOK)
    //    return ret;
	
	// Set up keys
	/*lv_indev_drv_t indev_drv;
	lv_indev_drv_init(&indev_drv);*/      /* Basic initialization */
    //indev_drv.type = LV_INDEV_TYPE_KEYPAD;
    //indev_drv.read_cb = droidboot_key_read;
    
    /* Register the driver in LVGL and save the created input device object */
   // droidboot_lvgl_indev = lv_indev_drv_register(&indev_drv);
    
    // Launch lvgl threads
	droidboot_lvgl_threads_init();
	
    // Init ext4 stuff
   /* droidboot_settings_dev.part_offset=droidboot_platform_get_storage_part_offset();
    droidboot_settings_dev.bdif->ph_bcnt = droidboot_platform_get_storage_block_count();
    droidboot_settings_dev.part_size = droidboot_platform_get_storage_size();
    ext4_device_register(&droidboot_settings_dev, "droidboot_settings");*/

    return ret;
}
