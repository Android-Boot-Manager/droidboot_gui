// This file contains a high level, universal (not platform dependent) code for displaying ABM gui menu
// At this stage we except that display is inited and lower level code is already executed.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <lvgl.h>

#include <droidboot_drivers.h>
#include <droidboot_screens.h>
#include <droidboot_config_parser.h>
#include <droidboot_dualboot_backend.h>
#include <droidboot_stdfunc.h>
#include <droidboot_helpers.h>

// droidboot_platforms_common.h is part of droidboot_platform and contains all functions used by droidboot gui
#include <droidboot_platforms/common/droidboot_platform_common.h>

int droidboot_exit;
int timeout;
lv_obj_t *timeout_label;
bool no_autoboot;
lv_timer_t * timer;

extern struct boot_entry *droidboot_entry_list;
extern struct global_config *droidboot_global_config;
extern int droidboot_num_of_boot_entries;

struct boot_entry *selected_entry;

static void event_handler(lv_event_t * e)
{
    droidboot_log(DROIDBOOT_LOG_INFO, "dualboot menu: got event\n");
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    int index = lv_obj_get_child_id(obj);
    if(code == LV_EVENT_CLICKED) {
        //lvgl_show_boot_logo();
        // Lets firstly write index to metadata
        ext4_file f;
        int ret = ext4_fopen(&f, "/boot/db/last_index", "wb");
       
        char s[128]=""; 
        sprintf(s,"%ld", index);
        ret=ext4_fwrite(&f, s, 128, 0);
        ret=ext4_fclose(&f);
        
        if(!strcmp((droidboot_entry_list + index)->kernel, "null"))
        {
            droidboot_log(DROIDBOOT_LOG_INFO, "droidboot_menu: Linux is null");   
            droidboot_exit=0;
            return;
        }

        selected_entry=droidboot_entry_list + index;
        no_autoboot=true;
           
        droidboot_exit=1;
        droidboot_log(DROIDBOOT_LOG_INFO, "droidboot_menu: exit is: %d\n", droidboot_exit);
    }
}

static void disable_autoboot_func(lv_event_t * e)
{
    droidboot_log(DROIDBOOT_LOG_INFO, "droidboot_menu: got event\n");
    no_autoboot = true;
}

void timeout_handler(lv_timer_t * timer)
{
    /*Use the user_data*/
    if(timeout>0 && !no_autoboot){
        timeout-=1;
        lv_label_set_text_fmt(timeout_label, "\n\nBooting in %d seconds.", timeout);
    } else if (timeout>0 && no_autoboot) {
        lv_obj_add_flag(timeout_label, LV_OBJ_FLAG_HIDDEN);
    } if (droidboot_exit==100 && timeout<=0 && !no_autoboot) {
        //lvgl_show_boot_logo();
        // Lets firstly read index from metadata
        int ret;
        ext4_file fp;
        unsigned char *buf;

        ret=ext4_fopen (&fp, "/boot/db/last_index", "r");
        if(ret!=0)
            return;
        
        ret=ext4_fseek(&fp, 0, SEEK_SET);  /* same as rewind(f); */
        if(ret!=0)
            return;
    
        buf = malloc(128);
    
        ret=ext4_fread(&fp, buf, 128, 0);
        if(ret!=0)
            return;

        ret=ext4_fclose(&fp);
	
        droidboot_log(DROIDBOOT_LOG_INFO, "droidboot_menu: last entry is: %s\n", buf);
        int index = droidboot_atoi(buf);
        
        if(!strcmp((droidboot_entry_list + index)->kernel, "null"))
        {
            droidboot_log(DROIDBOOT_LOG_INFO, "droidboot_menu: linux is null");   
            droidboot_exit=0;
            return;
        }

    selected_entry=droidboot_entry_list + index;
    droidboot_exit=1;
    
  }
}

void droidboot_add_dualboot_menu_buttons(lv_obj_t * list1){
    lv_obj_t * list_btn;
       
    //lv_img_dsc_t img_dscs[droidboot_num_of_boot_entries];
    int i;
    
    
    for (int i = 0; i < droidboot_num_of_boot_entries; i++) {
        char *title = malloc(strlen((droidboot_entry_list + i)->title) + 8);
        
        strcpy(title, "\n");
        strcat(title, (droidboot_entry_list + i)->title);
        strcat(title, "\n");

        if((droidboot_entry_list + i)->logo!="NULL"){
            char logo_path[strlen((droidboot_entry_list + i)->logo)+strlen("/boot/"+3)];
            strcpy(logo_path, "/boot/");
            strcat(logo_path, (droidboot_entry_list + i)->logo);
            list_btn = lv_list_add_btn(list1, droidboot_load_lvgl_image_from_ext4(logo_path), title);
        } else {
            list_btn = lv_list_add_btn(list1, NULL, title);
       }

        lv_obj_add_event_cb(list_btn, event_handler, LV_EVENT_CLICKED, NULL);
        lv_obj_add_event_cb(list_btn, disable_autoboot_func, LV_EVENT_DEFOCUSED, NULL);
    }
    // Set cursor to last selected entry
    int ret;
	ext4_file fp;
	unsigned char *buf;

    ret=ext4_fopen (&fp, "/boot/db/last_index", "r");
    if(ret!=0)
        return;
        
    ret=ext4_fseek(&fp, 0, SEEK_SET);  /* same as rewind(f); */
    if(ret!=0)
        return;
    
    buf = malloc(128);

	ret=ext4_fread(&fp, buf, 128, 0);
	if(ret!=0)
        return;

	ret=ext4_fclose(&fp);
	
	droidboot_log(DROIDBOOT_LOG_INFO, "droidboot_menu: last entry is: %s\n", buf);
	int index = droidboot_atoi(buf);
	list_btn=lv_obj_get_child(list1, index);
	lv_group_focus_obj(list_btn);
	timeout_label = lv_label_create(list_btn);
	lv_label_set_text_fmt(timeout_label, "\n\nBooting in %d seconds.", droidboot_global_config->timeout);
	timeout = droidboot_global_config->timeout;
	droidboot_log(DROIDBOOT_LOG_INFO, "droidboot_menu: timeout is: %d\n", timeout);
	timer = lv_timer_create(timeout_handler, 100,  0);
	lv_obj_set_y(timeout_label, lv_pct(3));
	lv_obj_set_align(timeout_label, LV_ALIGN_RIGHT_MID);
}

void droidboot_draw_dualboot_menu(struct boot_entry *droidboot_entry_list1, struct global_config *droidboot_global_config1, int droidboot_num_of_boot_entries1)
{
    droidboot_log(DROIDBOOT_LOG_INFO, "Enter draw dualboot menu\n");
    droidboot_entry_list = droidboot_entry_list1;
    droidboot_global_config = droidboot_global_config1;
    droidboot_num_of_boot_entries = droidboot_num_of_boot_entries1;
    droidboot_log(DROIDBOOT_LOG_INFO, "Dualboot menu have: %d entries\n", droidboot_num_of_boot_entries);
   // lv_scr_load(dualboot_menu);
    //lv_obj_del(boot_logo);
    //lv_obj_del(setup_main);

    lv_obj_t * win = lv_win_create(lv_scr_act(), lv_pct(6));
    lv_obj_set_pos(win, 0, 0);
    lv_obj_set_size(win, lv_pct(100), lv_pct(100));
    lv_obj_t * win_title = lv_win_add_title(win, "Select OS"); 
    lv_obj_set_pos(win_title, 0, 0);

    lv_obj_t * list1 = lv_list_create(win); 
    lv_obj_set_size(list1, lv_pct(100), lv_pct(100));
    lv_obj_set_pos(list1, 0, 00);
    lv_obj_align(list1, LV_ALIGN_BOTTOM_MID, 0, 0);
    droidboot_add_dualboot_menu_buttons(list1);

    no_autoboot = false;
    droidboot_exit=100;
    droidboot_log(DROIDBOOT_LOG_INFO, "Dualboot menu draw done, going into loop\n");
    while(droidboot_exit==100){
        droidboot_platform_tasks();
    }
    //lv_timer_del(timer);
    droidboot_log(DROIDBOOT_LOG_INFO, "Dualboot menu: exit\n");
    if(droidboot_exit==1){
        droidboot_log(DROIDBOOT_LOG_INFO, "Dualboot menu: exit, boot second OS\n");
        droidboot_boot_linux_from_ext4(selected_entry);
    }
    droidboot_log(DROIDBOOT_LOG_INFO, "Dualboot menu: exit to bl\n");
}
