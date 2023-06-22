/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include "common/type/string.hpp"

namespace tactile {

// TODO experiment with enum identifiers as indices into vector of translated strings,
//   this could prove a nicer to maintain while also keeping about the same performance

struct MenuStrings final {
  String file;
  String edit;
  String view;
  String map;
  String tileset;
  String help;
  String debug;
  String recent_files;
  String widgets;
  String export_as;
};

struct WindowStrings final {
  String layer_dock;
  String tileset_dock;
  String property_dock;
  String component_dock;
  String log_dock;
  String animation_dock;
  String component_editor;
  String settings_dialog;
  String about_tactile;
  String create_new_map;
  String create_tileset;
  String resize_map;
  String create_component;
  String credits;
  String map_parse_error;
  String add_property;
  String change_property_type;
  String rename_property;
  String create_attribute;
  String rename_layer;
  String rename_component;
  String rename_component_attribute;
  String export_as_godot_scene;
};

struct ActionStrings final {
  String create_map;
  String open_map;
  String save;
  String save_as;
  String close_document;
  String exit;
  String reopen_last_closed_file;
  String clear_file_history;

  String undo;
  String redo;

  String stamp_tool;
  String bucket_tool;
  String eraser_tool;
  String object_selection_tool;
  String rectangle_tool;
  String ellipse_tool;
  String point_tool;

  String show_component_editor;
  String show_settings;

  String center_viewport;
  String toggle_grid;
  String increase_zoom;
  String decrease_zoom;
  String reset_zoom;

  String increase_font_size;
  String decrease_font_size;
  String reset_font_size;

  String pan_up;
  String pan_down;
  String pan_right;
  String pan_left;

  String highlight_layer;
  String toggle_ui;

  String inspect_map;
  String inspect_tileset;
  String inspect_layer;
  String inspect_object;

  String add_tileset;
  String add_row;
  String add_column;
  String remove_row;
  String remove_column;
  String resize_map;
  String fix_invalid_tiles;
  String fix_invalid_tiles_tooltip;

  String reset_layout;
  String report_issue;

  String show_about;
  String show_credits;
  String show_metrics;
  String open_persistent_file_dir;

  String about_dear_imgui;

  String quick_theme;
  String quick_language;

  String create_attribute;
  String duplicate_attribute;
  String rename_attribute;
  String remove_attribute;

  String rename_component;
  String remove_component;

  String clear_log;

  String create_property;
  String rename_property;
  String change_property_type;
  String remove_property;

  String tile_layer;
  String object_layer;
  String group_layer;
  String rename_layer;
  String duplicate_layer;
  String remove_layer;
  String toggle_layer_visible;
  String move_layer_up;
  String move_layer_down;

  String toggle_object_visibility;
  String duplicate_object;
  String remove_object;

  String reset_attached_component;
  String detach_component;
  String component_editor;

  String create_tileset;
  String open_tileset;
  String remove_tileset;

  String export_as_godot_scene;

  String top_left;
  String top_right;
  String bottom_left;
  String bottom_right;
  String show_frame_rate;
};

struct SettingStrings final {
  String appearance_tab;
  String behavior_tab;
  String export_tab;

  String restore_defaults;
  String restore_last_session;
  String pref_tile_width;
  String pref_tile_height;
  String command_capacity;

  String language;
  String theme;
  String saturation;
  String viewport_bg_color;
  String grid_color;
  String window_border;
  String restore_layout;
  String use_default_font;
  String font_size;
  String pref_format;
  String embed_tilesets;
  String indent_output;
  String fold_tile_data;
};

struct TooltipStrings final {
  String fix_invalid_tiles;
  String pref_tile_width;
  String pref_tile_height;
  String command_capacity;
  String restore_layout;
  String use_default_font;
  String pref_format;
  String embed_tilesets;
  String indent_output;
  String fold_tile_data;
  String repository_link;
  String create_component;
  String show_component_actions;
  String add_new_layer;
  String remove_layer;
  String duplicate_layer;
  String move_layer_up;
  String move_layer_down;

  String stamp_random_tile;

  String create_tileset;

  String log_verbose_msg;
  String log_debug_msg;
  String log_info_msg;
  String log_warn_msg;
  String log_error_msg;
  String log_critical_msg;
  String add_component;

  String godot_project_folder;
  String godot_map_folder;
  String godot_image_folder;
  String godot_tileset_folder;
  String godot_polygon_points;
};

struct MiscStrings final {
  String ok;
  String cancel;
  String apply;
  String create;
  String close;
  String change;
  String add;
  String rename;
  String export_;
  String empty;
  String none;

  String undo;
  String redo;

  String row;
  String rows;
  String column;
  String columns;

  String name;
  String type;
  String default_value;
  String component;

  String cause;
  String preview;

  String license_info;
  String repository_link;
  String font_awesome_credit;

  String credits_info;
  String license;
  String library;

  String tile_width;
  String tile_height;
  String tile_count;
  String tileset;
  String tile;
  String row_count;
  String column_count;
  String first_tile_id;
  String last_tile_id;
  String embedded;
  String map;
  String tile_layer;
  String object_layer;
  String group_layer;

  String opacity;
  String visible;

  String rectangle;
  String ellipse;
  String point;

  String stamp_tool;
  String eraser_tool;
  String bucket_tool;
  String object_selection_tool;
  String rectangle_tool;
  String ellipse_tool;
  String point_tool;

  String width;
  String height;
  String size;

  String tag;
  String index;
  String animated;

  String select_image;
  String create_tileset_instruction;
  String tileset_image_input_hint;

  String empty_component;
  String map_has_no_components;
  String component_name_hint;

  String map_parse_error;
  String map_has_no_layers;

  String log_no_messages_match_filter;
  String filter;
  String log_trace_filter;
  String log_debug_filter;
  String log_info_filter;
  String log_warn_filter;
  String log_error_filter;
  String log_critical_filter;

  String property_name_hint;
  String type_int;
  String type_int2;
  String type_int3;
  String type_int4;
  String type_float;
  String type_float2;
  String type_float3;
  String type_float4;
  String type_bool;
  String type_string;
  String type_color;
  String type_object;
  String type_path;

  String context;
  String context_has_no_components;
  String no_available_components;

  String attribute_name_hint;

  String map_has_no_tilesets;

  String godot_project_folder_label;
  String godot_map_folder_label;
  String godot_image_folder_label;
  String godot_tileset_folder_label;
  String godot_polygon_points_label;

  String tile_encoding;
  String plain_encoding;
  String tile_compression;
  String compression_level;

  String local_id;
  String global_id;
};

struct CommandStrings final {
  String add_row;
  String add_rows;
  String remove_row;
  String remove_rows;
  String add_column;
  String add_columns;
  String remove_column;
  String remove_columns;
  String fix_map_tiles;
  String resize_map;

  String set_tile_format_encoding;
  String set_tile_format_compression;

  String add_layer;
  String remove_layer;
  String duplicate_layer;
  String move_layer_up;
  String move_layer_down;
  String rename_layer;
  String set_layer_opacity;
  String show_layer;
  String hide_layer;

  String add_point_object;
  String add_rectangle_object;
  String add_ellipse_object;
  String move_object;
  String rename_object;
  String update_object_tag;
  String show_object;
  String hide_object;
  String remove_point_object;
  String remove_rectangle_object;
  String remove_ellipse_object;

  String define_comp;
  String undef_comp;
  String attach_comp;
  String detach_comp;
  String rename_comp;
  String create_comp_attr;
  String remove_comp_attr;
  String update_comp_attr;
  String duplicate_comp_attr;
  String update_comp_attr_defaults;
  String change_comp_attr_type;
  String rename_comp_attr;
  String reset_comp;

  String add_property;
  String remove_property;
  String rename_property;
  String update_property;
  String change_property_type;

  String add_tileset;
  String remove_tileset;
  String rename_tileset;

  String delete_animation;
  String add_animation_frame;
  String remove_animation_frame;
  String set_animation_frame_duration;
  String move_animation_frame_forwards;
  String move_animation_frame_backwards;
  String rename_tile;

  String stamp_tool;
  String eraser_tool;
  String bucket_tool;

  String set_zlib_compression_level;
  String set_zstd_compression_level;
};

struct ParseErrorStrings final {
  String none;
  String unknown;

  String could_not_read_file;
  String map_does_not_exist;

  String unsupported_map_format;
  String unsupported_map_orientation;
  String unsupported_infinite_map;
  String no_map_width;
  String no_map_height;
  String no_map_tile_width;
  String no_map_tile_height;
  String no_next_layer_id;
  String no_next_object_id;

  String invalid_layer_type;
  String no_layer_type;
  String no_tile_layer_data;
  String corrupt_tile_layer_data;
  String unsupported_tile_layer_encoding;

  String external_tileset_does_not_exist;
  String unknown_external_tileset_error;
  String no_first_tileset_tile_id;
  String no_tileset_tile_width;
  String no_tileset_tile_height;
  String no_tileset_name;
  String no_tileset_image_path;
  String no_tileset_image_width;
  String no_tileset_image_height;
  String no_tileset_tile_count;
  String no_tileset_column_count;
  String no_tileset_version;
  String no_external_tileset_source;
  String tileset_image_does_not_exist;
  String unsupported_tileset_version;

  String corrupt_property_value;
  String no_property_name;
  String no_property_type;
  String unsupported_property_type;

  String comp_def_no_name;
  String comp_def_no_attr_name;
  String comp_def_no_attr_type;
  String comp_def_unsupported_attr_type;
  String comp_def_corrupt_attr_value;

  String no_component_type;
  String no_component_attr_name;
  String no_component_attr_value;
  String corrupt_component_attr_value;

  String no_object_id;
  String no_object_type;
  String unsupported_object_type;

  String tile_def_no_id;
  String tile_def_no_anim_frame_tile;
  String tile_def_no_anim_frame_duration;

  String bad_tile_format_encoding;
  String bad_tile_format_compression;
  String bad_zlib_compression_level;
  String bad_zstd_compression_level;
  String plain_encoding_with_compression;
};

struct AnimationDockStrings final {
  String title;
  String frames;
  String duration;
  String tile_has_no_animation;
  String no_selected_tile_hint;
  String move_frame_forwards;
  String move_frame_backwards;
  String select_referenced_tile;
  String remove_frame;
  String delete_animation;
  String new_animation_frame_selection_hint;
};

/// Provides translated strings for a single language.
struct Strings final {
  MenuStrings menu;
  WindowStrings window;
  ActionStrings action;
  SettingStrings setting;
  TooltipStrings tooltip;
  MiscStrings misc;
  CommandStrings cmd;
  ParseErrorStrings parse_error;
  AnimationDockStrings animation_dock;
};

}  // namespace tactile
