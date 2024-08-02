#include <lvgl.h>
#include <droidboot_error.h>
#include <droidboot_platforms/common/droidboot_platform_common.h>

/*A static variable to store the buffers*/
static lv_disp_draw_buf_t disp_buf;

static void droidboot_lvgl_print(const char * buf) {
	droidboot_log_level ll = DROIDBOOT_LOG_ERROR;
	if (buf[0] == '[' && buf[1] != '\0' && buf[2] != '\0' && buf[3] != '\0' && buf[4] != '\0') {
		if (buf[5] == ']') {
			if (buf[1] == 'I' && buf[2] == 'n' && buf[3] == 'f' && buf[4] == 'o') {
				ll = DROIDBOOT_LOG_TRACE; // LVGL info is relatively useless and equivalent of our TRACE
			}
			if (buf[1] == 'U' && buf[2] == 's' && buf[3] == 'e' && buf[4] == 'r') {
				ll = DROIDBOOT_LOG_ERROR;
			}
			if (buf[1] == 'W' && buf[2] == 'a' && buf[3] == 'r' && buf[4] == 'n') {
				ll = DROIDBOOT_LOG_WARNING;
			}
		} else if (buf[5] != '\0' && buf[6] == ']') {
			if (buf[1] == 'E' && buf[2] == 'r' && buf[3] == 'r' && buf[4] == 'o' && buf[5] == 'r') {
				ll = DROIDBOOT_LOG_ERROR;
			}
			if (buf[1] == 'T' && buf[2] == 'r' && buf[3] == 'a' && buf[4] == 'c' && buf[5] == 'e') {
				ll = DROIDBOOT_LOG_TRACE;
			}
		}
	}
	droidboot_log(ll, "%s", buf);
}

droidboot_error droidboot_lvgl_init(){
	lv_log_register_print_cb(droidboot_lvgl_print);
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
