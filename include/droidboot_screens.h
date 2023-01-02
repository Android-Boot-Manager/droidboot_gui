#pragma once
#include <lvgl.h>

extern lv_obj_t *boot_logo;
extern lv_obj_t *fastboot;
extern lv_obj_t *setup_main;
extern lv_obj_t *setup_about;
extern lv_obj_t *dualboot_menu;

void screens_init(void);
