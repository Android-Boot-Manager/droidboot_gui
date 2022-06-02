#include <lvgl.h>
#include <droidboot_error.h>
#include <droidboot_platforms/common/droidboot_platform_common.h>

droidboot_ret droidboot_driver_init(){
    droidboot_ret ret=DROIDBOOT_EOK;
    
    // Run platform init
    ret = droidboot_platform_init();
    if(ret!=DROIDBOOT_EOK)
        retutn ret;
     
    // Initi lvgl   
    lv_init();

    // Set up frame buffer and disp
    static lv_color_t buf_1[droidboot_platfoems_get_hor_res() * 500];
    static lv_color_t buf_2[droidboot_platfoems_get_hor_res() * 500];

	lv_disp_draw_buf_init(&disp_buf, buf_1, buf_2, droidboot_platfoems_get_hor_res()*500);
	static lv_disp_drv_t disp_drv;          /*A variable to hold the drivers. Must be static or global.*/
    lv_disp_drv_init(&disp_drv);            /*Basic initialization*/
	disp_drv.draw_buf = &disp_buf;          /*Set an initialized buffer*/
	disp_drv.flush_cb = droidboot_fb_flush;        /*Set a flush callback to draw to the display*/
	disp_drv.hor_res = droidboot_platfoems_get_hor_res();                 /*Set the horizontal resolution in pixels*/
	disp_drv.ver_res = droidboot_platfoems_get_ver_res();                 /*Set the vertical resolution in pixels*/

	lv_disp_t * disp;
	disp = lv_disp_drv_register(&disp_drv); /*Register the driver and save the created display objects*/
	
	// Set up keys
	lv_indev_drv_init(&indev_drv);      /*Basic initialization*/
    indev_drv.type = LV_INDEV_TYPE_KEYPAD;
    indev_drv.read_cb = droidboot_key_read;
    /*Register the driver in LVGL and save the created input device object*/
    lvgl_mtk_indev = lv_indev_drv_register(&indev_drv);
    
    return ret;
}
