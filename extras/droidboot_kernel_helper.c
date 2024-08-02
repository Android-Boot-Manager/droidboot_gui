#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <droidboot_logging.h>
#include <droidboot_error.h>
#include <mini_gzip.h>

#define MAGIC_KERNEL_ARM64 0x644d5241

droidboot_error droidboot_kernel_prepear(void* loaded_kernel, uint64_t kernel_raw_size, void* target_kernel, uint64_t max_kernel_size, uint64_t* kernel_actual_size)
{
    int ret;
    if(kernel_raw_size<=64) return DROIDBOOT_EINVAL;
    uint32_t magic=*(uint32_t*)(loaded_kernel+0x38);
    if(magic==MAGIC_KERNEL_ARM64){
        droidboot_log(DROIDBOOT_LOG_INFO, "Kernel is already decompressed, continuing boot\n");
        memcpy(target_kernel, loaded_kernel, kernel_raw_size);
        *kernel_actual_size=kernel_raw_size;
        return DROIDBOOT_EOK;
    }
    droidboot_log(DROIDBOOT_LOG_WARNING, "Kernel signature check failed, trying to decompress\n");
    unsigned char *header_ptr = (unsigned char *)loaded_kernel;

    // Check if the data starts with the gzip signature (0x1f 0x8b)
    if (header_ptr[0] != 0x1f || header_ptr[1] != 0x8b) {
        droidboot_log(DROIDBOOT_LOG_ERROR, "Provided file is not arm64 linux kernel nor gz archive, signature is: %x %x\n", header_ptr[0], header_ptr[1]);
        return DROIDBOOT_ENODATA;
    }

    struct mini_gzip gz;
    ret = mini_gz_start(&gz, loaded_kernel, kernel_raw_size);
    if(ret!=0)
        return ret;
    *kernel_actual_size = mini_gz_unpack(&gz, target_kernel, max_kernel_size);
    if(*kernel_actual_size<=64) return DROIDBOOT_EINVAL;
    magic=*(uint32_t*)(target_kernel+0x38);
    if(magic==MAGIC_KERNEL_ARM64){
        droidboot_log(DROIDBOOT_LOG_INFO, "Kernel signature is correct\n");
        return DROIDBOOT_EOK;
    }
    return DROIDBOOT_ENODATA;
}
