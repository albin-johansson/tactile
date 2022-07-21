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

using JSON = nlohmann::json;

namespace tactile::io {
namespace {

void _load(const JSON& json, std::string& string, const char* key)
{
  if (const auto* value = try_get(json, key)) {
    string = *value;
  }
}

void _load(const JSON& json, std::string& string, const char* key, const char* icon)
{
  if (const auto* value = try_get(json, key)) {
    string = fmt::format("{} {}", icon, *value);
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
}

void _load_window_strings(const JSON& json, WindowStrings& window)
{
  _load(json, window.layer_dock, "layer-dock");
  _load(json, window.tileset_dock, "tileset-dock");
  _load(json, window.property_dock, "property-dock");
  _load(json, window.component_dock, "component-dock");
  _load(json, window.log_dock, "log-dock");
  _load(json, window.component_editor, "component-editor");
  _load(json, window.settings_dialog, "settings-dialog");
  _load(json, window.about_tactile, "about-tactile");
  _load(json, window.create_new_map, "create-new-map");
  _load(json, window.create_tileset, "create-tileset");
  _load(json, window.resize_map, "resize-map");
  _load(json, window.credits, "credits");
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
  _load(json, setting.viewport_bg_color, "viewport-bg-color");
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
}

void _load_misc_strings(const JSON& json, MiscStrings& misc)
{
  _load(json, misc.ok, "ok");
  _load(json, misc.cancel, "cancel");
  _load(json, misc.apply, "apply");
  _load(json, misc.create, "create");
  _load(json, misc.close, "close");

  _load(json, misc.rows, "rows");
  _load(json, misc.columns, "columns");

  _load(json, misc.license_info, "license-info");
  _load(json, misc.repository_link, "repository-link");
  _load(json, misc.font_awesome_credit, "font-awesome-credit", ICON_FA_FONT_AWESOME);
  _load(json, misc.credits_info, "credits-info");

  _load(json, misc.tile_width, "tile-width");
  _load(json, misc.tile_height, "tile-height");
  _load(json, misc.select_image, "select-image");
  _load(json, misc.create_tileset_instruction, "create-tileset-instruction");
  _load(json, misc.tileset_image_input_hint, "tileset-image-input-hint");
}

}  // namespace

auto parse_language(const char* path, const Strings& fallback) -> Strings
{
  const auto resource = find_resource(path);
  const auto json = read_json(resource).value();

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

  return strings;
}

}  // namespace tactile::io
