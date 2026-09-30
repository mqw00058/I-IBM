// Linux build: rename this Turk PLY library's public ply_* symbols so they don't clash with
// src/PoissonRecon/PlyFile.cpp (on Windows the two .obj files overwrote each other).
#pragma once
#ifndef _WIN32
#define ply_close mc_ply_close
#define ply_describe_element mc_ply_describe_element
#define ply_describe_other_elements mc_ply_describe_other_elements
#define ply_describe_other_properties mc_ply_describe_other_properties
#define ply_describe_property mc_ply_describe_property
#define ply_element_count mc_ply_element_count
#define ply_free_other_elements mc_ply_free_other_elements
#define ply_get_comments mc_ply_get_comments
#define ply_get_element mc_ply_get_element
#define ply_get_element_description mc_ply_get_element_description
#define ply_get_element_setup mc_ply_get_element_setup
#define ply_get_info mc_ply_get_info
#define ply_get_obj_info mc_ply_get_obj_info
#define ply_get_other_element mc_ply_get_other_element
#define ply_get_other_properties mc_ply_get_other_properties
#define ply_get_property mc_ply_get_property
#define ply_header_complete mc_ply_header_complete
#define ply_open_for_reading mc_ply_open_for_reading
#define ply_open_for_writing mc_ply_open_for_writing
#define ply_put_comment mc_ply_put_comment
#define ply_put_element mc_ply_put_element
#define ply_put_element_setup mc_ply_put_element_setup
#define ply_put_obj_info mc_ply_put_obj_info
#define ply_put_other_elements mc_ply_put_other_elements
#define ply_read mc_ply_read
#define ply_type_size mc_ply_type_size
#define ply_write mc_ply_write
#endif
