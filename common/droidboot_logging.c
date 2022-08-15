#include <droidboot_platforms/common/droidboot_platform_common.h>
#include <droidboot_error.h>
#include <droidboot_logging.h>
#include <config.h>

extern int DROIDBOOT_LOG_LEVEL;

void droidboot_log(droidboot_log_level log_level, const char *fmt, ...){
    #ifndef DROIDBOOT_NO_LOGGING
    if(log_level>=DROIDBOOT_LOG_LEVEL){
        va_list args;
        #ifdef DROIDBOOT_ON_SCREEN_LOG
            va_start(args, fmt);
            droidboot_platform_on_screen_log(log_level, fmt, args);
            va_end(args);
        #endif
        #ifdef DROIDBOOT_SYSTEM_LOG
            va_start(args, fmt);
            droidboot_platform_system_log(log_level, fmt, args);
            va_end(args);
        #endif
    }
    #endif
}

void droidboot_dump_hex(droidboot_log_level log_level, const void* data, size_t size) {
	char ascii[17];
	size_t i, j;
	ascii[16] = '\0';
	for (i = 0; i < size; ++i) {
		video_printf("%02X ", ((unsigned char*)data)[i]);
		if (((unsigned char*)data)[i] >= ' ' && ((unsigned char*)data)[i] <= '~') {
			ascii[i % 16] = ((unsigned char*)data)[i];
		} else {
			ascii[i % 16] = '.';
		}
		if ((i+1) % 8 == 0 || i+1 == size) {
			video_printf(" ");
			if ((i+1) % 16 == 0) {
				video_printf("|  %s \n", ascii);
			} else if (i+1 == size) {
				ascii[(i+1) % 16] = '\0';
				if ((i+1) % 16 <= 8) {
					video_printf(" ");
				}
				for (j = (i+1) % 16; j < 16; ++j) {
					video_printf("   ");
				}
				video_printf("|  %s \n", ascii);
			}
		}
	}
}
