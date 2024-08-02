#pragma once
#include <lvgl.h>
#include <droidboot_config_parser.h>

extern lv_style_t droidboot_list_style;
extern lv_style_t droidboot_win_style;
extern lv_style_t droidboot_list_button_style;
extern lv_style_t droidboot_list_button_selected_style;
//extern lv_style_t droidboot_list_timeout_style;

void droidboot_style_init(struct global_config *global_config);
