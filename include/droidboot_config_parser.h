#pragma once
struct global_config {
	char *default_entry_title;
	struct boot_entry *default_entry;
	int timeout;
};


struct boot_entry {
	char *title;
	char *kernel;
	char *initrd;
    char *dtb;
	char *options;
	char *logo;
	int error;
};

int get_dualboot_entry_count(void);

int parse_boot_entries(struct boot_entry **_entry_list);

int parse_global_config(struct global_config *global_config);
