#pragma once
droidboot_error droidboot_kernel_prepear(void* loaded_kernel, off_t kernel_raw_size, void* target_kernel, off_t max_kernel_size, off_t* kernel_actual_size);
