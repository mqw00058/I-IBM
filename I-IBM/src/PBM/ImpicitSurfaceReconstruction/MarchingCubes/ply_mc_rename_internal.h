// Linux build: file-internal helpers of plyfile.c, renamed for the same reason (see ply_mc_rename.h).
#pragma once
#ifndef _WIN32
#define add_comment mc_add_comment
#define add_element mc_add_element
#define add_obj_info mc_add_obj_info
#define add_property mc_add_property
#define ascii_get_element mc_ascii_get_element
#define binary_get_element mc_binary_get_element
#define copy_property mc_copy_property
#define equal_strings mc_equal_strings
#define find_element mc_find_element
#define find_property mc_find_property
#define get_ascii_item mc_get_ascii_item
#define get_binary_item mc_get_binary_item
#define get_item_value mc_get_item_value
#define get_prop_type mc_get_prop_type
#define get_stored_item mc_get_stored_item
#define get_words mc_get_words
#define old_write_ascii_item mc_old_write_ascii_item
#define setup_other_props mc_setup_other_props
#define store_item mc_store_item
#define type_names mc_type_names
#define write_ascii_item mc_write_ascii_item
#define write_binary_item mc_write_binary_item
#define write_scalar_type mc_write_scalar_type
#endif
