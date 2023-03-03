#include <lvgl.h>
#include <ext4.h>
// This function must not be here, as lvgl already have its own fs support, but it is too buggy
const void* droidboot_load_lvgl_image_from_ext4(char* path){
    int ret=0;
	ext4_file fp;
	
	ret = ext4_fopen (&fp, path, "r");
    if(ret!=0){
		return NULL;
    }

    ext4_fseek(&fp, 0, SEEK_END);
    off_t entry_file_size = ext4_ftell(&fp);
    ext4_fseek(&fp, 0, SEEK_SET);  /* same as rewind(f); */

    off_t header_len      = sizeof(lv_img_header_t);          // file header struct
    off_t buf_len         = entry_file_size - header_len;     // file size minus header size
    lv_img_dsc_t* img_dsc = (lv_img_dsc_t*)malloc(sizeof(lv_img_dsc_t));     // image descriptor struct
    unsigned char *buf    = (unsigned char *)malloc(buf_len);                  // pixel data only

    ext4_fread(&fp, img_dsc, header_len, NULL);
    ext4_fread(&fp, buf, buf_len, NULL);

    ext4_fclose(&fp);

    img_dsc->data      = buf;
    img_dsc->data_size = buf_len;
    return img_dsc;
}

