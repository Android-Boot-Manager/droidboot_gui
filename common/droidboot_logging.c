#include <lvgl.h>
#include <droidboot_main.h>
#include <droidboot_platforms/common/droidboot_platform_common.h>
#include <droidboot_error.h>
#include <droidboot_logging.h>
#include <droidboot_config.h>

void droidboot_show_error(const char *buf)
{
	if(get_lvgl_init_done()) {
		lv_obj_t * error_label = lv_label_create(lv_scr_act());
		lv_label_set_long_mode(error_label, LV_LABEL_LONG_WRAP);
		lv_obj_set_style_text_align(error_label, LV_TEXT_ALIGN_CENTER, 0);
		lv_obj_align(error_label, LV_ALIGN_CENTER, 0, 0);
		lv_label_set_text(error_label, buf);
		while(1)
			droidboot_platform_tasks();
	}
}
void droidboot_log(droidboot_log_level log_level, const char *fmt, ...){
    #ifndef DROIDBOOT_NO_LOGGING
    if(log_level>=DROIDBOOT_LOG_LEVEL){
        va_list args;
        char printbuffer[512];
	    if(log_level>=DROIDBOOT_ON_SCREEN_LOG) {
		    va_start(args, fmt);
		    vsnprintf(printbuffer, 512, fmt, args);
		    va_end(args);
		    droidboot_platform_on_screen_log(printbuffer);
	    }
        #ifdef DROIDBOOT_SYSTEM_LOG
	        va_start(args, fmt);
	        vsnprintf(printbuffer, 512, fmt, args);
	        va_end(args);
            droidboot_platform_system_log(printbuffer);
        #endif
		va_start(args, fmt);
		vsnprintf(printbuffer, 512, fmt, args);
		va_end(args);
		if(log_level == DROIDBOOT_LOG_FATAL)
			droidboot_show_error(printbuffer);
    }
    #endif
}

void droidboot_dump_hex(droidboot_log_level log_level, const void* data, size_t size) {
    if(log_level>=DROIDBOOT_LOG_LEVEL){
	    char ascii[17];
	    size_t i, j;
	    ascii[16] = '\0';
	    for (i = 0; i < size; ++i) {
	    	droidboot_log(log_level, "%02X ", ((unsigned char*)data)[i]);
		    if (((unsigned char*)data)[i] >= ' ' && ((unsigned char*)data)[i] <= '~') {
			    ascii[i % 16] = ((unsigned char*)data)[i];
		    } else {
			    ascii[i % 16] = '.';
		    }
		    if ((i+1) % 8 == 0 || i+1 == size) {
			    droidboot_log(log_level, " ");
			    if ((i+1) % 16 == 0) {
				    droidboot_log(log_level, "|  %s \n", ascii);
			    } else if (i+1 == size) {
				    ascii[(i+1) % 16] = '\0';
				    if ((i+1) % 16 <= 8) {
				    	droidboot_log(log_level, " ");
				    }
				    for (j = (i+1) % 16; j < 16; ++j) {
				    	droidboot_log(log_level, "   ");
				    }
				    droidboot_log(log_level, "|  %s \n", ascii);
			    }
		    }
	    }
    }
}
