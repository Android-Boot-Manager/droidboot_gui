#pragma once
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
