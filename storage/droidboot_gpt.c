#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <droidboot_error.h>
#include <droidboot_logging.h>
#include <sys/types.h>
#include <droidboot_gpt.h>
#include <droidboot_platform_common.h>
#include <libc-hack.h>

#ifndef __PACKED
#define __PACKED __attribute__((__packed__))
#endif

#define DROIDBOOT_ROUNDUP(a, b) (((a) + ((b)-1)) & ~((b)-1))
#define DROIDBOOT_ROUNDDOWN(a, b) ((a) & ~((b)-1))

#define DROIDBOOT_ALIGN(a, b) DROIDBOOT_ROUNDUP(a, b)
#define DROIDBOOT_IS_ALIGNED(a, b) (!((a) & ((b)-1)))

/* allocate a buffer on the stack aligned and padded to the cpu's cache line size */
#define DROIDBOOT_STACKBUF_DMA_ALIGN(var, size) \
	uint8_t __##var[(size) + CACHE_LINE]; uint8_t *var = (uint8_t *)(DROIDBOOT_ROUNDUP((addr_t)__##var, CACHE_LINE))

struct chs {
	uint8_t c;
	uint8_t h;
	uint8_t s;
};

struct mbr_part {
	uint8_t status;
	struct chs start;
	uint8_t type;
	struct chs end;
	uint32_t lba_start;
	uint32_t lba_length;
} __PACKED;

struct gpt_header {
	uint64_t first_usable_lba;
	uint32_t partition_entry_size;
	uint32_t header_size;
	uint32_t max_partition_count;
};

uint64_t abm_settings_offset;
uint64_t abm_settings_blkcnt;

#ifdef DROIDBOOT_NO_SD_SUPPORT
uint64_t userdata_offset;
uint64_t userdata_blkcnt;
#endif

#ifdef DROIDBOOT_NO_SD_ENCRYPTED_SUPPORT
uint64_t metadata_offset;
uint64_t metadata_blkcnt;
#endif

bool parse_done = false;

static droidboot_error validate_mbr_partition(const struct mbr_part *part)
{
	/* check for invalid types */
	if (part->type == 0)
		return -1;
	/* check for invalid status */
	if (part->status != 0x80 && part->status != 0x00)
		return -1;

	/* make sure the range fits within the device */
	if (part->lba_start >= droidboot_sd_blkcnt())
		return -1;
	if ((part->lba_start + part->lba_length) > droidboot_sd_blkcnt())
		return -1;

	/* that's about all we can do, MBR has no other good way to see if it's valid */

	return 0;
}

/*
 * Parse the gpt header and get the required header fields
 * Return 0 on valid signature
 */
static unsigned int
partition_parse_gpt_header(const char *buffer, struct gpt_header* header)
{
	/* Check GPT Signature */
	if (((uint32_t *) buffer)[0] != GPT_SIGNATURE_2 ||
	    ((uint32_t *) buffer)[1] != GPT_SIGNATURE_1)
		return 1;

	header->header_size = GET_LWORD_FROM_BYTE(&buffer[HEADER_SIZE_OFFSET]);
	header->first_usable_lba =
	    GET_LLWORD_FROM_BYTE(&buffer[FIRST_USABLE_LBA_OFFSET]);
	header->max_partition_count =
	    GET_LWORD_FROM_BYTE(&buffer[PARTITION_COUNT_OFFSET]);
	header->partition_entry_size =
	    GET_LWORD_FROM_BYTE(&buffer[PENTRY_SIZE_OFFSET]);

	return 0;
}

droidboot_error droidboot_parse_gpt_on_sd()
{
    droidboot_log(DROIDBOOT_LOG_INFO, "Enter droidboot_parse_gpt_on_sd\n");
	abm_settings_blkcnt=0;
#ifdef DROIDBOOT_NO_SD_SUPPORT
	userdata_blkcnt=0;
#endif
    if(parse_done){
       return DROIDBOOT_EOK;
    }

    if(!droidboot_sd_exists()){
        droidboot_log(DROIDBOOT_LOG_ERROR, "droidboot_parse_gpt_on_sd: sd card is not present\n");
        return DROIDBOOT_ENOENT;
    }

	if(droidboot_sd_blklen()==0){
        droidboot_log(DROIDBOOT_LOG_ERROR, "droidboot_parse_gpt_on_sd: sd card blklen is 0\n");
        return DROIDBOOT_ENOENT;
    }

    if(droidboot_sd_blkcnt()==0){
        droidboot_log(DROIDBOOT_LOG_ERROR, "droidboot_parse_gpt_on_sd: sd card blkcnt is 0\n");
        return DROIDBOOT_ENOENT;
    }

    droidboot_log(DROIDBOOT_LOG_INFO, "SD card blklen: %d, blkcnt: %d\n", droidboot_sd_blklen(), droidboot_sd_blkcnt());


    // get a dma aligned and padded block to read info
	char *buf = malloc(droidboot_sd_blklen()*2);
	droidboot_log(DROIDBOOT_LOG_TRACE, "SD card blklen: %d, blkcnt: %d\n", droidboot_sd_blklen(), droidboot_sd_blkcnt());
	/* sniff for MBR partition types */
		unsigned int i, j, n;
		int gpt_partitions_exist = 0;

		droidboot_sd_read_block(buf, 0, 1);

		/* look for the aa55 tag */
		//if (buf[510] != 0x55 || buf[511] != 0xaa)
		//	break;

		/* see if a partition table makes sense here */
		struct mbr_part part[4];
		memcpy(part, buf + 446, sizeof(part));
		droidboot_log(DROIDBOOT_LOG_TRACE, "mbr partition table dump:\n");
		for (i=0; i < 4; i++) {
			droidboot_log(DROIDBOOT_LOG_TRACE, "\t%i: status 0x%hhx, type 0x%hhx, start 0x%x, len 0x%x\n", i, part[i].status, part[i].type, part[i].lba_start, part[i].lba_length);
		}

		/* validate each of the partition entries */
		for (i=0; i < 4; i++) {
			if (validate_mbr_partition(&part[i]) >= 0) {
				/* Type 0xEE indicates end of MBR and GPT partitions exist */
				if(part[i].type==0xee) {
					gpt_partitions_exist = 1;
					break;
				}
			}
		}

		if(!gpt_partitions_exist) return DROIDBOOT_ENOENT;
		droidboot_log(DROIDBOOT_LOG_INFO, "found GPT\n");

		droidboot_sd_read_block(buf, 1, 1);

		struct gpt_header gpthdr;
		uint err = partition_parse_gpt_header(buf, &gpthdr);
		droidboot_dump_hex(DROIDBOOT_LOG_TRACE, buf, 16);
		if (err) {
			/* Check the backup gpt */

			uint64_t backup_header_lba = droidboot_sd_blkcnt() - 1;
			droidboot_sd_read_block(buf, backup_header_lba , 1);

			err = partition_parse_gpt_header(buf, &gpthdr);
			if (err) {
				droidboot_log(DROIDBOOT_LOG_ERROR, "GPT: Primary and backup signatures invalid\n");
				return DROIDBOOT_EILSEQ;
			}
		}

		uint32_t part_entry_cnt = droidboot_sd_blklen() / ENTRY_SIZE;
		uint64_t partition_0 = GET_LLWORD_FROM_BYTE(&buf[PARTITION_ENTRIES_OFFSET]);
		/* Read GPT Entries */
		droidboot_log(DROIDBOOT_LOG_TRACE, "Partition entries offset: %d\n", partition_0);
		for (i = 0; i < (DROIDBOOT_ROUNDUP(gpthdr.max_partition_count, part_entry_cnt)) / part_entry_cnt; i++) {
		    droidboot_sd_read_block(buf, 2 + i, 1);
		    droidboot_dump_hex(DROIDBOOT_LOG_TRACE, buf, 16);

			for (j = 0; j < part_entry_cnt; j++) {
				char type_guid[PARTITION_TYPE_GUID_SIZE];
				char name[MAX_GPT_NAME_SIZE];
				char UTF16_name[MAX_GPT_NAME_SIZE];
				uint64_t first_lba, last_lba, size;

				// guid
				memcpy(&type_guid,
			       &buf[(j * gpthdr.partition_entry_size)],
			       PARTITION_TYPE_GUID_SIZE);
				if (type_guid[0]==0 && type_guid[1]==0) {
					i = DROIDBOOT_ROUNDUP(gpthdr.max_partition_count, part_entry_cnt);
					//break;
				}

				// size
				first_lba = GET_LLWORD_FROM_BYTE(&buf[(j * gpthdr.partition_entry_size) + FIRST_LBA_OFFSET]);
				last_lba = GET_LLWORD_FROM_BYTE(&buf[(j * gpthdr.partition_entry_size) + LAST_LBA_OFFSET]);
				size = last_lba - first_lba + 1;

				// name
				memset(&UTF16_name, 0x00, MAX_GPT_NAME_SIZE);
				memcpy(UTF16_name, &buf[(j * gpthdr.partition_entry_size) +
					PARTITION_NAME_OFFSET], MAX_GPT_NAME_SIZE);

				/*
				 * Currently partition names in *.xml are UTF-8 and lowercase
				 * Only supporting english for now so removing 2nd byte of UTF-16
				 */
				for (n = 0; n < MAX_GPT_NAME_SIZE / 2; n++) {
					name[n] = UTF16_name[n * 2];
				}

                if(strcmp(name, "abm_settings")==0){
                   droidboot_log(DROIDBOOT_LOG_INFO, "FOUND abm settings\n");
                   abm_settings_offset=first_lba;
                   abm_settings_blkcnt=size;
                }
#ifdef DROIDBOOT_NO_SD_SUPPORT
                if(strcmp(name, "userdata")==0){
                   droidboot_log(DROIDBOOT_LOG_INFO, "FOUND userdata\n");
                   userdata_offset=first_lba;
                   userdata_blkcnt=size;
                }
#endif
#ifdef DROIDBOOT_NO_SD_ENCRYPTED_SUPPORT
                if(strcmp(name, "metadata")==0){
                   droidboot_log(DROIDBOOT_LOG_INFO, "FOUND metadata\n");
                   metadata_offset=first_lba;
                   metadata_blkcnt=size;
                }
#endif
				//droidboot_log(DROIDBOOT_LOG_INFO, "got part!!!!!!!!!!!!!! '%s' size=%llu!, first lba: %d\n", name, size, first_lba);
			}
		}

#ifdef DROIDBOOT_NO_SD_SUPPORT
	if(userdata_blkcnt!=0)
	{
		return DROIDBOOT_EOK;
	}
#endif

	if(abm_settings_blkcnt==0)
	{
		return DROIDBOOT_ENOENT;
	}
    return DROIDBOOT_EOK;
}
