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
        lv_obj_t * label1 = lv_label_create(lv_scr_act());
    
    lv_label_set_long_mode(label1, LV_LABEL_LONG_WRAP);     /*Break the long lines*/
    lv_label_set_recolor(label1, true);                      /*Enable re-coloring by commands in the text*/
    lv_label_set_text(label1, "#0000ff Re-color# #ff00ff words# #ff0000 of a# label, align the lines to the center "
                              "and wrap long text automatically.");
    lv_obj_set_width(label1, 150);  /*Set smaller width to make the lines wrap*/
    lv_obj_set_style_text_align(label1, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(label1, LV_ALIGN_CENTER, 0, -40);
    lv_tick_inc(1);
    lv_timer_handler();
}
