#include <lvgl.h>
#include <droidboot_error.h>
#include <droidboot_platforms/common/droidboot_platform_common.h>

/*A static variable to store the buffers*/
static lv_disp_draw_buf_t disp_buf;

droidboot_error droidboot_lvgl_init(){
    lv_init();
    
    /*Use double buffering*/
    static lv_color_t buf_1[720 * 30];
    if(droidboot_use_double_buffering()){
        static lv_color_t buf_2[720 * 30];
        lv_disp_draw_buf_init(&disp_buf, buf_1, buf_2, 720*30);
    }
    else
        lv_disp_draw_buf_init(&disp_buf, buf_1, NULL, 720*30);

	static lv_disp_drv_t disp_drv;          /*A variable to hold the drivers. Must be static or global.*/
    lv_disp_drv_init(&disp_drv);            /*Basic initialization*/
	disp_drv.draw_buf = &disp_buf;          /*Set an initialized buffer*/
	disp_drv.flush_cb = droidboot_fb_flush;        /*Set a flush callback to draw to the display*/
	disp_drv.hor_res = droidboot_get_display_width();                 /*Set the horizontal resolution in pixels*/
	disp_drv.ver_res = droidboot_get_display_height();                 /*Set the vertical resolution in pixels*/

	lv_disp_t * disp;
	disp = lv_disp_drv_register(&disp_drv); /*Register the driver and save the created display objects*/
	return DROIDBOOT_EOK;
}
