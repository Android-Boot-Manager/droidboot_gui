#pragma once
#if 1
typedef struct {
    unsigned long start_block;
    unsigned long end_block;
} file_range;

typedef struct {
    unsigned long size;
    unsigned long block_size;
    unsigned long physical_partition_offset;
    int range_count;
    file_range *ranges;
} file_metadata;

file_metadata droidboot_read_file_metadata(const char *metadata_file_path, unsigned long physical_partition_offset);
void droidboot_read_mapped_file_blocks(const file_metadata *metadata, unsigned long logical_offset, unsigned long block_count, void *buffer);
#endif
