#pragma once
#include <droidboot_error.h>
#include <stdint.h>

droidboot_error droidboot_kernel_prepear(void* loaded_kernel, uint64_t kernel_raw_size, void* target_kernel, uint64_t max_kernel_size, uint64_t* kernel_actual_size);
