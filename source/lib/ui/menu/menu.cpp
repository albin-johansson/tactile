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

#include "menu.hpp"

#include <utility>  // move

#include <fmt/format.h>

#include "common/debug/profile.hpp"
#include "common/predef.hpp"
#include "common/type/chrono.hpp"
#include "common/type/hash_map.hpp"
#include "common/util/assoc.hpp"
#include "core/map.hpp"
#include "core/tileset.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/document.hpp"
#include "model/file_history.hpp"
#include "model/model.hpp"
#include "model/settings.hpp"
#include "model/systems/document_system.hpp"
#include "ui/constants.hpp"

namespace tactile {
namespace {

// TODO menu entities?

inline HashMap<MenuAction, MenuItem> gItems;

void _set_label(const MenuAction action, String label)
{
  gItems[action].text = std::move(label);
}

void _update_tileset_menu(const Model& model, const Entity document_entity)
{
  const auto* tileset_document =
      model.unchecked_try_get<TilesetDocument>(document_entity);

  menu_set_enabled(MenuAction::InspectTileset, tileset_document != nullptr);
}

void _update_map_menu(const Model& model, const Entity document_entity)
{
  const auto* map_document = model.unchecked_try_get<MapDocument>(document_entity);
  const auto is_map_document = map_document != nullptr;

  const Map* map = nullptr;
  if (map_document) {
    map = model.unchecked_try_get<Map>(map_document->map);
  }

  menu_set_enabled(MenuAction::InspectMap, is_map_document);
  menu_set_enabled(MenuAction::AddTileset, is_map_document);

  menu_set_enabled(MenuAction::AddRow, is_map_document);
  menu_set_enabled(MenuAction::AddColumn, is_map_document);
  menu_set_enabled(MenuAction::RemoveRow, map && map->extent.rows > 1);
  menu_set_enabled(MenuAction::RemoveColumn, map && map->extent.cols > 1);

  menu_set_enabled(MenuAction::ExportGodotScene, is_map_document);
}

void _update_view_menu(const Model& model, const Entity document_entity)
{
  const auto& settings = get_global_settings();

  const auto* document = model.unchecked_try_get<Document>(document_entity);
  const auto* viewport = model.unchecked_try_get<Viewport>(document_entity);
  const auto* map_document = model.unchecked_try_get<MapDocument>(document_entity);

  const auto is_document_open = document != nullptr;
  const auto is_map_document = map_document != nullptr;

  menu_set_enabled(MenuAction::CenterViewport, is_document_open);

  menu_set_enabled(MenuAction::ResetZoom, is_document_open);
  menu_set_enabled(MenuAction::IncreaseZoom, is_document_open);
  menu_set_enabled(MenuAction::DecreaseZoom, viewport && viewport->can_zoom_out());

  const auto using_default_font = settings.test_flag(SETTINGS_USE_DEFAULT_FONT_BIT);
  menu_set_enabled(MenuAction::IncreaseFontSize,
                   !using_default_font && settings.get_font_size() < kMaxFontSize);
  menu_set_enabled(MenuAction::DecreaseFontSize,
                   !using_default_font && settings.get_font_size() > kMinFontSize);
  menu_set_enabled(MenuAction::ResetFontSize,
                   !settings.test_flag(SETTINGS_USE_DEFAULT_FONT_BIT));

  menu_set_enabled(MenuAction::PanUp, is_document_open);
  menu_set_enabled(MenuAction::PanDown, is_document_open);
  menu_set_enabled(MenuAction::PanRight, is_document_open);
  menu_set_enabled(MenuAction::PanLeft, is_document_open);

  menu_set_enabled(MenuAction::HighlightLayer, is_map_document);
  menu_set_enabled(MenuAction::ToggleUi, is_document_open);
}

void _update_tool_menu_items(const Model& model, const MapDocument* map_document)
{
  // TODO
  //  auto update_tool = [&](const MenuAction action, const ToolType tool) {
  //    if (map_document) {
  //      const auto& tools = map_document->get_tools();
  //      menu_set_enabled(action, tools.is_available(model, tool));
  //      menu_set_selected(action, tools.is_enabled(tool));
  //    }
  //    else {
  //      menu_set_enabled(action, false);
  //    }
  //  };
  //
  //  update_tool(MenuAction::StampTool, ToolType::Stamp);
  //  update_tool(MenuAction::EraserTool, ToolType::Eraser);
  //  update_tool(MenuAction::BucketTool, ToolType::Bucket);
  //  update_tool(MenuAction::ObjectSelectionTool, ToolType::ObjectSelection);
  //  update_tool(MenuAction::RectangleTool, ToolType::Rectangle);
  //  update_tool(MenuAction::EllipseTool, ToolType::Ellipse);
  //  update_tool(MenuAction::PointTool, ToolType::Point);
}

void _update_edit_menu(const Model& model, const Entity document_entity)
{
  const auto& lang = get_current_language();

  const auto* map_document = model.unchecked_try_get<MapDocument>(document_entity);
  const auto* command_stack = model.unchecked_try_get<CommandStack>(document_entity);

  const auto can_undo = command_stack && command_stack->can_undo();
  const auto can_redo = command_stack && command_stack->can_redo();

  auto& undo_item = lookup_in(gItems, MenuAction::Undo);
  undo_item.enabled = can_undo;
  undo_item.text = fmt::format("{} {}",
                               lang.action.undo,
                               can_undo ? command_stack->get_undo_text() : "");

  auto& redo_item = lookup_in(gItems, MenuAction::Redo);
  redo_item.enabled = can_redo;
  redo_item.text = fmt::format("{} {}",
                               lang.action.redo,
                               can_redo ? command_stack->get_redo_text() : "");

  _update_tool_menu_items(model, map_document);

  menu_set_enabled(MenuAction::ComponentEditor, map_document != nullptr);
}

void _update_file_menu(const Model& model, const Entity document_entity)
{
  const auto has_active_document = sys::has_active_document(model);

  bool has_clean_history;
  if (has_active_document) {
    const auto& command_stack = model.get<CommandStack>(document_entity);
    has_clean_history = command_stack.is_clean();
  }

  menu_set_enabled(MenuAction::Save, has_active_document && !has_clean_history);
  menu_set_enabled(MenuAction::SaveAs, has_active_document);

  menu_set_enabled(MenuAction::CloseDocument, has_active_document);

  menu_set_enabled(MenuAction::ReopenLastClosedFile, is_last_closed_file_valid());
  menu_set_enabled(MenuAction::ClearFileHistory,
                   has_active_document && !get_file_history().entries.empty());
}

}  // namespace

void init_menus()
{
  menu_translate(get_current_language());
}

void update_menus(const Model& model)
{
  const auto document_entity = sys::get_active_document(model);

  _update_file_menu(model, document_entity);
  _update_edit_menu(model, document_entity);
  _update_view_menu(model, document_entity);
  _update_map_menu(model, document_entity);
  _update_tileset_menu(model, document_entity);
}

void menu_translate(const Strings& strings)
{
  spdlog::debug("Translating menus...");

  // File
  _set_label(MenuAction::NewMap, strings.action.create_map);
  _set_label(MenuAction::OpenMap, strings.action.open_map);
  _set_label(MenuAction::Save, strings.action.save);
  _set_label(MenuAction::SaveAs, strings.action.save_as);
  _set_label(MenuAction::CloseDocument, strings.action.close_document);
  _set_label(MenuAction::Quit, strings.action.exit);

  _set_label(MenuAction::ReopenLastClosedFile, strings.action.reopen_last_closed_file);
  _set_label(MenuAction::ClearFileHistory, strings.action.clear_file_history);

  // Edit
  _set_label(MenuAction::Undo, strings.action.undo);
  _set_label(MenuAction::Redo, strings.action.redo);
  _set_label(MenuAction::StampTool, strings.action.stamp_tool);
  _set_label(MenuAction::EraserTool, strings.action.eraser_tool);
  _set_label(MenuAction::BucketTool, strings.action.bucket_tool);
  _set_label(MenuAction::ObjectSelectionTool, strings.action.object_selection_tool);
  _set_label(MenuAction::RectangleTool, strings.action.rectangle_tool);
  _set_label(MenuAction::EllipseTool, strings.action.ellipse_tool);
  _set_label(MenuAction::PointTool, strings.action.point_tool);
  _set_label(MenuAction::ComponentEditor, strings.action.component_editor);
  _set_label(MenuAction::OpenSettings, strings.action.show_settings);

  // View
  _set_label(MenuAction::CenterViewport, strings.action.center_viewport);
  _set_label(MenuAction::ToggleGrid, strings.action.toggle_grid);
  _set_label(MenuAction::IncreaseZoom, strings.action.increase_zoom);
  _set_label(MenuAction::DecreaseZoom, strings.action.decrease_zoom);
  _set_label(MenuAction::ResetZoom, strings.action.reset_zoom);
  _set_label(MenuAction::IncreaseFontSize, strings.action.increase_font_size);
  _set_label(MenuAction::DecreaseFontSize, strings.action.decrease_font_size);
  _set_label(MenuAction::ResetFontSize, strings.action.reset_font_size);
  _set_label(MenuAction::PanUp, strings.action.pan_up);
  _set_label(MenuAction::PanDown, strings.action.pan_down);
  _set_label(MenuAction::PanRight, strings.action.pan_right);
  _set_label(MenuAction::PanLeft, strings.action.pan_left);
  _set_label(MenuAction::HighlightLayer, strings.action.highlight_layer);
  _set_label(MenuAction::ToggleUi, strings.action.toggle_ui);

  // Map
  _set_label(MenuAction::InspectMap, strings.action.inspect_map);
  _set_label(MenuAction::AddTileset, strings.action.add_tileset);
  _set_label(MenuAction::AddRow, strings.action.add_row);
  _set_label(MenuAction::AddColumn, strings.action.add_column);
  _set_label(MenuAction::RemoveRow, strings.action.remove_row);
  _set_label(MenuAction::RemoveColumn, strings.action.remove_column);
  _set_label(MenuAction::FixInvalidTiles, strings.action.fix_invalid_tiles);
  _set_label(MenuAction::ResizeMap, strings.action.resize_map);
  _set_label(MenuAction::ExportGodotScene, strings.action.export_as_godot_scene);

  // Tileset
  _set_label(MenuAction::InspectTileset, strings.action.inspect_tileset);

  // Debug
  _set_label(MenuAction::DemoWindow, "Demo Window...");
  _set_label(MenuAction::StyleEditor, "Style Editor...");

  // Help
  _set_label(MenuAction::AboutTactile, strings.action.show_about);
  _set_label(MenuAction::AboutDearImGui, strings.action.about_dear_imgui);
  _set_label(MenuAction::ReportIssue, strings.action.report_issue);
  _set_label(MenuAction::Credits, strings.action.show_credits);
}

void menu_set_enabled(const MenuAction action, const bool enabled)
{
  gItems[action].enabled = enabled;
}

void menu_set_selected(const MenuAction action, const bool selected)
{
  gItems[action].selected = selected;
}

auto get_menu_item(const MenuAction action) -> const MenuItem&
{
  return lookup_in(gItems, action);
}

}  // namespace tactile
