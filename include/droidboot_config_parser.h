#pragma once

struct global_config {
	char *default_entry_title;
	struct boot_entry *default_entry;
	int timeout;
	uint32_t win_bg_color;
	uint8_t win_radius;
	uint8_t win_border_size;
	uint32_t win_border_color;
	uint32_t list_bg_color;
	uint8_t list_radius;
	uint8_t list_border_size;
	uint32_t list_border_color;
	uint32_t global_font_size;
	char* global_font_name;
	uint32_t button_unselected_color;
	uint32_t button_unselected_text_color;
	uint32_t button_selected_color;
	uint32_t button_selected_text_color;
	uint8_t button_unselected_radius;
	uint8_t button_selected_radius;
	bool button_grow_default;
	uint8_t button_border_unselected_size;
	uint32_t button_border_unselected_color;
	uint8_t button_border_selected_size;
	uint32_t button_border_selected_color;
};


struct boot_entry {
	char *title;
	char *kernel;
	char *initrd;
    char *dtb;
    char *dtbo;
	char *options;
	char *logo;
	int error;
};

int get_dualboot_entry_count(void);

int parse_boot_entries(struct boot_entry **_entry_list);

int parse_global_config(struct global_config *global_config);
