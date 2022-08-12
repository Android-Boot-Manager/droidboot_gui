#include <lvgl.h>
#include <droidboot_screens.h>
#include <res/abm_logo.h>

void droidboot_show_boot_logo()
{
       // lv_obj_clean(lv_scr_act());
        //lv_scr_load(boot_logo);
       // lv_obj_del(setup_main);
       // lv_obj_del(dualboot_menu);
       // lv_obj_clean(lv_scr_act());
        lv_obj_t * boot_logo_img = lv_img_create(lv_scr_act());
        //lv_img_set_src(boot_logo_img, &abm_logo);
        lv_obj_align(boot_logo_img, LV_ALIGN_CENTER, 0, 0);
}
