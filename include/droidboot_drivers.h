#pragma once
#include <lvgl.h>
#include <droidboot_error.h>

extern lv_indev_t * droidboot_lvgl_indev;

droidboot_ret droidboot_driver_init(void);
bool droidboot_get_sd_fail(void);
