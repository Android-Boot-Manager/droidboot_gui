// SPDX-License-Identifier: GPL-2.0+
// © 2019 Mis012
// © 2020-2022 luka177
// © 2022 Hallo Welt Systeme UG
// © 2022-2023 luka177

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <droidboot_config.h>
#include <droidboot_logging.h>
#include <droidboot_error.h>
#include <droidboot_stdfunc.h>
#include <droidboot_fs_util.h>
#include <droidboot_platform_common.h>

#include <ext4.h>

#define ENTRIES_DIR "/boot/db/entries/"
#define GLOBAL_CONFIG_FILE "/boot/db/db.conf"

struct boot_entry {
	char *title;
	char *kernel;
	char *initrd;
    char *dtb;
	char *options;
	char *logo;
	int error;
};

struct global_config {
    char *default_entry_title;
    struct boot_entry *default_entry;
    int timeout;
    uint32_t bg_color;
    uint32_t button_color;
    uint32_t button_text_color;
    uint32_t button_selected_color;
    uint32_t button_selected_text_color;
    uint8_t border_width;
    uint32_t border_color;
    uint32_t border_selected_color;
    uint8_t button_border_width;
    uint32_t button_border_color;
    uint32_t button_border_selected_color;
    uint8_t radius;
    bool button_grow_default;
};

int config_parse_option(char **_dest, const char *option, const char *buffer) {
	char *temp = strstr(buffer, option);
	if(!temp)
		return -1;

	temp += strlen(option);
	while (*temp == ' ')
		temp++;
	char *newline = strchr(temp, '\n');
	if(newline)
		*newline = '\0';
	char *dest = malloc(strlen(temp) + 1);
	if(!dest)
		return DROIDBOOT_ENOMEM;
	strcpy(dest, temp);
	*_dest = dest;

	//restore the buffer
	*newline = '\n';

	return 0;
}

int parse_boot_entry_file(struct boot_entry *entry, char *file) {
	int ret;
	ext4_file fp;
	size_t rb;
	unsigned char *buf;

	char *path = malloc(strlen(file) + strlen(ENTRIES_DIR) + strlen("/") + 1);
	strcpy(path, ENTRIES_DIR);
	strcat(path, file);
    droidboot_log(DROIDBOOT_LOG_INFO, "config_parser: Going to parse file: %s\n", path);
    ret=ext4_fopen (&fp, path, "r");
    ext4_fseek(&fp, 0, SEEK_END);
    off_t entry_file_size = ext4_ftell(&fp);
    ext4_fseek(&fp, 0, SEEK_SET);  /* same as rewind(f); */
    
	buf = malloc(entry_file_size + 1);
	if(!buf)
		return DROIDBOOT_ENOMEM;

    droidboot_log(DROIDBOOT_LOG_INFO, "config_parser: Alloc entr buf ok\n");
    
	ext4_fread(&fp, buf, entry_file_size, &rb);
	
	ret = ext4_fclose(&fp);
    
    if(ret) {
		free(buf);
		return ret;
	}

	buf[entry_file_size] = '\0';

	ret = config_parse_option(&entry->title, "title", (const char *)buf);
	if(ret < 0) {
		droidboot_log(DROIDBOOT_LOG_WARNING, "config_parser: SYNTAX ERROR: entry \"%s\" - no option 'title'\n", path);
		free(buf);
		return ret;
	}

	ret = config_parse_option(&entry->kernel, "linux", (const char *)buf);
	if(ret < 0) {
		droidboot_log(DROIDBOOT_LOG_WARNING, "config_parser: SYNTAX ERROR: entry \"%s\" - no option 'linux'\n", path);
		free(buf);
		return ret;
	}

	ret = config_parse_option(&entry->initrd, "initrd", (const char *)buf);
	if(ret < 0) {
		droidboot_log(DROIDBOOT_LOG_WARNING, "config_parser: SYNTAX ERROR: entry \"%s\" - no option 'initrd'\n", path);
		free(buf);
		return ret;
	}

	ret = config_parse_option(&entry->dtb, "dtb", (const char *)buf);
	if(ret < 0) {
		droidboot_log(DROIDBOOT_LOG_WARNING, "config_parser: SYNTAX ERROR: entry \"%s\" - no option 'dtb'\n", path);
		free(buf);
		return ret;
	}

	ret = config_parse_option(&entry->options, "options", (const char *)buf);
	if(ret < 0) {
		droidboot_log(DROIDBOOT_LOG_WARNING, "config_parser: SYNTAX ERROR: entry \"%s\" - no option 'options'\n", path);
		free(buf);
		return ret;
	}
	
	ret = config_parse_option(&entry->logo, "logo", (const char *)buf);
	if(ret < 0) {
	    droidboot_log(DROIDBOOT_LOG_INFO, "config_parser: SYNTAX ERROR: entry \"%s\" - no option 'logo'\n", path);
		ret=0;
		entry->logo="NULL";
	}

	free(buf);

	entry->error = false;

	return 0;
}
int entry_count;


int get_dualboot_entry_count(void) {
	return entry_count;
}

int parse_boot_entries(struct boot_entry **_entry_list) {
	int ret;

    struct boot_entry *entry_list=malloc(sizeof(struct boot_entry)*dir_count_entries(ENTRIES_DIR));    

    //ret=ext4_mount("abm_settings", "/boot/", false);
   // if(ret!=DROIDBOOT_EOK)
   // {
    //    droidboot_log(DROIDBOOT_LOG_ERROR, "config_parser: failed to mount abm_settings, mount ret: %d\n", ret);
    //    return ret;
   // }

	ret = entry_count = dir_count_entries(ENTRIES_DIR);
	droidboot_log(DROIDBOOT_LOG_TRACE, "config_parser: found %d entries\n");
	if (ret < 0) {
		entry_count = 0;
	}

	const ext4_direntry *de;
    ext4_dir d;

    ret=ext4_dir_open (&d, ENTRIES_DIR);
    if(ret!=0){
        droidboot_log(DROIDBOOT_LOG_ERROR, "config_parser: entries dir open failed: %d\n", ret);
        droidboot_delay(1000);
    }
    droidboot_log(DROIDBOOT_LOG_TRACE, "config_parser: entries dir open ok\n");
    de = ext4_dir_entry_next(&d);
	int i = 0;
	while(de) {
	     droidboot_log(DROIDBOOT_LOG_TRACE, "config_parser: Found direntry in entries dir\n");
         if(de->inode_type==1){
            struct boot_entry *entry = (entry_list+i);
            ret = parse_boot_entry_file(entry, de->name);
            droidboot_log(DROIDBOOT_LOG_TRACE, "config_parser: found file in entries dir\n");
            if(ret < 0) {
                entry->error = true;
                entry->title = "SYNTAX ERROR";
            }

            i++;
        }
        de = ext4_dir_entry_next(&d);
	}
	
	ext4_dir_close(&d);
    
   *_entry_list = entry_list;
    droidboot_log(DROIDBOOT_LOG_INFO, "config_parser: First entry is: %s\n", entry_list->title);
    //ext4_umount("/boot/");
	return 0;
}

int parse_global_config(struct global_config *global_config) {
	int ret;
	ext4_file fp;
	unsigned char *buf;

    ret = ext4_fopen (&fp, GLOBAL_CONFIG_FILE, "r");
    if(ret!=0){
        global_config->default_entry_title = NULL;
		global_config->timeout = 20;
        droidboot_log(DROIDBOOT_LOG_WARNING, "config_parser: no global config found\n");
        // Background color
        global_config->bg_color = 0x000000;

        // Button color
        global_config->button_color = 0x000000;

        // Button text color
        global_config->button_text_color = 0xffffff;

        // Button selected color
        global_config->button_selected_color = 0xff9800;

        // Button selected text color
        global_config->button_selected_text_color = 0x000000;

        // Border width
        global_config->border_width = 0;

        // Border color
        global_config->border_color = 0xffffff;

        // Border selected color
        global_config->border_selected_color = 0xffffff;

        // Button border width
        global_config->button_border_width = 1;

        // Button border color
        global_config->button_border_color = 0xffffff;

        // Button border selected color
        global_config->button_border_selected_color = 0xffffff;

        // Radius
        global_config->radius = 0;

        // Button grow by default
        global_config->button_grow_default = true;

		return 0;

    }

    ext4_fseek(&fp, 0, SEEK_END);
    long fsize = ext4_ftell(&fp);
    ext4_fseek(&fp, 0, SEEK_SET);  /* same as rewind(f); */
    
    buf = malloc(fsize + 1);
    
	ext4_fread(&fp, buf, fsize, NULL);

	ext4_fclose(&fp);

	ret = config_parse_option(&global_config->default_entry_title, "default", (const char *)buf);
	if(ret < 0) {
		global_config->default_entry_title = NULL;
	}
    droidboot_log(DROIDBOOT_LOG_INFO, "config_parser: default config parse done\n");

    char *timeout = NULL;
    ret = config_parse_option(&timeout, "timeout", (const char *)buf);
    if(ret<0) global_config->timeout =20;
    else global_config->timeout = droidboot_atoi(timeout);
    droidboot_log(DROIDBOOT_LOG_INFO, "config_parser: timeout is: %d seconds\n", global_config->timeout);

    // Theme parsing

    // Background color
    char *bg_color = NULL;
    ret = config_parse_option(&bg_color, "bg_color", (const char *)buf);
    if(ret<0) global_config->bg_color = 0x000000;
    else global_config->bg_color = strtoul(bg_color, NULL, 16);

    // Button color
    char *button_color = NULL;
    ret = config_parse_option(&button_color, "button_color", (const char *)buf);
    if(ret<0) global_config->button_color = 0x000000;
    else global_config->button_color = strtoul(button_color, NULL, 16);

    // Button text color
    char *button_text_color = NULL;
    ret = config_parse_option(&button_text_color, "button_text_color", (const char *)buf);
    if(ret<0) global_config->button_text_color = 0xffffff;
    else global_config->button_text_color = strtoul(button_text_color, NULL, 16);

    // Button selected color
    char *button_selected_color = NULL;
    ret = config_parse_option(&button_selected_color, "button_selected_color", (const char *)buf);
    if(ret<0) global_config->button_selected_color = 0xff9800;
    else global_config->button_selected_color = strtoul(button_selected_color, NULL, 16);

    // Button selected text color
    char *button_selected_text_color = NULL;
    ret = config_parse_option(&button_selected_text_color, "button_selected_text_color", (const char *)buf);
    if(ret<0) global_config->button_selected_text_color = 0x000000;
    else global_config->button_selected_text_color = strtoul(button_selected_text_color, NULL, 16);

    // Border width
    char *border_width = NULL;
    ret = config_parse_option(&border_width, "border_width", (const char *)buf);
    if(ret<0) global_config->border_width = 0;
    else global_config->border_width = droidboot_atoi(border_width);

    // Border color
    char *border_color = NULL;
    ret = config_parse_option(&border_color, "border_color", (const char *)buf);
    if(ret<0) global_config->border_color = 0xffffff;
    else global_config->border_color = strtoul(border_color, NULL, 16);

    // Border selected color
    char *border_selected_color = NULL;
    ret = config_parse_option(&border_selected_color, "border_selected_color", (const char *)buf);
    if(ret<0) global_config->border_selected_color = 0xffffff;
    else global_config->border_selected_color = strtoul(border_selected_color, NULL, 16);

    // Button border width
    char *button_border_width = NULL;
    ret = config_parse_option(&button_border_width, "button_border_width", (const char *)buf);
    if(ret<0) global_config->button_border_width = 1;
    else global_config->button_border_width = droidboot_atoi(button_border_width);

    // Button border color
    char *button_border_color = NULL;
    ret = config_parse_option(&button_border_color, "button_border_color", (const char *)buf);
    if(ret<0) global_config->button_border_color = 0xffffff;
    else global_config->button_border_color = strtoul(button_border_color, NULL, 16);

    // Button border selected color
    char *button_border_selected_color = NULL;
    ret = config_parse_option(&button_border_selected_color, "button_border_selected_color", (const char *)buf);
    if(ret<0) global_config->button_border_selected_color = 0xffffff;
    else global_config->button_border_selected_color = strtoul(button_border_selected_color, NULL, 16);

    // Radius
    char *radius = NULL;
    ret = config_parse_option(&radius, "radius", (const char *)buf);
    if(ret<0) global_config->radius = 0;
    else global_config->radius = droidboot_atoi(radius);

    // Button grow by default
    char *button_grow_default = NULL;
    ret = config_parse_option(&button_grow_default, "button_grow_default", (const char *)buf);
    if(ret<0) global_config->button_grow_default = true;
    else
        if(button_grow_default=="true")
            global_config->button_grow_default = true;
        else
            global_config->button_grow_default = false;

	return 0;
} 
