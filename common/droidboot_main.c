#include <lvgl.h>
#include <droidboot_main.h>
#include <droidboot_error.h>
#include <droidboot_init.h>
#include <droidboot_config_parser.h>
#include <droidboot_logging.h>
#include <droidboot_platforms/common/droidboot_platform_common.h>

struct boot_entry *droidboot_entry_list;
struct global_config *droidboot_global_config;
int droidboot_num_of_boot_entries;

void droidboot_init()
{
    droidboot_lvgl_init();
    droidboot_driver_init();
}

void droidboot_show_dualboot_menu()
{
    /*droidboot_error ret = droidboot_check_metadata();
    if(ret)
        return ret;
   */
    
    
    //Parse config
    droidboot_log(DROIDBOOT_LOG_TRACE, "droidboot main: going to parse configs");
    droidboot_error ret = parse_boot_entries(&droidboot_entry_list);
    droidboot_log(DROIDBOOT_LOG_TRACE, "droidboot main: parse configs done");
    // Parse global config
    droidboot_global_config = malloc(sizeof(struct global_config));
    parse_global_config(droidboot_global_config);

    // Get number of entries from storage
    droidboot_num_of_boot_entries = get_dualboot_entry_count();
    droidboot_log(DROIDBOOT_LOG_INFO, "droidboot main: found %d entries\n", droidboot_num_of_boot_entries);
    droidboot_draw_dualboot_menu(droidboot_entry_list, droidboot_global_config, droidboot_num_of_boot_entries); 
}
