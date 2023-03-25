#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <droidboot_logging.h>
#include <droidboot_error.h>
#include <droidboot_libufdt.h>
#include <sys/types.h>
#include <droidboot_libfdt.h>

typedef void* fdt;
typedef struct _KERNEL_FDT_PROTOCOL {
        //
        // Device Tree pointer
        //
        fdt Fdt;

        //
        // Device Tree size
        //
        uint64_t FdtSize;
} KERNEL_FDT_PROTOCOL;
typedef uint32_t fdt32_t;


droidboot_error fdt_add_ramdisk_addr(void *ramdisk_raw, off_t ramdisk_size, void *dtb_raw);
int fdt_add_merge_memory(void *dtb_raw,uint64_t addr, uint64_t size);
int32_t fdt_get_address_cells(fdt*fdt);
int32_t fdt_get_size_cells(fdt*fdt);
bool fdt_get_reg(fdt*fdt,int off,int index,uint64_t*base,uint64_t*size);
bool fdt_get_memory(fdt*fdt,int index,uint64_t*base,uint64_t*size);
int fdt_add_memory(void *dtb_raw, uint64_t addr, uint64_t size);

