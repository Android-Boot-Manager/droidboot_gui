#include <lvgl.h>
#include <droidboot_config_parser.h>
#include <droidboot_theme.h>
#include <droidboot_logging.h>

lv_style_t droidboot_list_style;
lv_style_t droidboot_win_style;
lv_style_t droidboot_list_button_style;
lv_style_t droidboot_list_button_selected_style;
//lv_style_t droidboot_list_timeout_style;

void droidboot_style_init(struct global_config *global_config){
    droidboot_log(DROIDBOOT_LOG_TRACE, "Current theme settings: radius: %d, bg_color: %llx, border_width: %d, border_color: %llx \n", global_config->radius, global_config->bg_color, global_config->border_width, global_config->border_color);
    lv_style_init(&droidboot_list_style);
    lv_style_set_radius(&droidboot_list_style, global_config->win_radius);
    lv_style_set_bg_color(&droidboot_list_style, lv_color_hex(global_config->win_bg_color));
    lv_style_set_border_width(&droidboot_list_style, global_config->win_border_size);
    lv_style_set_border_color(&droidboot_list_style, lv_color_hex(global_config->win_border_color));

    lv_style_init(&droidboot_win_style);
    lv_style_set_radius(&droidboot_win_style, global_config->list_radius);
    lv_style_set_bg_color(&droidboot_win_style, lv_color_hex(global_config->list_bg_color));
    lv_style_set_border_width(&droidboot_win_style, global_config->list_border_size);
    lv_style_set_border_color(&droidboot_win_style, lv_color_hex(global_config->list_border_color));

    lv_style_init(&droidboot_list_button_style);
    lv_style_set_radius(&droidboot_list_button_style, global_config->button_unselected_radius);
    lv_style_set_text_color(&droidboot_list_button_style, lv_color_hex(global_config->button_text_color));
    lv_style_set_bg_color(&droidboot_list_button_style, lv_color_hex(global_config->button_color));
    lv_style_set_border_width(&droidboot_list_button_style, global_config->button_border_width);
    lv_style_set_border_color(&droidboot_list_button_style, lv_color_hex(global_config->button_border_color));
    if(1)
        lv_style_set_transform_width(&droidboot_list_button_style, 100);

    lv_style_init(&droidboot_list_button_selected_style);
    lv_style_set_radius(&droidboot_list_button_selected_style, global_config->button_selected_radius);
    lv_style_set_border_color(&droidboot_list_button_selected_style, lv_color_hex(global_config->border_selected_color));
    lv_style_set_text_color(&droidboot_list_button_selected_style, lv_color_hex(global_config->button_selected_text_color));
    lv_style_set_bg_color(&droidboot_list_button_selected_style, lv_color_hex(global_config->button_selected_color));
    lv_style_set_border_width(&droidboot_list_button_selected_style, global_config->button_border_width);
    lv_style_set_border_color(&droidboot_list_button_selected_style, lv_color_hex(global_config->button_border_color));


}

