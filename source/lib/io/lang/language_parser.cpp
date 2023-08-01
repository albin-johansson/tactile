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

#include "language_parser.hpp"

#include <fmt/format.h>
#include <spdlog/spdlog.h>

#include "common/type/string.hpp"
#include "core/debug/panic.hpp"
#include "core/prelude.hpp"
#include "io/directories.hpp"
#include "io/json_utils.hpp"
#include "ui/style/icons.hpp"

namespace tactile {
namespace {

void _load(const JSON& json, String& string, const char* key)
{
  if (const auto* value = try_get(json, key)) {
    string = value->get<String>();
  }

  if (string.empty()) {
    if constexpr (kIsDebugBuild) {
      spdlog::critical("[IO] Found no translation for '{}'", key);
    }
    throw TactileError {"Invalid empty translated string"};
  }
}

void _load(const JSON& json, String& string, const char* key, const char* icon)
{
  if (const auto* value = try_get(json, key)) {
    string = fmt::format("{} {}", icon, value->get<String>());
  }

  if (string.empty()) {
    throw TactileError {"Invalid empty translated string"};
  }
}

void _load_menu_strings(const JSON& json, MenuStrings& menu)
{
  _load(json, menu.file, "file");
  _load(json, menu.edit, "edit");
  _load(json, menu.view, "view");
  _load(json, menu.map, "map");
  _load(json, menu.tileset, "tileset");
  _load(json, menu.help, "help");
  _load(json, menu.debug, "debug");
  _load(json, menu.recent_files, "recent-files", TAC_ICON_HISTORY);
  _load(json, menu.widgets, "widgets");
  _load(json, menu.export_as, "export-as");
}

void _load_window_strings(const JSON& json, WindowStrings& window)
{
  _load(json, window.layer_dock, "layer-dock");
  _load(json, window.tileset_dock, "tileset-dock");
  _load(json, window.property_dock, "property-dock");
  _load(json, window.component_dock, "component-dock");
  _load(json, window.log_dock, "log-dock");
  _load(json, window.animation_dock, "animation-dock");
  _load(json, window.component_editor, "component-editor");
  _load(json, window.settings_dialog, "settings-dialog");
  _load(json, window.about_tactile, "about-tactile");
  _load(json, window.create_new_map, "create-new-map");
  _load(json, window.create_tileset, "create-tileset");
  _load(json, window.resize_map, "resize-map");
  _load(json, window.create_component, "create-component");
  _load(json, window.credits, "credits");
  _load(json, window.map_parse_error, "map-parse-error");
  _load(json, window.add_property, "add-property");
  _load(json, window.change_property_type, "change-property-type");
  _load(json, window.rename_property, "rename-property");
  _load(json, window.create_attribute, "create-attribute");
  _load(json, window.rename_layer, "rename-layer");
  _load(json, window.rename_component, "rename-component");
  _load(json, window.rename_component_attribute, "rename-component-attribute");
  _load(json, window.export_as_godot_scene, "export-as-godot-scene");
}

void _load_action_strings(const JSON& json, ActionStrings& action)
{
  _load(json, action.create_map, "create-map", TAC_ICON_FILE);
  _load(json, action.open_map, "open-map", TAC_ICON_OPEN);
  _load(json, action.save, "save", TAC_ICON_SAVE);
  _load(json, action.save_as, "save-as", TAC_ICON_SAVE);
  _load(json, action.close_document, "close-document", TAC_ICON_CLOSE);
  _load(json, action.exit, "exit", TAC_ICON_EXIT);
  _load(json, action.reopen_last_closed_file, "reopen-last-closed-file", TAC_ICON_OPEN);
  _load(json, action.clear_file_history, "clear-file-history", TAC_ICON_CLEAR_HISTORY);

  _load(json, action.undo, "undo", TAC_ICON_UNDO);
  _load(json, action.redo, "redo", TAC_ICON_REDO);

  _load(json, action.stamp_tool, "stamp-tool", TAC_ICON_STAMP);
  _load(json, action.bucket_tool, "bucket-tool", TAC_ICON_BUCKET);
  _load(json, action.eraser_tool, "eraser-tool", TAC_ICON_ERASER);
  _load(json,
        action.object_selection_tool,
        "object-selection-tool",
        TAC_ICON_OBJECT_SELECTION);
  _load(json, action.rectangle_tool, "rectangle-tool", TAC_ICON_RECTANGLE);
  _load(json, action.ellipse_tool, "ellipse-tool", TAC_ICON_ELLIPSE);
  _load(json, action.point_tool, "point-tool", TAC_ICON_POINT);

  _load(json, action.show_component_editor, "show-component-editor", TAC_ICON_COMPONENT);
  _load(json, action.show_settings, "show-settings", TAC_ICON_SETTINGS);

  _load(json, action.center_viewport, "center-viewport", TAC_ICON_CENTER);
  _load(json, action.toggle_grid, "toggle-grid", TAC_ICON_GRID);
  _load(json, action.increase_zoom, "increase-zoom", TAC_ICON_ZOOM_IN);
  _load(json, action.decrease_zoom, "decrease-zoom", TAC_ICON_ZOOM_OUT);
  _load(json, action.reset_zoom, "reset-zoom", TAC_ICON_RESET_ZOOM);

  _load(json, action.increase_font_size, "increase-font-size");
  _load(json, action.decrease_font_size, "decrease-font-size");
  _load(json, action.reset_font_size, "reset-font-size");

  _load(json, action.pan_up, "pan-up", TAC_ICON_MOVE_UP);
  _load(json, action.pan_down, "pan-down", TAC_ICON_MOVE_DOWN);
  _load(json, action.pan_right, "pan-right", TAC_ICON_MOVE_RIGHT);
  _load(json, action.pan_left, "pan-left", TAC_ICON_MOVE_LEFT);

  _load(json, action.highlight_layer, "highlight-layer");
  _load(json, action.toggle_ui, "toggle-ui");

  _load(json, action.inspect_map, "inspect-map", TAC_ICON_INSPECT);
  _load(json, action.inspect_tileset, "inspect-tileset", TAC_ICON_INSPECT);
  _load(json, action.inspect_layer, "inspect-layer", TAC_ICON_INSPECT);
  _load(json, action.inspect_object, "inspect-object", TAC_ICON_INSPECT);

  _load(json, action.add_tileset, "add-tileset", TAC_ICON_TILESET);
  _load(json, action.add_row, "add-row");
  _load(json, action.add_column, "add-column");
  _load(json, action.remove_row, "remove-row");
  _load(json, action.remove_column, "remove-column");
  _load(json, action.resize_map, "resize-map");
  _load(json, action.fix_invalid_tiles, "fix-invalid-tiles", TAC_ICON_REPAIR);

  _load(json, action.reset_layout, "reset-layout");
  _load(json, action.report_issue, "report-issue", TAC_ICON_BUG);

  _load(json, action.show_about, "show-about", TAC_ICON_ABOUT);
  _load(json, action.about_dear_imgui, "about-dear-imgui", TAC_ICON_ABOUT);
  _load(json, action.show_credits, "show-credits");
  _load(json, action.show_metrics, "show-metrics", TAC_ICON_METRICS);
  _load(json, action.open_persistent_file_dir, "open-persistent-file-dir");

  _load(json, action.quick_theme, "quick-theme", ICON_FA_DROPLET);
  _load(json, action.quick_language, "quick-language", ICON_FA_GLOBE);

  _load(json, action.create_attribute, "create-attribute");
  _load(json, action.duplicate_attribute, "duplicate-attribute", TAC_ICON_DUPLICATE);
  _load(json, action.rename_attribute, "rename-attribute", TAC_ICON_EDIT);
  _load(json, action.remove_attribute, "remove-attribute", TAC_ICON_REMOVE);

  _load(json, action.rename_component, "rename-component", TAC_ICON_EDIT);
  _load(json, action.remove_component, "remove-component", TAC_ICON_REMOVE);

  _load(json, action.clear_log, "clear-log", TAC_ICON_CLEAR_HISTORY);

  _load(json, action.create_property, "create-property", TAC_ICON_ADD);
  _load(json, action.rename_property, "rename-property", TAC_ICON_EDIT);
  _load(json, action.change_property_type, "change-property-type", TAC_ICON_OBJECTS);
  _load(json, action.remove_property, "remove-property", TAC_ICON_REMOVE);

  _load(json, action.tile_layer, "tile-layer", TAC_ICON_TILE_LAYER);
  _load(json, action.object_layer, "object-layer", TAC_ICON_OBJECT_LAYER);
  _load(json, action.group_layer, "group-layer", TAC_ICON_GROUP_LAYER);
  _load(json, action.rename_layer, "rename-layer", TAC_ICON_EDIT);
  _load(json, action.duplicate_layer, "duplicate-layer", TAC_ICON_DUPLICATE);
  _load(json, action.remove_layer, "remove-layer", TAC_ICON_REMOVE);
  _load(json, action.toggle_layer_visible, "toggle-layer-visible", TAC_ICON_VISIBILITY);
  _load(json, action.move_layer_up, "move-layer-up", TAC_ICON_MOVE_UP);
  _load(json, action.move_layer_down, "move-layer-down", TAC_ICON_MOVE_DOWN);

  // clang-format off
  _load(json, action.toggle_object_visibility, "toggle-object-visibility", TAC_ICON_VISIBILITY);
  _load(json, action.duplicate_object, "duplicate-object", TAC_ICON_DUPLICATE);
  _load(json, action.remove_object, "remove-object", TAC_ICON_REMOVE);
  // clang-format on

  _load(json,
        action.reset_attached_component,
        "reset-attached-component",
        TAC_ICON_RESET);
  _load(json, action.detach_component, "detach-component", TAC_ICON_REMOVE);
  _load(json, action.component_editor, "component-editor", TAC_ICON_COMPONENT);

  _load(json, action.create_tileset, "create-tileset", TAC_ICON_ADD);
  _load(json, action.open_tileset, "open-tileset", TAC_ICON_OPEN);
  _load(json, action.remove_tileset, "remove-tileset", TAC_ICON_REMOVE);

  _load(json, action.export_as_godot_scene, "export-as-godot-scene");

  _load(json, action.top_left, "top-left");
  _load(json, action.top_right, "top-right");
  _load(json, action.bottom_left, "bottom-left");
  _load(json, action.bottom_right, "bottom-right");
  _load(json, action.show_frame_rate, "show-frame-rate", TAC_ICON_METRICS);
}

void _load_setting_strings(const JSON& json, SettingStrings& setting)
{
  _load(json, setting.behavior_tab, "behavior-tab");
  _load(json, setting.appearance_tab, "appearance-tab");
  _load(json, setting.export_tab, "export-tab");

  _load(json, setting.restore_defaults, "restore-defaults");
  _load(json, setting.restore_last_session, "restore-last-session");

  _load(json, setting.pref_tile_width, "pref-tile-width");
  _load(json, setting.pref_tile_height, "pref-tile-height");
  _load(json, setting.command_capacity, "command-capacity");

  _load(json, setting.language, "language");
  _load(json, setting.theme, "theme");
  _load(json, setting.saturation, "saturation");
  _load(json, setting.viewport_bg_color, "viewport-bg-color");
  _load(json, setting.grid_color, "grid-color");
  _load(json, setting.window_border, "window-border");
  _load(json, setting.restore_layout, "restore-layout");
  _load(json, setting.use_default_font, "use-default-font");
  _load(json, setting.font_size, "font-size");
  _load(json, setting.pref_format, "pref-format");
  _load(json, setting.embed_tilesets, "embed-tilesets");
  _load(json, setting.indent_output, "indent-output");
  _load(json, setting.fold_tile_data, "fold-tile-data");
}

void _load_tooltip_strings(const JSON& json, TooltipStrings& tooltip)
{
  _load(json, tooltip.fix_invalid_tiles, "fix-invalid-tiles");
  _load(json, tooltip.pref_tile_width, "pref-tile-width");
  _load(json, tooltip.pref_tile_height, "pref-tile-height");
  _load(json, tooltip.command_capacity, "command-capacity");
  _load(json, tooltip.restore_layout, "restore-layout");
  _load(json, tooltip.use_default_font, "use-default-font");
  _load(json, tooltip.pref_format, "pref-format");
  _load(json, tooltip.embed_tilesets, "embed-tilesets");
  _load(json, tooltip.indent_output, "indent-output");
  _load(json, tooltip.fold_tile_data, "fold-tile-data");
  _load(json, tooltip.repository_link, "repository-link");
  _load(json, tooltip.create_component, "create-component");
  _load(json, tooltip.show_component_actions, "show-component-actions");
  _load(json, tooltip.add_new_layer, "add-new-layer");
  _load(json, tooltip.remove_layer, "remove-layer");
  _load(json, tooltip.duplicate_layer, "duplicate-layer");
  _load(json, tooltip.move_layer_up, "move-layer-up");
  _load(json, tooltip.move_layer_down, "move-layer-down");

  _load(json, tooltip.stamp_random_tile, "stamp-random-tile");

  _load(json, tooltip.create_tileset, "create-tileset");

  _load(json, tooltip.log_verbose_msg, "log-verbose-msg");
  _load(json, tooltip.log_debug_msg, "log-debug-msg");
  _load(json, tooltip.log_info_msg, "log-info-msg");
  _load(json, tooltip.log_warn_msg, "log-warn-msg");
  _load(json, tooltip.log_error_msg, "log-error-msg");
  _load(json, tooltip.log_critical_msg, "log-critical-msg");
  _load(json, tooltip.add_component, "add-component");

  _load(json, tooltip.godot_project_folder, "godot-project-folder");
  _load(json, tooltip.godot_map_folder, "godot-map-folder");
  _load(json, tooltip.godot_image_folder, "godot-image-folder");
  _load(json, tooltip.godot_tileset_folder, "godot-tileset-folder");
  _load(json, tooltip.godot_polygon_points, "godot-polygon-points");
}

void _load_misc_strings(const JSON& json, MiscStrings& misc)
{
  _load(json, misc.ok, "ok");
  _load(json, misc.cancel, "cancel");
  _load(json, misc.apply, "apply");
  _load(json, misc.create, "create");
  _load(json, misc.close, "close");
  _load(json, misc.change, "change");
  _load(json, misc.add, "add");
  _load(json, misc.rename, "rename");
  _load(json, misc.export_, "export");
  _load(json, misc.empty, "empty");
  _load(json, misc.none, "none");

  _load(json, misc.undo, "undo");
  _load(json, misc.redo, "redo");

  _load(json, misc.row, "row");
  _load(json, misc.rows, "rows");
  _load(json, misc.column, "column");
  _load(json, misc.columns, "columns");

  _load(json, misc.name, "name");
  _load(json, misc.type, "type");
  _load(json, misc.default_value, "default-value");
  _load(json, misc.component, "component");

  _load(json, misc.cause, "cause");
  _load(json, misc.preview, "preview");

  _load(json, misc.license_info, "license-info");
  _load(json, misc.repository_link, "repository-link");
  _load(json, misc.font_awesome_credit, "font-awesome-credit", ICON_FA_FONT_AWESOME);

  _load(json, misc.credits_info, "credits-info");
  _load(json, misc.library, "library");
  _load(json, misc.license, "license");

  _load(json, misc.tile_width, "tile-width");
  _load(json, misc.tile_height, "tile-height");
  _load(json, misc.tile_count, "tile-count");
  _load(json, misc.row_count, "row-count");
  _load(json, misc.column_count, "column-count");
  _load(json, misc.tileset, "tileset");
  _load(json, misc.tile, "tile");
  _load(json, misc.first_tile_id, "first-tile-id");
  _load(json, misc.last_tile_id, "last-tile-id");
  _load(json, misc.embedded, "embedded");
  _load(json, misc.map, "map");
  _load(json, misc.tile_layer, "tile-layer");
  _load(json, misc.object_layer, "object-layer");
  _load(json, misc.group_layer, "group-layer");

  _load(json, misc.opacity, "opacity");
  _load(json, misc.visible, "visible");

  _load(json, misc.rectangle, "rectangle");
  _load(json, misc.ellipse, "ellipse");
  _load(json, misc.point, "point");

  _load(json, misc.stamp_tool, "stamp-tool");
  _load(json, misc.eraser_tool, "eraser-tool");
  _load(json, misc.bucket_tool, "bucket-tool");
  _load(json, misc.object_selection_tool, "object-selection-tool");
  _load(json, misc.rectangle_tool, "rectangle-tool");
  _load(json, misc.ellipse_tool, "ellipse-tool");
  _load(json, misc.point_tool, "point-tool");

  _load(json, misc.width, "width");
  _load(json, misc.height, "height");
  _load(json, misc.size, "size");

  _load(json, misc.tag, "tag");
  _load(json, misc.index, "index");
  _load(json, misc.animated, "animated");

  _load(json, misc.select_image, "select-image");
  _load(json, misc.create_tileset_instruction, "create-tileset-instruction");
  _load(json, misc.tileset_image_input_hint, "tileset-image-input-hint");

  _load(json, misc.empty_component, "empty-component");
  _load(json, misc.map_has_no_components, "map-has-no-components");
  _load(json, misc.component_name_hint, "component-name-hint");

  _load(json, misc.map_parse_error, "map-parse-error", ICON_FA_TRIANGLE_EXCLAMATION);
  _load(json, misc.map_has_no_layers, "map-has-no-layers");

  _load(json, misc.log_no_messages_match_filter, "log-no-messages-match-filter");
  _load(json, misc.filter, "filter");
  _load(json, misc.log_trace_filter, "log-trace-filter");
  _load(json, misc.log_debug_filter, "log-debug-filter");
  _load(json, misc.log_info_filter, "log-info-filter");
  _load(json, misc.log_warn_filter, "log-warn-filter");
  _load(json, misc.log_error_filter, "log-error-filter");
  _load(json, misc.log_critical_filter, "log-critical-filter");

  _load(json, misc.property_name_hint, "property-name-hint");
  _load(json, misc.type_int, "type-int");
  _load(json, misc.type_int2, "type-int2");
  _load(json, misc.type_int3, "type-int3");
  _load(json, misc.type_int4, "type-int4");
  _load(json, misc.type_float, "type-float");
  _load(json, misc.type_float2, "type-float2");
  _load(json, misc.type_float3, "type-float3");
  _load(json, misc.type_float4, "type-float4");
  _load(json, misc.type_bool, "type-bool");
  _load(json, misc.type_string, "type-string");
  _load(json, misc.type_color, "type-color");
  _load(json, misc.type_object, "type-object");
  _load(json, misc.type_path, "type-path");

  _load(json, misc.context, "context");
  _load(json, misc.context_has_no_components, "context-has-no-components");
  _load(json, misc.no_available_components, "no-available-components");

  _load(json, misc.attribute_name_hint, "attribute-name-hint");

  _load(json, misc.map_has_no_tilesets, "map-has-no-tilesets");

  _load(json, misc.godot_project_folder_label, "godot-project-folder-label");
  _load(json, misc.godot_map_folder_label, "godot-map-folder-label");
  _load(json, misc.godot_image_folder_label, "godot-image-folder-label");
  _load(json, misc.godot_tileset_folder_label, "godot-tileset-folder-label");
  _load(json, misc.godot_polygon_points_label, "godot-polygon-points-label");

  _load(json, misc.tile_encoding, "tile-encoding");
  _load(json, misc.plain_encoding, "plain-encoding");
  _load(json, misc.tile_compression, "tile-compression");
  _load(json, misc.compression_level, "compression-level");

  _load(json, misc.local_id, "local-id");
  _load(json, misc.global_id, "global-id");
}

void _load_command_strings(const JSON& json, CommandStrings& cmd)
{
  _load(json, cmd.add_row, "add-row");
  _load(json, cmd.add_rows, "add-rows");
  _load(json, cmd.remove_row, "remove-row");
  _load(json, cmd.remove_rows, "remove-rows");
  _load(json, cmd.add_column, "add-columns");
  _load(json, cmd.add_columns, "add-columns");
  _load(json, cmd.remove_column, "remove-columns");
  _load(json, cmd.remove_columns, "remove-columns");
  _load(json, cmd.fix_map_tiles, "fix-map-tiles");
  _load(json, cmd.resize_map, "resize-map");

  _load(json, cmd.set_tile_format_encoding, "set-tile-format-encoding");
  _load(json, cmd.set_tile_format_compression, "set-tile-format-compression");

  _load(json, cmd.add_layer, "add-layer");
  _load(json, cmd.remove_layer, "remove-layer");
  _load(json, cmd.duplicate_layer, "duplicate-layer");
  _load(json, cmd.move_layer_up, "move-layer-up");
  _load(json, cmd.move_layer_down, "move-layer-down");
  _load(json, cmd.rename_layer, "rename-layer");
  _load(json, cmd.set_layer_opacity, "set-layer-opacity");
  _load(json, cmd.show_layer, "show-layer");
  _load(json, cmd.hide_layer, "hide-layer");

  _load(json, cmd.add_point_object, "add-point-object");
  _load(json, cmd.add_rectangle_object, "add-rectangle-object");
  _load(json, cmd.add_ellipse_object, "add-ellipse-object");
  _load(json, cmd.move_object, "move-object");
  _load(json, cmd.rename_object, "rename-object");
  _load(json, cmd.update_object_tag, "update-object-tag");
  _load(json, cmd.show_object, "show-object");
  _load(json, cmd.hide_object, "hide-object");
  _load(json, cmd.remove_point_object, "remove-point-object");
  _load(json, cmd.remove_rectangle_object, "remove-rectangle-object");
  _load(json, cmd.remove_ellipse_object, "remove-ellipse-object");

  _load(json, cmd.define_comp, "define-comp");
  _load(json, cmd.undef_comp, "undef-comp");
  _load(json, cmd.attach_comp, "attach-comp");
  _load(json, cmd.detach_comp, "detach-comp");
  _load(json, cmd.rename_comp, "rename-comp");
  _load(json, cmd.create_comp_attr, "create-comp-attr");
  _load(json, cmd.remove_comp_attr, "remove-comp-attr");
  _load(json, cmd.update_comp_attr, "update-comp-attr");
  _load(json, cmd.duplicate_comp_attr, "duplicate-comp-attr");
  _load(json, cmd.update_comp_attr_defaults, "update-comp-attr-defaults");
  _load(json, cmd.change_comp_attr_type, "change-comp-attr-type");
  _load(json, cmd.rename_comp_attr, "rename-comp-attr");
  _load(json, cmd.reset_comp, "reset-comp");

  _load(json, cmd.add_property, "add-property");
  _load(json, cmd.remove_property, "remove-property");
  _load(json, cmd.rename_property, "rename-property");
  _load(json, cmd.update_property, "update-property");
  _load(json, cmd.change_property_type, "change-property-type");

  _load(json, cmd.add_tileset, "add-tileset");
  _load(json, cmd.remove_tileset, "remove-tileset");
  _load(json, cmd.rename_tileset, "rename-tileset");

  _load(json, cmd.delete_animation, "delete-animation");
  _load(json, cmd.add_animation_frame, "add-animation-frame");
  _load(json, cmd.remove_animation_frame, "remove-animation-frame");
  _load(json, cmd.set_animation_frame_duration, "set-animation-frame-duration");
  _load(json, cmd.move_animation_frame_forwards, "move-animation-frame-forwards");
  _load(json, cmd.move_animation_frame_backwards, "move-animation-frame-backwards");
  _load(json, cmd.rename_tile, "rename-tile");

  _load(json, cmd.stamp_tool, "stamp-tool");
  _load(json, cmd.eraser_tool, "eraser-tool");
  _load(json, cmd.bucket_tool, "bucket-tool");

  _load(json, cmd.set_zlib_compression_level, "set-zlib-compression-level");
  _load(json, cmd.set_zstd_compression_level, "set-zstd-compression-level");
}

void _load_parse_error_strings(const JSON& json, ParseErrorStrings& err)
{
  _load(json, err.none, "none");
  _load(json, err.unknown, "unknown");

  _load(json, err.could_not_read_file, "could-not-read-file");
  _load(json, err.map_does_not_exist, "map-does-not-exist");

  _load(json, err.unsupported_map_format, "unsupported-map-format");
  _load(json, err.unsupported_map_orientation, "unsupported-map-orientation");
  _load(json, err.unsupported_infinite_map, "unsupported-infinite-map");
  _load(json, err.no_map_width, "no-map-width");
  _load(json, err.no_map_height, "no-map-height");
  _load(json, err.no_map_tile_width, "no-map-tile-width");
  _load(json, err.no_map_tile_height, "no-map-tile-height");
  _load(json, err.no_next_layer_id, "no-next-layer-id");
  _load(json, err.no_next_object_id, "no-next-object-id");

  _load(json, err.invalid_layer_type, "invalid-layer-type");
  _load(json, err.no_layer_type, "no-layer-type");
  _load(json, err.no_tile_layer_data, "no-tile-layer-data");
  _load(json, err.corrupt_tile_layer_data, "corrupt-tile-layer-data");
  _load(json, err.unsupported_tile_layer_encoding, "unsupported-tile-layer-encoding");

  _load(json, err.external_tileset_does_not_exist, "external-tileset-does-not-exist");
  _load(json, err.unknown_external_tileset_error, "unknown-external-tileset-error");
  _load(json, err.no_first_tileset_tile_id, "no-first-tileset-tile-id");
  _load(json, err.no_tileset_tile_width, "no-tileset-tile-width");
  _load(json, err.no_tileset_tile_height, "no-tileset-tile-height");
  _load(json, err.no_tileset_name, "no-tileset-name");
  _load(json, err.no_tileset_image_path, "no-tileset-image-path");
  _load(json, err.no_tileset_image_width, "no-tileset-image-width");
  _load(json, err.no_tileset_image_height, "no-tileset-image-height");
  _load(json, err.no_tileset_tile_count, "no-tileset-tile-count");
  _load(json, err.no_tileset_column_count, "no-tileset-column-count");
  _load(json, err.no_tileset_version, "no-tileset-version");
  _load(json, err.no_external_tileset_source, "no-external-tileset-source");
  _load(json, err.tileset_image_does_not_exist, "tileset-image-does-not-exist");
  _load(json, err.unsupported_tileset_version, "unsupported-tileset-version");

  _load(json, err.corrupt_property_value, "corrupt-property-value");
  _load(json, err.no_property_name, "no-property-name");
  _load(json, err.no_property_type, "no-property-type");
  _load(json, err.unsupported_property_type, "unsupported-property-type");

  _load(json, err.comp_def_no_name, "comp-def-no-name");
  _load(json, err.comp_def_no_attr_name, "comp-def-no-attr-name");
  _load(json, err.comp_def_no_attr_type, "comp-def-no-attr-type");
  _load(json, err.comp_def_unsupported_attr_type, "comp-def-unsupported-attr-type");
  _load(json, err.comp_def_corrupt_attr_value, "comp-def-corrupt-attr-value");

  _load(json, err.no_component_type, "no-component-type");
  _load(json, err.no_component_attr_name, "no-component-attr-name");
  _load(json, err.no_component_attr_value, "no-component-attr-value");
  _load(json, err.corrupt_component_attr_value, "corrupt-component-attr-value");

  _load(json, err.no_object_id, "no-object-id");
  _load(json, err.no_object_type, "no-object-type");
  _load(json, err.unsupported_object_type, "unsupported-object-type");

  _load(json, err.tile_def_no_id, "tile-def-no-id");
  _load(json, err.tile_def_no_anim_frame_tile, "tile-def-no-anim-frame-tile");
  _load(json, err.tile_def_no_anim_frame_duration, "tile-def-no-anim-frame-duration");

  _load(json, err.bad_tile_format_encoding, "bad-tile-format-encoding");
  _load(json, err.bad_tile_format_compression, "bad-tile-format-compression");
  _load(json, err.bad_zlib_compression_level, "bad-zlib-compression-level");
  _load(json, err.bad_zstd_compression_level, "bad-zstd-compression-level");
  _load(json, err.plain_encoding_with_compression, "plain-encoding-with-compression");
}

void _load_animation_dock_strings(const JSON& json, AnimationDockStrings& dock)
{
  _load(json, dock.title, "title");
  _load(json, dock.frames, "frames");
  _load(json, dock.duration, "duration", TAC_ICON_DURATION);
  _load(json, dock.tile_has_no_animation, "tile-has-no-animation");
  _load(json, dock.no_selected_tile_hint, "no-selected-tile-hint");
  _load(json, dock.move_frame_forwards, "move-frame-forwards", TAC_ICON_MOVE_LEFT);
  _load(json, dock.move_frame_backwards, "move-frame-backwards", TAC_ICON_MOVE_RIGHT);
  _load(json, dock.select_referenced_tile, "select-referenced-tile", TAC_ICON_LOCATE);
  _load(json, dock.remove_frame, "remove-frame", TAC_ICON_REMOVE);
  _load(json, dock.delete_animation, "delete-animation", TAC_ICON_DELETE);

  // clang-format off
  _load(json, dock.new_animation_frame_selection_hint, "new-animation-frame-selection-hint");
  // clang-format on
}

}  // namespace

auto parse_language(const char* path, const Strings& fallback) -> Strings
{
  const auto resource = find_resource(path);
  const auto json = parse_json_file(resource).value();

  auto strings = fallback;

  if (const auto* menus = try_get(json, "menus")) {
    _load_menu_strings(*menus, strings.menu);
  }

  if (const auto* windows = try_get(json, "windows")) {
    _load_window_strings(*windows, strings.window);
  }

  if (const auto* actions = try_get(json, "actions")) {
    _load_action_strings(*actions, strings.action);
  }

  if (const auto* settings = try_get(json, "settings")) {
    _load_setting_strings(*settings, strings.setting);
  }

  if (const auto* tooltips = try_get(json, "tooltips")) {
    _load_tooltip_strings(*tooltips, strings.tooltip);
  }

  if (const auto* misc = try_get(json, "misc")) {
    _load_misc_strings(*misc, strings.misc);
  }

  if (const auto* commands = try_get(json, "commands")) {
    _load_command_strings(*commands, strings.cmd);
  }

  if (const auto* err = try_get(json, "parse-errors")) {
    _load_parse_error_strings(*err, strings.parse_error);
  }

  if (const auto* dock = try_get(json, "animation-dock")) {
    _load_animation_dock_strings(*dock, strings.animation_dock);
  }

  return strings;
}

}  // namespace tactile
