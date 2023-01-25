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

    //ret=ext4_mount("abm_settings", "/boot/", false);
    //if(ret!=DROIDBOOT_EOK)
    //{
    //    droidboot_log(DROIDBOOT_LOG_ERROR, "config_parser: failed to mount abm_settings, mount ret: %d\n", ret);
    //    return ret;
    //}

    ret = ext4_fopen (&fp, GLOBAL_CONFIG_FILE, "r");
    if(ret!=0){
        global_config->default_entry_title = NULL;
		global_config->timeout = 20;
        droidboot_log(DROIDBOOT_LOG_WARNING, "config_parser: no global config found\n");
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
		global_config->timeout = 20;

		return 0;
	}
    droidboot_log(DROIDBOOT_LOG_INFO, "config_parser: default config parse done\n");
	//char timeout;
	//ret = config_parse_option(&timeout, "timeout", (const char *)buf);

	global_config->timeout = 20;
    droidboot_log(DROIDBOOT_LOG_INFO, "config_parser: timeout is: %d seconds\n", 20);
    //ext4_umount("/boot/");
	return 0;
} 
