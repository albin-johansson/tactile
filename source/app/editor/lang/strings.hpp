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
};

struct MiscStrings final
{
  std::string ok;
  std::string cancel;
  std::string apply;
  std::string create;
  std::string close;

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
  std::string select_image;
  std::string create_tileset_instruction;
  std::string tileset_image_input_hint;

  std::string empty_component;
  std::string map_has_no_components;
  std::string component_name_hint;

  std::string map_parse_error;
  std::string map_has_no_layers;
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
  ParseErrorStrings parse_error;
};

}  // namespace tactile