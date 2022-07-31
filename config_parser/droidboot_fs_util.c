#include <stdlib.h>
#include <stdio.h>

#include <ext4.h>

int dir_count_entries(const char *path) {
    const ext4_direntry *de;
    ext4_dir d;

    // Ensure we can open directory.

    ext4_dir_open (&d, path);

    // Process each entry.
    int res=0;
    de = ext4_dir_entry_next(&d);
	while (de) {
		if(de->inode_type==EXT4_DE_REG_FILE){
		  res+=1;
		}
		de = ext4_dir_entry_next(&d);
	}
	ext4_dir_close(&d);
    return res;
}
