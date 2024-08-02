#include <lvgl.h>
#include <stdlib.h>
#include <droidboot_main.h>
#include <droidboot_error.h>
#include <droidboot_init.h>
#include <droidboot_config_parser.h>
#include <droidboot_logging.h>
#include <droidboot_drivers.h>
#include <dualboot_menu.h>
#include <droidboot_platforms/common/droidboot_platform_common.h>
#include <droidboot_theme.h>

struct boot_entry *droidboot_entry_list;
struct global_config *droidboot_global_config;
int droidboot_num_of_boot_entries;
bool lvgl_init_done;

bool get_lvgl_init_done()
{
    return lvgl_init_done;
}

void droidboot_init()
{
    lvgl_init_done = false;
    droidboot_log(DROIDBOOT_LOG_TRACE, "Welcome to abm init!\n");
    // Run platform init it should be in drivers init, but get_width/height functions can be used by lvgl_init, and on some platforms you cant use them before plarform init.
    droidboot_error ret = droidboot_platform_init();
    if (ret!=DROIDBOOT_EOK)
        return;
    droidboot_log(DROIDBOOT_LOG_TRACE, "droidboot: platform init done\n");
    droidboot_lvgl_init();
    lvgl_init_done = true;
    droidboot_log(DROIDBOOT_LOG_TRACE, "droidboot: lvgl init done\n");
    droidboot_driver_init();
    droidboot_log(DROIDBOOT_LOG_TRACE, "droidboot: driver init done\n");
}

void droidboot_show_dualboot_menu()
{
    /*droidboot_error ret = droidboot_check_metadata();
    if(ret)
        return ret;
   */
    
    // If there is no SD card and fallback is unsupported fail unconditionally, else just error and continue to fallback
    if(droidboot_get_sd_fail()){
        droidboot_log(DROIDBOOT_LOG_ERROR, "droidboot main: failed to get sd card\n");
        if(!droidboot_have_fallback()) while(1);
        return;
    }

    //Parse config
    droidboot_log(DROIDBOOT_LOG_TRACE, "droidboot main: going to parse configs\n");
    droidboot_error ret = parse_boot_entries(&droidboot_entry_list);
    droidboot_log(DROIDBOOT_LOG_TRACE, "droidboot main: parse configs done\n");

    // Parse global config
    droidboot_global_config = malloc(sizeof(struct global_config));
    droidboot_log(DROIDBOOT_LOG_TRACE, "droidboot main: going to parse global configs\n");
    parse_global_config(droidboot_global_config);
    droidboot_log(DROIDBOOT_LOG_TRACE, "droidboot main: parse global config done\n");

    // Get number of entries from storage
    droidboot_num_of_boot_entries = get_dualboot_entry_count();
    droidboot_log(DROIDBOOT_LOG_INFO, "droidboot main: found %d entries\n", droidboot_num_of_boot_entries);

    // Init styles
    droidboot_style_init(droidboot_global_config);

    // Show dualboot menu
    droidboot_draw_dualboot_menu(droidboot_entry_list, droidboot_global_config, droidboot_num_of_boot_entries);
}
