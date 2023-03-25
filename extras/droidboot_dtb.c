#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <droidboot_logging.h>
#include <droidboot_error.h>
#include <droidboot_libufdt.h>
#include <sys/types.h>
#include <droidboot_libfdt.h>
#include <droidboot_dtb.h>

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

static int fdt_append_reg(void*fdt,int off,bool use32,uint64_t val){
	return use32?
		fdt_appendprop_u32(fdt,off,"reg",(uint32_t)val):
		fdt_appendprop_u64(fdt,off,"reg",(uint64_t)val);
}

int fdt_add_memory(void *dtb_raw,uint64_t addr,uint64_t size){
	int ret=0;
	char buf[64];
	static int off=-1;
	static void*dtb=NULL;
	static bool use32=false;
	if(dtb!=dtb_raw){
		off=fdt_path_offset(dtb_raw,"/memory");
        droidboot_log(DROIDBOOT_LOG_ERROR, "memory off is: %d\n", off);
		if(off<0){
			off=fdt_add_subnode(dtb_raw,0,"/memory");
			if(off<0){
                droidboot_log(DROIDBOOT_LOG_ERROR,
				"get memory node failed: %s",
				fdt_strerror(off)
			);
                return -1;
            }
			fdt_setprop_string(dtb_raw,off,"device_type","memory");
		}
		fdt_delprop(dtb_raw,off,"reg");
		dtb=dtb_raw;
	}
	droidboot_log(DROIDBOOT_LOG_INFO,
		"memory: 0x%016llx - 0x%016llx (%llu bytes)",
		(unsigned long long)addr,
		(unsigned long long)addr+size,
		(unsigned long long)size
	);

	ret=fdt_append_reg(dtb_raw,off,use32,addr);
	if(ret<0){
        droidboot_log(DROIDBOOT_LOG_ERROR,
		"add memory address failed: %s",
		fdt_strerror(ret)
	);
        return ret;
    }
	ret=fdt_append_reg(dtb_raw,off,use32,size);
	if(ret<0)droidboot_log(DROIDBOOT_LOG_ERROR,
		"add memory size failed: %s",
		fdt_strerror(ret)
	);
	return ret;
}

int fdt_add_merge_memory(void *dtb_raw,uint64_t addr,uint64_t size){
	STATIC uint64_t last_end=0,start=0;
	uint64_t end=addr+size;
	if(size!=0){
		if(last_end!=addr){
			if(start>0&&end>start)fdt_add_memory(dtb_raw,start,end-start);
			start=addr;
		}
		last_end=end;
	}else if(start>0&&last_end>start)fdt_add_memory(dtb_raw,start,last_end-start);
	return 0;
}

int32_t fdt_get_address_cells(fdt*fdt){
	const int32_t*prop;
	int32_t length=0,ret=1;
	prop=fdt_getprop(fdt,0,"#address-cells",&length);
	if(length==4)ret=fdt32_to_cpu(*prop);
	return ret;
}

int32_t fdt_get_size_cells(fdt*fdt){
	const int32_t*prop;
	int32_t length=0,ret=1;
	prop=fdt_getprop(fdt,0,"#size-cells",&length);
	if(length==4)ret=fdt32_to_cpu(*prop);
	return ret;
}

bool fdt_get_reg(fdt*fdt,int off,int index,uint64_t*base,uint64_t*size){
	const int32_t*prop;
	int32_t ac,sc,item,len=0;
	if(!fdt||off<0||index<0||!base||!size)return false;

	// load address cells and size cells
	ac=fdt_get_address_cells(fdt);
	sc=fdt_get_size_cells(fdt);
	*base=0,*size=0;

	prop=fdt_getprop(fdt,off,"reg",&len);
	item=(ac+sc)*sizeof(int32_t);
	if(len<item)return false;

	// in range
	if(index>=len/item)return false;
	prop+=(ac+sc)*index;

	// calc memory base
	*base=fdt32_to_cpu(prop[0]);
	if(ac>1)*base=((*base)<<32)|fdt32_to_cpu(prop[1]);
	prop+=ac;

	// calc memory size
	*size=fdt32_to_cpu(prop[0]);
	if(sc>1)*size=((*size)<<32)|fdt32_to_cpu(prop[1]);
	prop+=sc;

	return true;
}

bool fdt_get_memory(fdt*fdt,int index,uint64_t*base,uint64_t*size){
	return fdt?fdt_get_reg(fdt,fdt_path_offset(fdt,"/memory"),index,base,size):false;
}
