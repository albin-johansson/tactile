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

#include "language_parser.hpp"

#include <fmt/format.h>

#include "editor/ui/icons.hpp"
#include "io/directories.hpp"
#include "io/maps/json_utils.hpp"  // TODO move
#include "misc/panic.hpp"

using JSON = nlohmann::json;

namespace tactile::io {
namespace {

void load(const JSON& json, std::string& string, const char* key)
{
  if (const auto* value = try_get(json, key)) {
    string = *value;
  }

  if (string.empty()) {
    throw TactileError {"Invalid empty translated string!"};
  }
}

void load(const JSON& json, std::string& string, const char* key, const char* icon)
{
  if (const auto* value = try_get(json, key)) {
    string = fmt::format("{} {}", icon, *value);
  }

  if (string.empty()) {
    throw TactileError {"Invalid empty translated string!"};
  }
}

void load_menu_strings(const JSON& json, MenuStrings& menu)
{
  load(json, menu.file, "file");
  load(json, menu.edit, "edit");
  load(json, menu.view, "view");
  load(json, menu.map, "map");
  load(json, menu.tileset, "tileset");
  load(json, menu.help, "help");
  load(json, menu.debug, "debug");
  load(json, menu.recent_files, "recent-files", TAC_ICON_HISTORY);
  load(json, menu.widgets, "widgets");
}

void load_window_strings(const JSON& json, WindowStrings& window)
{
  load(json, window.layer_dock, "layer-dock");
  load(json, window.tileset_dock, "tileset-dock");
  load(json, window.property_dock, "property-dock");
  load(json, window.component_dock, "component-dock");
  load(json, window.log_dock, "log-dock");
  load(json, window.component_editor, "component-editor");
  load(json, window.settings_dialog, "settings-dialog");
  load(json, window.about_tactile, "about-tactile");
  load(json, window.create_new_map, "create-new-map");
  load(json, window.create_tileset, "create-tileset");
  load(json, window.resize_map, "resize-map");
  load(json, window.create_component, "create-component");
  load(json, window.credits, "credits");
  load(json, window.map_parse_error, "map-parse-error");
  load(json, window.add_property, "add-property");
  load(json, window.change_property_type, "change-property-type");
  load(json, window.rename_property, "rename-property");
  load(json, window.create_attribute, "create-attribute");
  load(json, window.rename_layer, "rename-layer");
  load(json, window.rename_component, "rename-component");
  load(json, window.rename_component_attribute, "rename-component-attribute");
}

void load_action_strings(const JSON& json, ActionStrings& action)
{
  load(json, action.create_map, "create-map", TAC_ICON_FILE);
  load(json, action.open_map, "open-map", TAC_ICON_OPEN);
  load(json, action.save, "save", TAC_ICON_SAVE);
  load(json, action.save_as, "save-as", TAC_ICON_SAVE);
  load(json, action.close_document, "close-document", TAC_ICON_CLOSE);
  load(json, action.exit, "exit", TAC_ICON_EXIT);
  load(json, action.reopen_last_closed_file, "reopen-last-closed-file", TAC_ICON_OPEN);
  load(json, action.clear_file_history, "clear-file-history", TAC_ICON_CLEAR_HISTORY);

  load(json, action.undo, "undo", TAC_ICON_UNDO);
  load(json, action.redo, "redo", TAC_ICON_REDO);

  load(json, action.stamp_tool, "stamp-tool", TAC_ICON_STAMP);
  load(json, action.bucket_tool, "bucket-tool", TAC_ICON_BUCKET);
  load(json, action.eraser_tool, "eraser-tool", TAC_ICON_ERASER);
  load(json,
       action.object_selection_tool,
       "object-selection-tool",
       TAC_ICON_OBJECT_SELECTION);
  load(json, action.rectangle_tool, "rectangle-tool", TAC_ICON_RECTANGLE);
  load(json, action.ellipse_tool, "ellipse-tool", TAC_ICON_ELLIPSE);
  load(json, action.point_tool, "point-tool", TAC_ICON_POINT);

  load(json, action.show_component_editor, "show-component-editor", TAC_ICON_COMPONENT);
  load(json, action.show_settings, "show-settings", TAC_ICON_SETTINGS);

  load(json, action.center_viewport, "center-viewport", TAC_ICON_CENTER);
  load(json, action.toggle_grid, "toggle-grid", TAC_ICON_GRID);
  load(json, action.increase_zoom, "increase-zoom", TAC_ICON_ZOOM_IN);
  load(json, action.decrease_zoom, "decrease-zoom", TAC_ICON_ZOOM_OUT);
  load(json, action.reset_zoom, "reset-zoom", TAC_ICON_RESET_ZOOM);

  load(json, action.increase_font_size, "increase-font-size");
  load(json, action.decrease_font_size, "decrease-font-size");
  load(json, action.reset_font_size, "reset-font-size");

  load(json, action.pan_up, "pan-up", TAC_ICON_MOVE_UP);
  load(json, action.pan_down, "pan-down", TAC_ICON_MOVE_DOWN);
  load(json, action.pan_right, "pan-right", TAC_ICON_MOVE_RIGHT);
  load(json, action.pan_left, "pan-left", TAC_ICON_MOVE_LEFT);

  load(json, action.highlight_layer, "highlight-layer");
  load(json, action.toggle_ui, "toggle-ui");

  load(json, action.inspect_map, "inspect-map", TAC_ICON_INSPECT);
  load(json, action.inspect_tileset, "inspect-tileset", TAC_ICON_INSPECT);

  load(json, action.add_tileset, "add-tileset", TAC_ICON_TILESET);
  load(json, action.add_row, "add-row");
  load(json, action.add_column, "add-column");
  load(json, action.remove_row, "remove-row");
  load(json, action.remove_column, "remove-column");
  load(json, action.resize_map, "resize-map");
  load(json, action.fix_invalid_tiles, "fix-invalid-tiles", TAC_ICON_REPAIR);

  load(json, action.reset_layout, "reset-layout");
  load(json, action.report_issue, "report-issue", TAC_ICON_BUG);

  load(json, action.show_about, "show-about", TAC_ICON_ABOUT);
  load(json, action.about_dear_imgui, "about-dear-imgui", TAC_ICON_ABOUT);
  load(json, action.show_credits, "show-credits");
  load(json, action.show_metrics, "show-metrics", TAC_ICON_METRICS);
  load(json, action.open_persistent_file_dir, "open-persistent-file-dir");

  load(json, action.quick_theme, "quick-theme", ICON_FA_DROPLET);
  load(json, action.quick_language, "quick-language", ICON_FA_GLOBE);

  load(json, action.create_attribute, "create-attribute");
  load(json, action.duplicate_attribute, "duplicate-attribute", TAC_ICON_DUPLICATE);
  load(json, action.rename_attribute, "rename-attribute", TAC_ICON_EDIT);
  load(json, action.remove_attribute, "remove-attribute", TAC_ICON_REMOVE);

  load(json, action.rename_component, "rename-component", TAC_ICON_EDIT);
  load(json, action.remove_component, "remove-component", TAC_ICON_REMOVE);

  load(json, action.clear_log, "clear-log", TAC_ICON_CLEAR_HISTORY);

  load(json, action.create_property, "create-property", TAC_ICON_ADD);
  load(json, action.rename_property, "rename-property", TAC_ICON_EDIT);
  load(json, action.change_property_type, "change-property-type", TAC_ICON_OBJECTS);
  load(json, action.remove_property, "remove-property", TAC_ICON_REMOVE);

  load(json, action.tile_layer, "tile-layer", TAC_ICON_TILE_LAYER);
  load(json, action.object_layer, "object-layer", TAC_ICON_OBJECT_LAYER);
  load(json, action.group_layer, "group-layer", TAC_ICON_GROUP_LAYER);
  load(json, action.inspect_layer, "inspect-layer", TAC_ICON_INSPECT);
  load(json, action.rename_layer, "rename-layer", TAC_ICON_EDIT);
  load(json, action.duplicate_layer, "duplicate-layer", TAC_ICON_DUPLICATE);
  load(json, action.remove_layer, "remove-layer", TAC_ICON_REMOVE);
  load(json, action.toggle_layer_visible, "toggle-layer-visible", TAC_ICON_VISIBILITY);
  load(json, action.move_layer_up, "move-layer-up", TAC_ICON_MOVE_UP);
  load(json, action.move_layer_down, "move-layer-down", TAC_ICON_MOVE_DOWN);

  load(json, action.reset_attached_component, "reset-attached-component", TAC_ICON_RESET);
  load(json, action.detach_component, "detach-component", TAC_ICON_REMOVE);
  load(json, action.component_editor, "component-editor", TAC_ICON_COMPONENT);

  load(json, action.create_tileset, "create-tileset", TAC_ICON_ADD);
  load(json, action.open_tileset, "open-tileset", TAC_ICON_OPEN);
  load(json, action.remove_tileset, "remove-tileset", TAC_ICON_REMOVE);
}

void load_setting_strings(const JSON& json, SettingStrings& setting)
{
  load(json, setting.behavior_tab, "behavior-tab");
  load(json, setting.appearance_tab, "appearance-tab");
  load(json, setting.export_tab, "export-tab");

  load(json, setting.restore_defaults, "restore-defaults");
  load(json, setting.restore_last_session, "restore-last-session");

  load(json, setting.pref_tile_width, "pref-tile-width");
  load(json, setting.pref_tile_height, "pref-tile-height");
  load(json, setting.command_capacity, "command-capacity");

  load(json, setting.language, "language");
  load(json, setting.theme, "theme");
  load(json, setting.viewport_bg_color, "viewport-bg-color");
  load(json, setting.window_border, "window-border");
  load(json, setting.restore_layout, "restore-layout");
  load(json, setting.use_default_font, "use-default-font");
  load(json, setting.font_size, "font-size");
  load(json, setting.pref_format, "pref-format");
  load(json, setting.embed_tilesets, "embed-tilesets");
  load(json, setting.indent_output, "indent-output");
  load(json, setting.fold_tile_data, "fold-tile-data");
}

void load_tooltip_strings(const JSON& json, TooltipStrings& tooltip)
{
  load(json, tooltip.fix_invalid_tiles, "fix-invalid-tiles");
  load(json, tooltip.pref_tile_width, "pref-tile-width");
  load(json, tooltip.pref_tile_height, "pref-tile-height");
  load(json, tooltip.command_capacity, "command-capacity");
  load(json, tooltip.restore_layout, "restore-layout");
  load(json, tooltip.use_default_font, "use-default-font");
  load(json, tooltip.pref_format, "pref-format");
  load(json, tooltip.embed_tilesets, "embed-tilesets");
  load(json, tooltip.indent_output, "indent-output");
  load(json, tooltip.fold_tile_data, "fold-tile-data");
  load(json, tooltip.repository_link, "repository-link");
  load(json, tooltip.create_component, "create-component");
  load(json, tooltip.show_component_actions, "show-component-actions");
  load(json, tooltip.add_new_layer, "add-new-layer");
  load(json, tooltip.remove_layer, "remove-layer");
  load(json, tooltip.duplicate_layer, "duplicate-layer");
  load(json, tooltip.move_layer_up, "move-layer-up");
  load(json, tooltip.move_layer_down, "move-layer-down");

  load(json, tooltip.log_verbose_msg, "log-verbose-msg");
  load(json, tooltip.log_debug_msg, "log-debug-msg");
  load(json, tooltip.log_info_msg, "log-info-msg");
  load(json, tooltip.log_warn_msg, "log-warn-msg");
  load(json, tooltip.log_error_msg, "log-error-msg");
  load(json, tooltip.log_critical_msg, "log-critical-msg");
  load(json, tooltip.add_component, "add-component");
}

void load_misc_strings(const JSON& json, MiscStrings& misc)
{
  load(json, misc.ok, "ok");
  load(json, misc.cancel, "cancel");
  load(json, misc.apply, "apply");
  load(json, misc.create, "create");
  load(json, misc.close, "close");
  load(json, misc.change, "change");
  load(json, misc.add, "add");
  load(json, misc.rename, "rename");
  load(json, misc.empty, "empty");

  load(json, misc.rows, "rows");
  load(json, misc.columns, "columns");

  load(json, misc.name, "name");
  load(json, misc.type, "type");
  load(json, misc.default_value, "default-value");
  load(json, misc.component, "component");

  load(json, misc.cause, "cause");

  load(json, misc.license_info, "license-info");
  load(json, misc.repository_link, "repository-link");
  load(json, misc.font_awesome_credit, "font-awesome-credit", ICON_FA_FONT_AWESOME);

  load(json, misc.credits_info, "credits-info");
  load(json, misc.library, "library");
  load(json, misc.license, "license");

  load(json, misc.tile_width, "tile-width");
  load(json, misc.tile_height, "tile-height");
  load(json, misc.tile_count, "tile-count");
  load(json, misc.row_count, "row-count");
  load(json, misc.column_count, "column-count");
  load(json, misc.tileset, "tileset");
  load(json, misc.first_tile_id, "first-tile-id");
  load(json, misc.last_tile_id, "last-tile-id");
  load(json, misc.embedded, "embedded");
  load(json, misc.map, "map");
  load(json, misc.tile_layer, "tile-layer");
  load(json, misc.object_layer, "object-layer");
  load(json, misc.group_layer, "group-layer");

  load(json, misc.opacity, "opacity");
  load(json, misc.visible, "visible");

  load(json, misc.rectangle, "rectangle");
  load(json, misc.ellipse, "ellipse");
  load(json, misc.point, "point");

  load(json, misc.width, "width");
  load(json, misc.height, "height");

  load(json, misc.tag, "tag");

  load(json, misc.select_image, "select-image");
  load(json, misc.create_tileset_instruction, "create-tileset-instruction");
  load(json, misc.tileset_image_input_hint, "tileset-image-input-hint");

  load(json, misc.empty_component, "empty-component");
  load(json, misc.map_has_no_components, "map-has-no-components");
  load(json, misc.component_name_hint, "component-name-hint");

  load(json, misc.map_parse_error, "map-parse-error", ICON_FA_TRIANGLE_EXCLAMATION);
  load(json, misc.map_has_no_layers, "map-has-no-layers");

  load(json, misc.log_no_messages_match_filter, "log-no-messages-match-filter");
  load(json, misc.filter, "filter");
  load(json, misc.log_trace_filter, "log-trace-filter");
  load(json, misc.log_debug_filter, "log-debug-filter");
  load(json, misc.log_info_filter, "log-info-filter");
  load(json, misc.log_warn_filter, "log-warn-filter");
  load(json, misc.log_error_filter, "log-error-filter");
  load(json, misc.log_critical_filter, "log-critical-filter");

  load(json, misc.property_name_hint, "property-name-hint");
  load(json, misc.type_int, "type-int");
  load(json, misc.type_float, "type-float");
  load(json, misc.type_bool, "type-bool");
  load(json, misc.type_string, "type-string");
  load(json, misc.type_color, "type-color");
  load(json, misc.type_object, "type-object");
  load(json, misc.type_path, "type-path");

  load(json, misc.context, "context");
  load(json, misc.context_has_no_components, "context-has-no-components");
  load(json, misc.no_available_components, "no-available-components");

  load(json, misc.attribute_name_hint, "attribute-name-hint");
}

void load_parse_error_strings(const JSON& json, ParseErrorStrings& err)
{
  load(json, err.none, "none");
  load(json, err.unknown, "unknown");

  load(json, err.could_not_read_file, "could-not-read-file");
  load(json, err.map_does_not_exist, "map-does-not-exist");

  load(json, err.unsupported_map_format, "unsupported-map-format");
  load(json, err.unsupported_map_orientation, "unsupported-map-orientation");
  load(json, err.unsupported_infinite_map, "unsupported-infinite-map");
  load(json, err.no_map_width, "no-map-width");
  load(json, err.no_map_height, "no-map-height");
  load(json, err.no_map_tile_width, "no-map-tile-width");
  load(json, err.no_map_tile_height, "no-map-tile-height");
  load(json, err.no_next_layer_id, "no-next-layer-id");
  load(json, err.no_next_object_id, "no-next-object-id");

  load(json, err.invalid_layer_type, "invalid-layer-type");
  load(json, err.no_layer_type, "no-layer-type");
  load(json, err.no_tile_layer_data, "no-tile-layer-data");
  load(json, err.corrupt_tile_layer_data, "corrupt-tile-layer-data");
  load(json, err.unsupported_tile_layer_encoding, "unsupported-tile-layer-encoding");

  load(json, err.external_tileset_does_not_exist, "external-tileset-does-not-exist");
  load(json, err.unknown_external_tileset_error, "unknown-external-tileset-error");
  load(json, err.no_first_tileset_tile_id, "no-first-tileset-tile-id");
  load(json, err.no_tileset_tile_width, "no-tileset-tile-width");
  load(json, err.no_tileset_tile_height, "no-tileset-tile-height");
  load(json, err.no_tileset_name, "no-tileset-name");
  load(json, err.no_tileset_image_path, "no-tileset-image-path");
  load(json, err.no_tileset_image_width, "no-tileset-image-width");
  load(json, err.no_tileset_image_height, "no-tileset-image-height");
  load(json, err.no_tileset_tile_count, "no-tileset-tile-count");
  load(json, err.no_tileset_column_count, "no-tileset-column-count");
  load(json, err.no_tileset_version, "no-tileset-version");
  load(json, err.no_external_tileset_source, "no-external-tileset-source");
  load(json, err.tileset_image_does_not_exist, "tileset-image-does-not-exist");
  load(json, err.unsupported_tileset_version, "unsupported-tileset-version");

  load(json, err.corrupt_property_value, "corrupt-property-value");
  load(json, err.no_property_name, "no-property-name");
  load(json, err.no_property_type, "no-property-type");
  load(json, err.unsupported_property_type, "unsupported-property-type");

  load(json, err.comp_def_no_name, "comp-def-no-name");
  load(json, err.comp_def_no_attr_name, "comp-def-no-attr-name");
  load(json, err.comp_def_no_attr_type, "comp-def-no-attr-type");
  load(json, err.comp_def_unsupported_attr_type, "comp-def-unsupported-attr-type");
  load(json, err.comp_def_corrupt_attr_value, "comp-def-corrupt-attr-value");

  load(json, err.no_component_type, "no-component-type");
  load(json, err.no_component_attr_name, "no-component-attr-name");
  load(json, err.no_component_attr_value, "no-component-attr-value");
  load(json, err.corrupt_component_attr_value, "corrupt-component-attr-value");

  load(json, err.no_object_id, "no-object-id");
  load(json, err.no_object_type, "no-object-type");
  load(json, err.unsupported_object_type, "unsupported-object-type");

  load(json, err.tile_def_no_id, "tile-def-no-id");
  load(json, err.tile_def_no_anim_frame_tile, "tile-def-no-anim-frame-tile");
  load(json, err.tile_def_no_anim_frame_duration, "tile-def-no-anim-frame-duration");
}

}  // namespace

auto parse_language(const char* path, const Strings& fallback) -> Strings
{
  const auto resource = find_resource(path);
  const auto json = read_json(resource).value();

  auto strings = fallback;

  if (const auto* menus = try_get(json, "menus")) {
    load_menu_strings(*menus, strings.menu);
  }

  if (const auto* windows = try_get(json, "windows")) {
    load_window_strings(*windows, strings.window);
  }

  if (const auto* actions = try_get(json, "actions")) {
    load_action_strings(*actions, strings.action);
  }

  if (const auto* settings = try_get(json, "settings")) {
    load_setting_strings(*settings, strings.setting);
  }

  if (const auto* tooltips = try_get(json, "tooltips")) {
    load_tooltip_strings(*tooltips, strings.tooltip);
  }

  if (const auto* misc = try_get(json, "misc")) {
    load_misc_strings(*misc, strings.misc);
  }

  if (const auto* err = try_get(json, "parse-errors")) {
    load_parse_error_strings(*err, strings.parse_error);
  }

  return strings;
}

}  // namespace tactile::io
