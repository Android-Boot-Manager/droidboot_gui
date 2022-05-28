#include <droidboot_platforms/common/droidboot_platform_common.h>
#include <droidboot_gui/config.h>

void droidboot_log(droidboot_log_level log_level, const char *fmt, ...){
    #ifndef DROIDBOOT_NO_LOGGING
    if(log_level>=DROIDBOOT_LOG_LEVEL){
        va_list args;
        #ifdef DROIDBOOT_ON_SCREEN_LOG
            va_start(args, fmt);
            droidboot_platform_on_screen_log(fmt, args);
            va_end(args);
        #endif
        #ifdef DROIDBOOT_SYSTEM_LOG
            va_start(args, fmt);
            droidboot_platform_system_log(fmt, args);
            va_end(args);
        #endif
    }
    #endif
}
