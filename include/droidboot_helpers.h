#pragma once
#include <lvgl.h>

// This function must not be here, as lvgl already have its own fs support, but it is too buggy
const void* droidboot_load_lvgl_image_from_ext4(char* path);
