#include <lvgl.h>
#include <droidboot_error.h>
#include <droidboot_init.h>
#include <droidboot_platforms/common/droidboot_platform_common.h>

void droidboot_init()
{
    droidboot_lvgl_init();
    droidboot_driver_init();
}

droidboot_error droidboot_show_dualboot_menu()
{
    droidboot_error ret = droidboot_check_metadata();
    if(ret)
        return ret;
   
    ret = droidboot_parse_config();
    if(ret)
        return ret;
     
    ret = droidboot_show_menu();   
    if(ret)
        return ret;
}
