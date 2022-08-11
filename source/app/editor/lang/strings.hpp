/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
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

#include <string>  // string

namespace tactile {

struct MenuStrings final
{
  std::string file;
  std::string edit;
  std::string view;
  std::string map;
  std::string tileset;
  std::string help;
  std::string debug;
  std::string recent_files;
  std::string widgets;
  std::string export_as;
};

struct WindowStrings final
{
  std::string layer_dock;
  std::string tileset_dock;
  std::string property_dock;
  std::string component_dock;
  std::string log_dock;
  std::string component_editor;
  std::string settings_dialog;
  std::string about_tactile;
  std::string create_new_map;
  std::string create_tileset;
  std::string resize_map;
  std::string create_component;
  std::string credits;
  std::string map_parse_error;
  std::string add_property;
  std::string change_property_type;
  std::string rename_property;
  std::string create_attribute;
  std::string rename_layer;
  std::string rename_component;
  std::string rename_component_attribute;
  std::string export_as_godot_scene;
};

struct ActionStrings final
{
  std::string create_map;
  std::string open_map;
  std::string save;
  std::string save_as;
  std::string close_document;
  std::string exit;
  std::string reopen_last_closed_file;
  std::string clear_file_history;

  std::string undo;
  std::string redo;

  std::string stamp_tool;
  std::string bucket_tool;
  std::string eraser_tool;
  std::string object_selection_tool;
  std::string rectangle_tool;
  std::string ellipse_tool;
  std::string point_tool;

  std::string show_component_editor;
  std::string show_settings;

  std::string center_viewport;
  std::string toggle_grid;
  std::string increase_zoom;
  std::string decrease_zoom;
  std::string reset_zoom;

  std::string increase_font_size;
  std::string decrease_font_size;
  std::string reset_font_size;

  std::string pan_up;
  std::string pan_down;
  std::string pan_right;
  std::string pan_left;

  std::string highlight_layer;
  std::string toggle_ui;

  std::string inspect_map;
  std::string inspect_tileset;

  std::string add_tileset;
  std::string add_row;
  std::string add_column;
  std::string remove_row;
  std::string remove_column;
  std::string resize_map;
  std::string fix_invalid_tiles;
  std::string fix_invalid_tiles_tooltip;

  std::string reset_layout;
  std::string report_issue;

  std::string show_about;
  std::string show_credits;
  std::string show_metrics;
  std::string open_persistent_file_dir;

  std::string about_dear_imgui;

  std::string quick_theme;
  std::string quick_language;

  std::string create_attribute;
  std::string duplicate_attribute;
  std::string rename_attribute;
  std::string remove_attribute;

  std::string rename_component;
  std::string remove_component;

  std::string clear_log;

  std::string create_property;
  std::string rename_property;
  std::string change_property_type;
  std::string remove_property;

  std::string tile_layer;
  std::string object_layer;
  std::string group_layer;
  std::string inspect_layer;
  std::string rename_layer;
  std::string duplicate_layer;
  std::string remove_layer;
  std::string toggle_layer_visible;
  std::string move_layer_up;
  std::string move_layer_down;

  std::string reset_attached_component;
  std::string detach_component;
  std::string component_editor;

  std::string create_tileset;
  std::string open_tileset;
  std::string remove_tileset;

  std::string export_as_godot_scene;
};

struct SettingStrings final
{
  std::string appearance_tab;
  std::string behavior_tab;
  std::string export_tab;

  std::string restore_defaults;
  std::string restore_last_session;
  std::string pref_tile_width;
  std::string pref_tile_height;
  std::string command_capacity;

  std::string language;
  std::string theme;
  std::string viewport_bg_color;
  std::string window_border;
  std::string restore_layout;
  std::string use_default_font;
  std::string font_size;
  std::string pref_format;
  std::string embed_tilesets;
  std::string indent_output;
  std::string fold_tile_data;
};

struct TooltipStrings final
{
  std::string fix_invalid_tiles;
  std::string pref_tile_width;
  std::string pref_tile_height;
  std::string command_capacity;
  std::string restore_layout;
  std::string use_default_font;
  std::string pref_format;
  std::string embed_tilesets;
  std::string indent_output;
  std::string fold_tile_data;
  std::string repository_link;
  std::string create_component;
  std::string show_component_actions;
  std::string add_new_layer;
  std::string remove_layer;
  std::string duplicate_layer;
  std::string move_layer_up;
  std::string move_layer_down;

  std::string log_verbose_msg;
  std::string log_debug_msg;
  std::string log_info_msg;
  std::string log_warn_msg;
  std::string log_error_msg;
  std::string log_critical_msg;
  std::string add_component;

  std::string godot_project_folder;
  std::string godot_map_folder;
  std::string godot_image_folder;
  std::string godot_tileset_folder;
  std::string godot_polygon_points;
};

struct MiscStrings final
{
  std::string ok;
  std::string cancel;
  std::string apply;
  std::string create;
  std::string close;
  std::string change;
  std::string add;
  std::string rename;
  std::string export_;
  std::string empty;

  std::string rows;
  std::string columns;

  std::string name;
  std::string type;
  std::string default_value;
  std::string component;

  std::string cause;

  std::string license_info;
  std::string repository_link;
  std::string font_awesome_credit;

  std::string credits_info;
  std::string license;
  std::string library;

  std::string tile_width;
  std::string tile_height;
  std::string tile_count;
  std::string tileset;
  std::string row_count;
  std::string column_count;
  std::string first_tile_id;
  std::string last_tile_id;
  std::string embedded;
  std::string map;
  std::string tile_layer;
  std::string object_layer;
  std::string group_layer;

  std::string opacity;
  std::string visible;

  std::string rectangle;
  std::string ellipse;
  std::string point;

  std::string width;
  std::string height;

  std::string tag;

  std::string select_image;
  std::string create_tileset_instruction;
  std::string tileset_image_input_hint;

  std::string empty_component;
  std::string map_has_no_components;
  std::string component_name_hint;

  std::string map_parse_error;
  std::string map_has_no_layers;

  std::string log_no_messages_match_filter;
  std::string filter;
  std::string log_trace_filter;
  std::string log_debug_filter;
  std::string log_info_filter;
  std::string log_warn_filter;
  std::string log_error_filter;
  std::string log_critical_filter;

  std::string property_name_hint;
  std::string type_int;
  std::string type_float;
  std::string type_bool;
  std::string type_string;
  std::string type_color;
  std::string type_object;
  std::string type_path;

  std::string context;
  std::string context_has_no_components;
  std::string no_available_components;

  std::string attribute_name_hint;

  std::string map_has_no_tilesets;

  std::string godot_project_folder_label;
  std::string godot_map_folder_label;
  std::string godot_image_folder_label;
  std::string godot_tileset_folder_label;
  std::string godot_polygon_points_label;
};

struct CommandStrings final
{
  std::string add_row;
  std::string add_rows;
  std::string remove_row;
  std::string remove_rows;
  std::string add_column;
  std::string add_columns;
  std::string remove_column;
  std::string remove_columns;
  std::string fix_map_tiles;
  std::string resize_map;

  std::string add_layer;
  std::string remove_layer;
  std::string duplicate_layer;
  std::string move_layer_up;
  std::string move_layer_down;
  std::string rename_layer;
  std::string set_layer_opacity;
  std::string show_layer;
  std::string hide_layer;

  std::string add_point_object;
  std::string add_rectangle_object;
  std::string add_ellipse_object;
  std::string move_object;
  std::string rename_object;
  std::string update_object_tag;
  std::string show_object;
  std::string hide_object;

  std::string define_comp;
  std::string undef_comp;
  std::string attach_comp;
  std::string detach_comp;
  std::string rename_comp;
  std::string create_comp_attr;
  std::string remove_comp_attr;
  std::string update_comp_attr;
  std::string duplicate_comp_attr;
  std::string update_comp_attr_defaults;
  std::string change_comp_attr_type;
  std::string rename_comp_attr;
  std::string reset_comp;

  std::string add_property;
  std::string remove_property;
  std::string rename_property;
  std::string update_property;
  std::string change_property_type;

  std::string add_tileset;
  std::string remove_tileset;
  std::string rename_tileset;

  std::string stamp_tool;
  std::string eraser_tool;
  std::string bucket_tool;
};

struct ParseErrorStrings final
{
  std::string none;
  std::string unknown;

  std::string could_not_read_file;
  std::string map_does_not_exist;

  std::string unsupported_map_format;
  std::string unsupported_map_orientation;
  std::string unsupported_infinite_map;
  std::string no_map_width;
  std::string no_map_height;
  std::string no_map_tile_width;
  std::string no_map_tile_height;
  std::string no_next_layer_id;
  std::string no_next_object_id;

  std::string invalid_layer_type;
  std::string no_layer_type;
  std::string no_tile_layer_data;
  std::string corrupt_tile_layer_data;
  std::string unsupported_tile_layer_encoding;

  std::string external_tileset_does_not_exist;
  std::string unknown_external_tileset_error;
  std::string no_first_tileset_tile_id;
  std::string no_tileset_tile_width;
  std::string no_tileset_tile_height;
  std::string no_tileset_name;
  std::string no_tileset_image_path;
  std::string no_tileset_image_width;
  std::string no_tileset_image_height;
  std::string no_tileset_tile_count;
  std::string no_tileset_column_count;
  std::string no_tileset_version;
  std::string no_external_tileset_source;
  std::string tileset_image_does_not_exist;
  std::string unsupported_tileset_version;

  std::string corrupt_property_value;
  std::string no_property_name;
  std::string no_property_type;
  std::string unsupported_property_type;

  std::string comp_def_no_name;
  std::string comp_def_no_attr_name;
  std::string comp_def_no_attr_type;
  std::string comp_def_unsupported_attr_type;
  std::string comp_def_corrupt_attr_value;

  std::string no_component_type;
  std::string no_component_attr_name;
  std::string no_component_attr_value;
  std::string corrupt_component_attr_value;

  std::string no_object_id;
  std::string no_object_type;
  std::string unsupported_object_type;

  std::string tile_def_no_id;
  std::string tile_def_no_anim_frame_tile;
  std::string tile_def_no_anim_frame_duration;

  std::string bad_tile_format_encoding;
  std::string bad_tile_format_compression;
  std::string bad_tile_format_endianness;
  std::string bad_zlib_compression_level;
  std::string plain_encoding_with_compression;
};

/// Provides translated strings for a single language.
struct Strings final
{
  MenuStrings       menu;
  WindowStrings     window;
  ActionStrings     action;
  SettingStrings    setting;
  TooltipStrings    tooltip;
  MiscStrings       misc;
  CommandStrings    cmd;
  ParseErrorStrings parse_error;
};

}  // namespace tactile