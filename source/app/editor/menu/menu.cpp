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

#include "menu.hpp"

#include <utility>  // move

#include <fmt/format.h>

#include "app/app_context.hpp"
#include "core/predef.hpp"
#include "core/tile/tileset_bundle.hpp"
#include "core/type/chrono.hpp"
#include "core/type/hash_map.hpp"
#include "core/type/string.hpp"
#include "core/util/assoc.hpp"
#include "core/vocabulary.hpp"
#include "core/debug/profile.hpp"
#include "io/proto/history.hpp"
#include "io/proto/preferences.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/document/map_document.hpp"
#include "model/document/tileset_document.hpp"
#include "model/model.hpp"
#include "ui/fonts.hpp"

namespace tactile {
namespace {

inline HashMap<MenuAction, MenuItem> items;

void set_label(const MenuAction action, String label)
{
  items[action].text = std::move(label);
}

void update_tileset_menu(const TilesetDocument* tileset_document)
{
  menu_set_enabled(MenuAction::InspectTileset, tileset_document != nullptr);
}

void update_map_menu(const MapDocument* map_document)
{
  const auto is_map_active = map_document != nullptr;

  menu_set_enabled(MenuAction::InspectMap, is_map_active);
  menu_set_enabled(MenuAction::AddTileset, is_map_active);

  menu_set_enabled(MenuAction::AddRow, is_map_active);
  menu_set_enabled(MenuAction::AddColumn, is_map_active);
  menu_set_enabled(MenuAction::RemoveRow,
                   is_map_active && map_document->get_map().row_count() > 1);
  menu_set_enabled(MenuAction::RemoveColumn,
                   is_map_active && map_document->get_map().column_count() > 1);

  menu_set_enabled(MenuAction::ExportGodotScene, is_map_active);
}

void update_view_menu(const Document* document, const MapDocument* map_document)
{
  const auto& prefs = io::get_preferences();
  const auto is_document_open = document != nullptr;
  const auto is_map_active = map_document != nullptr;

  menu_set_enabled(MenuAction::CenterViewport, is_document_open);

  menu_set_enabled(MenuAction::ResetZoom, is_document_open);
  menu_set_enabled(MenuAction::IncreaseZoom, is_document_open);
  menu_set_enabled(MenuAction::DecreaseZoom,
                   document && document->get_viewport().can_zoom_out());

  menu_set_enabled(MenuAction::IncreaseFontSize, ui::can_increase_font_size());
  menu_set_enabled(MenuAction::DecreaseFontSize, ui::can_decrease_font_size());
  menu_set_enabled(MenuAction::ResetFontSize, !prefs.use_default_font);

  menu_set_enabled(MenuAction::PanUp, is_document_open);
  menu_set_enabled(MenuAction::PanDown, is_document_open);
  menu_set_enabled(MenuAction::PanRight, is_document_open);
  menu_set_enabled(MenuAction::PanLeft, is_document_open);

  menu_set_enabled(MenuAction::HighlightLayer, is_map_active);
  menu_set_enabled(MenuAction::ToggleUi, is_document_open);
}

void update_tool_menu_items(const DocumentModel& model, const MapDocument* map_document)
{
  auto update_tool = [&](const MenuAction action, const ToolType tool) {
    if (map_document) {
      const auto& tools = map_document->get_tools();
      menu_set_enabled(action, tools.is_available(model, tool));
      menu_set_selected(action, tools.is_enabled(tool));
    }
    else {
      menu_set_enabled(action, false);
    }
  };

  update_tool(MenuAction::StampTool, ToolType::Stamp);
  update_tool(MenuAction::EraserTool, ToolType::Eraser);
  update_tool(MenuAction::BucketTool, ToolType::Bucket);
  update_tool(MenuAction::ObjectSelectionTool, ToolType::ObjectSelection);
  update_tool(MenuAction::RectangleTool, ToolType::Rectangle);
  update_tool(MenuAction::EllipseTool, ToolType::Ellipse);
  update_tool(MenuAction::PointTool, ToolType::Point);
}

void update_edit_menu(const Document* document, const MapDocument* map_document)
{
  const auto& lang = get_current_language();

  const auto can_undo = document && document->get_history().can_undo();
  const auto can_redo = document && document->get_history().can_redo();

  auto& undo_item = lookup_in(items, MenuAction::Undo);
  undo_item.enabled = can_undo;
  undo_item.text = fmt::format("{} {}",
                               lang.action.undo,
                               can_undo ? document->get_history().get_undo_text() : "");

  auto& redo_item = lookup_in(items, MenuAction::Redo);
  redo_item.enabled = can_redo;
  redo_item.text = fmt::format("{} {}",
                               lang.action.redo,
                               can_redo ? document->get_history().get_redo_text() : "");

  update_tool_menu_items(get_model(), map_document);

  menu_set_enabled(MenuAction::ComponentEditor, map_document != nullptr);
}

void update_file_menu(const Document* document)
{
  const auto is_document_open = document != nullptr;

  menu_set_enabled(MenuAction::Save, document && !document->get_history().is_clean());
  menu_set_enabled(MenuAction::SaveAs, is_document_open);

  menu_set_enabled(MenuAction::CloseDocument, is_document_open);

  menu_set_enabled(MenuAction::ReopenLastClosedFile, io::is_last_closed_file_valid());
  menu_set_enabled(MenuAction::ClearFileHistory, document && !io::file_history().empty());
}

}  // namespace

void init_menus()
{
  menu_translate(get_current_language());
}

void update_menus()
{
  const auto& model = get_model();
  const auto* document = model.active_document();
  const auto* map_document = dynamic_cast<const MapDocument*>(document);
  const auto* tileset_document = dynamic_cast<const TilesetDocument*>(document);
  update_file_menu(document);
  update_edit_menu(document, map_document);
  update_view_menu(document, map_document);
  update_map_menu(map_document);
  update_tileset_menu(tileset_document);
}

void menu_translate(const Strings& strings)
{
  spdlog::debug("Translating menus...");

  // File
  set_label(MenuAction::NewMap, strings.action.create_map);
  set_label(MenuAction::OpenMap, strings.action.open_map);
  set_label(MenuAction::Save, strings.action.save);
  set_label(MenuAction::SaveAs, strings.action.save_as);
  set_label(MenuAction::CloseDocument, strings.action.close_document);
  set_label(MenuAction::Quit, strings.action.exit);

  set_label(MenuAction::ReopenLastClosedFile, strings.action.reopen_last_closed_file);
  set_label(MenuAction::ClearFileHistory, strings.action.clear_file_history);

  // Edit
  set_label(MenuAction::Undo, strings.action.undo);
  set_label(MenuAction::Redo, strings.action.redo);
  set_label(MenuAction::StampTool, strings.action.stamp_tool);
  set_label(MenuAction::EraserTool, strings.action.eraser_tool);
  set_label(MenuAction::BucketTool, strings.action.bucket_tool);
  set_label(MenuAction::ObjectSelectionTool, strings.action.object_selection_tool);
  set_label(MenuAction::RectangleTool, strings.action.rectangle_tool);
  set_label(MenuAction::EllipseTool, strings.action.ellipse_tool);
  set_label(MenuAction::PointTool, strings.action.point_tool);
  set_label(MenuAction::ComponentEditor, strings.action.component_editor);
  set_label(MenuAction::OpenSettings, strings.action.show_settings);

  // View
  set_label(MenuAction::CenterViewport, strings.action.center_viewport);
  set_label(MenuAction::ToggleGrid, strings.action.toggle_grid);
  set_label(MenuAction::IncreaseZoom, strings.action.increase_zoom);
  set_label(MenuAction::DecreaseZoom, strings.action.decrease_zoom);
  set_label(MenuAction::ResetZoom, strings.action.reset_zoom);
  set_label(MenuAction::IncreaseFontSize, strings.action.increase_font_size);
  set_label(MenuAction::DecreaseFontSize, strings.action.decrease_font_size);
  set_label(MenuAction::ResetFontSize, strings.action.reset_font_size);
  set_label(MenuAction::PanUp, strings.action.pan_up);
  set_label(MenuAction::PanDown, strings.action.pan_down);
  set_label(MenuAction::PanRight, strings.action.pan_right);
  set_label(MenuAction::PanLeft, strings.action.pan_left);
  set_label(MenuAction::HighlightLayer, strings.action.highlight_layer);
  set_label(MenuAction::ToggleUi, strings.action.toggle_ui);

  // Map
  set_label(MenuAction::InspectMap, strings.action.inspect_map);
  set_label(MenuAction::AddTileset, strings.action.add_tileset);
  set_label(MenuAction::AddRow, strings.action.add_row);
  set_label(MenuAction::AddColumn, strings.action.add_column);
  set_label(MenuAction::RemoveRow, strings.action.remove_row);
  set_label(MenuAction::RemoveColumn, strings.action.remove_column);
  set_label(MenuAction::FixInvalidTiles, strings.action.fix_invalid_tiles);
  set_label(MenuAction::ResizeMap, strings.action.resize_map);
  set_label(MenuAction::ExportGodotScene, strings.action.export_as_godot_scene);

  // Tileset
  set_label(MenuAction::InspectTileset, strings.action.inspect_tileset);

  // Debug
  set_label(MenuAction::DemoWindow, "Demo Window...");
  set_label(MenuAction::StyleEditor, "Style Editor...");

  // Help
  set_label(MenuAction::AboutTactile, strings.action.show_about);
  set_label(MenuAction::AboutDearImGui, strings.action.about_dear_imgui);
  set_label(MenuAction::ReportIssue, strings.action.report_issue);
  set_label(MenuAction::Credits, strings.action.show_credits);
}

void menu_set_enabled(const MenuAction action, const bool enabled)
{
  items[action].enabled = enabled;
}

void menu_set_selected(const MenuAction action, const bool selected)
{
  items[action].selected = selected;
}

auto get_menu_item(const MenuAction action) -> const MenuItem&
{
  return lookup_in(items, action);
}

}  // namespace tactile
