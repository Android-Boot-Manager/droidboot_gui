#include <droidboot_error.h>
#include <droidboot_logging.h>
#include <droidboot_mapped.h>
#include <droidboot_platform_common.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ext4.h>
#include <libc-hack.h>


file_metadata droidboot_read_file_metadata(const char *metadata_file_path, unsigned long physical_partition_offset) {
    ext4_file fp;
    	size_t rb;
    file_metadata metadata;
    memset(&metadata, 0, sizeof(metadata)); // Initialize metadata structure

    droidboot_error ret = ext4_fopen(&fp, metadata_file_path, "r");
    if (ret!=DROIDBOOT_EOK) {
        droidboot_log(DROIDBOOT_LOG_ERROR, "Error opening metadata file: %d\n", ret);
    }

    // Move to the end to get the file size
    ext4_fseek(&fp, 0, SEEK_END);
    long fsize = ext4_ftell(&fp);
    ext4_fseek(&fp, 0, SEEK_SET);

    // Allocate memory to read the whole file
    char *buffer = (char *)malloc(fsize + 1);
    if (buffer == NULL) {
        ext4_fclose(&fp);
        droidboot_log(DROIDBOOT_LOG_ERROR, "Memory allocation failed.\n");

    }

    // Read the entire file into memory
    ext4_fread(&fp, buffer, fsize, &rb);
    ext4_fclose(&fp); // Close the file as it's no longer needed
    buffer[fsize] = 0; // Null-terminate the buffer

    // Parse the buffer
    char *ptr = buffer;
    int itemsParsed = sscanf(ptr, "%*s %lu %lu %d", &metadata.size, &metadata.block_size, &metadata.range_count);
    if (itemsParsed != 3) {
        droidboot_log(DROIDBOOT_LOG_ERROR, "Failed to parse file size and block size.\n");
        free(buffer);
            while(1);
    }

    // Skip header lines
    ptr = strchr(ptr, '\n') + 1;
    ptr = strchr(ptr, '\n') + 1;

    // Allocate memory for ranges
    metadata.ranges = (file_range *)malloc(metadata.range_count * sizeof(file_range));
    if (metadata.ranges == NULL) {
        droidboot_log(DROIDBOOT_LOG_ERROR, "Memory allocation for ranges failed.\n");
        free(buffer);
    }

    // Parse each range
    for (int i = 0; i < metadata.range_count; ++i) {
        if ((ptr = strchr(ptr, '\n')) != NULL) {
            ptr++; // Move past the newline character
            itemsParsed = sscanf(ptr, "%lu %lu", &metadata.ranges[i].start_block, &metadata.ranges[i].end_block);
            if (itemsParsed != 2) {
                droidboot_log(DROIDBOOT_LOG_ERROR, "Failed to parse range %d.\n", i);
                // Cleanup might be needed here
            }
        }
    }

    free(buffer); // Free the buffer as it's no longer needed

    metadata.physical_partition_offset = physical_partition_offset;
    droidboot_log(DROIDBOOT_LOG_INFO, "Parsed map, file size: %lu, block size: %lu, range count: %d\n", metadata.size, metadata.block_size, metadata.range_count);

    return metadata;
}

void droidboot_read_mapped_file_blocks(const file_metadata *metadata, unsigned long logical_offset, unsigned long block_count, void *buffer) {
    unsigned long remaining_blocks = block_count;
    unsigned long buffer_offset = 0;
    const unsigned long block_size = metadata->block_size;
    unsigned long logical_block = logical_offset;
    for (int range_index = 0; range_index < metadata->range_count && remaining_blocks > 0; range_index++) {
        unsigned long range_start = metadata->ranges[range_index].start_block;
        unsigned long range_end = metadata->ranges[range_index].end_block;
        unsigned long range_blocks = range_end - range_start + 1;
        // Check if the current logical block falls within this range
        if (logical_block < range_blocks) {
            // The block is within this range, calculate the actual block to read
            while (remaining_blocks > 0 && logical_block < range_blocks) {
                unsigned long actual_block_number = range_start + logical_block;
                droidboot_sd_read_block((void *)buffer + buffer_offset, actual_block_number + metadata->physical_partition_offset, 1);

                remaining_blocks--;
                buffer_offset += block_size;
                logical_block++;
            }
        } else {
            // The block is not in this range, skip to the next range
            logical_block -= range_blocks;
        }
    }
    if (remaining_blocks > 0) {
        droidboot_log(DROIDBOOT_LOG_ERROR, "Error: Requested blocks exceed file size or are out of bounds.\n");
    }
}
