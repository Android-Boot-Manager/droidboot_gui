#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <droidboot_logging.h>
#include <droidboot_error.h>
#include <droidboot_libufdt.h>
#include <sys/types.h>

droidboot_error fdt_add_ramdisk_addr(void *ramdisk_raw, off_t ramdisk_size, void *dtb_raw)
{
     int r,off;
    droidboot_log(DROIDBOOT_LOG_TRACE, "going to add initrd to dtb at addr: %llx, initrd: %llx\n", (uint64_t)dtb_raw, (uint64_t)ramdisk_raw);
    r=fdt_check_header(dtb_raw);
	if(r!=0){
        droidboot_log(DROIDBOOT_LOG_ERROR, "dtb header check failed: %d\n", r);
        return r;
    }

    uint64_t start,end;

    r=fdt_path_offset(dtb_raw,"/chosen");
    if(r<0){
        droidboot_log(DROIDBOOT_LOG_ERROR, "get chosen node failed: %s\n", fdt_strerror(r));
        return r;
    }
    off=r;

    start=(uint64_t)ramdisk_raw;
    end=start+ramdisk_size;

    r=fdt_setprop_u64(
        dtb_raw,off,
        "linux,initrd-start",start
    );
    if(r<0){
        droidboot_log(DROIDBOOT_LOG_ERROR, "update initramfs start failed: %s\n", fdt_strerror(r));
        return r;
    }

    r=fdt_setprop_u64(
        dtb_raw,off,
        "linux,initrd-end",end
    );
    if(r<0){
        droidboot_log(DROIDBOOT_LOG_ERROR, "update initramfs end failed: %s\n", fdt_strerror(r));
        return r;
    }

    droidboot_log(DROIDBOOT_LOG_INFO,
    "update fdt initramfs address done (0x%llx - 0x%llx)\n",
    (unsigned long long)start,
    (unsigned long long)end
	);
    return DROIDBOOT_EOK;
}
