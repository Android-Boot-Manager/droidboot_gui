set(LV_CONF_INCLUDE_SIMPLE OFF CACHE INTERNAL "blah" FORCE)
add_subdirectory(lib/lvgl)

add_library(droidboot_gui STATIC
		lib/ext4/ext4.c
		lib/ext4/ext4_balloc.c
		lib/ext4/ext4_bcache.c
		lib/ext4/ext4_bitmap.c
		lib/ext4/ext4_block_group.c
		lib/ext4/ext4_blockdev.c
		lib/ext4/ext4_crc32.c
		lib/ext4/ext4_debug.c
		lib/ext4/ext4_dir.c
		lib/ext4/ext4_dir_idx.c
		lib/ext4/ext4_extent.c
		lib/ext4/ext4_fs.c
		lib/ext4/ext4_hash.c
		lib/ext4/ext4_ialloc.c
		lib/ext4/ext4_inode.c
		lib/ext4/ext4_journal.c
		lib/ext4/ext4_mbr.c
		lib/ext4/ext4_mkfs.c
		lib/ext4/ext4_super.c
		lib/ext4/ext4_trans.c
		lib/ext4/ext4_xattr.c
		lib/minigz/mini_gzip.c
		lib/minigz/miniz.c
		# List C/C++ source files with relative paths to this CMakeLists.txt.
		common/droidboot_helpers.c
		common/droidboot_init.c
		common/droidboot_logging.c
		common/droidboot_main.c
		common/droidboot_screens.c
		common/droidboot_stdfunc.c
		common/droidboot_theme.c
		storage/droidboot_gpt.c
		storage/droidboot_mapped.c
		config_parser/droidboot_config.c
		config_parser/droidboot_fs_util.c
		dualboot_gui/droidboot_boot_logo.c
		dualboot_gui/dualboot_menu.c
		extras/droidboot_dtb.c
		extras/droidboot_kernel_helper.c
		droidboot_platforms/common/droidboot_platform_common.c
		droidboot_platforms/simulator/simulator.c
		backend/droidboot_drivers.c
		backend/droidboot_dualboot_backend.c)
target_link_libraries(droidboot_gui PUBLIC lvgl android log jnigraphics)
add_compile_definitions(PLATFORM_SIMULATOR)
target_include_directories(droidboot_gui PUBLIC
		lib/libufdt/sysdeps/include
		lib/libufdt/include
		lib/ext4/include
		lib/minigz
		droidboot_platforms/common
		droidboot_platforms/simulator
		droidboot_platforms/libc-hack
		droidboot_platforms/libc-hack/lk
		include
		.
)
target_compile_options(droidboot_gui PUBLIC -fsanitize=address -fsanitize-recover=address -fno-omit-frame-pointer)
target_link_options(droidboot_gui PUBLIC -fsanitize=address -fsanitize-recover=address)