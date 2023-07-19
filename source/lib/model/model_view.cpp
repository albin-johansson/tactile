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

#include "model_view.hpp"

#include "model/documents/command_system.hpp"
#include "model/documents/document_components.hpp"
#include "model/documents/document_system.hpp"
#include "model/i18n/language_system.hpp"
#include "model/layers/layer_components.hpp"
#include "model/maps/map_system.hpp"
#include "model/persistence/file_history_components.hpp"
#include "model/persistence/file_history_system.hpp"
#include "model/persistence/settings_system.hpp"
#include "model/services/locator.hpp"
#include "model/textures/texture_components.hpp"
#include "model/tools/tool_system.hpp"
#include "model/viewports/viewport_system.hpp"

namespace tactile {

ModelView::ModelView(const Registry& registry, Dispatcher& dispatcher)
    : mRegistry {registry},
      mDispatcher {dispatcher}
{
}

auto ModelView::get_file_history() const -> const Deque<String>&
{
  const auto& file_history = get_registry().get<FileHistory>();
  return file_history.entries;
}

auto ModelView::get_open_documents() const -> const Vector<Entity>&
{
  const auto& document_context = get_registry().get<DocumentContext>();
  return document_context.open_documents;
}

auto ModelView::get_tactile_icon_texture() const -> void*
{
  const auto& registry = get_registry();
  const auto& icons = registry.get<Icons>();
  const auto& icon_texture = registry.get<Texture>(icons.tactile_icon);
  return icon_texture.handle;
}

auto ModelView::get_settings() const -> const Settings&
{
  return Locator<SettingsSystem>::get().current_settings();
}

auto ModelView::get_language_strings() const -> const Strings&
{
  const auto& language_system = Locator<LanguageSystem>::get();
  return language_system.get_strings(get_settings().get_language());
}

auto ModelView::has_active_document() const -> bool
{
  return sys::has_active_document(get_registry());
}

auto ModelView::has_active_map_document() const -> bool
{
  return sys::is_map_document_active(get_registry());
}

auto ModelView::has_active_tileset_document() const -> bool
{
  return sys::is_tileset_document_active(get_registry());
}

auto ModelView::is_tile_layer_active() const -> bool
{
  const auto& registry = get_registry();

  if (const auto* map = sys::try_get_active_map(registry)) {
    return map->active_layer != kNullEntity && registry.has<TileLayer>(map->active_layer);
  }

  return false;
}

auto ModelView::is_document_active(const Entity document_entity) const -> bool
{
  return sys::get_active_document(get_registry()) == document_entity;
}

auto ModelView::is_document_open(const Entity document_entity) const -> bool
{
  return sys::is_document_open(get_registry(), document_entity);
}

auto ModelView::is_map_document(const Entity document_entity) const -> bool
{
  return get_registry().has<MapDocument>(document_entity);
}

auto ModelView::is_tileset_document(const Entity document_entity) const -> bool
{
  return get_registry().has<TilesetDocument>(document_entity);
}

auto ModelView::get_document_name(const Entity document_entity) const -> String
{
  return sys::get_document_name(get_registry(), document_entity);
}

auto ModelView::is_available(const MenuAction action) const -> bool
{
  switch (action) {
    case MenuAction::NewMap:
    case MenuAction::OpenMap:
    case MenuAction::Quit:
    case MenuAction::OpenSettings:
    case MenuAction::ToggleGrid:
    case MenuAction::OpenDemoWindow:
    case MenuAction::OpenStyleEditor:
    case MenuAction::ShowAbout:
    case MenuAction::ShowAboutImGui:
    case MenuAction::ShowCredits:
    case MenuAction::ReportIssue:
      return true;

    case MenuAction::SaveAs:
    case MenuAction::Close:
    case MenuAction::IncreaseZoom:
    case MenuAction::ResetZoom:
    case MenuAction::PanUp:
    case MenuAction::PanDown:
    case MenuAction::PanRight:
    case MenuAction::PanLeft:
    case MenuAction::CenterViewport:
    case MenuAction::ToggleUi:
      return has_active_document();

    case MenuAction::OpenComponentEditor:
    case MenuAction::HighlightLayer:
    case MenuAction::InspectMap:
    case MenuAction::CreateTileset:
    case MenuAction::AddRow:
    case MenuAction::AddColumn:
    case MenuAction::FixInvalidTiles:
    case MenuAction::ResizeMap:
    case MenuAction::ExportGodotScene:
      return has_active_map_document();

    case MenuAction::InspectTileset:
      return has_active_tileset_document();

    case MenuAction::Save:
      return Locator<CommandSystem>::get().is_save_possible(get_registry());

    case MenuAction::ReopenLastFile:
      return _can_reopen_last_closed_file();

    case MenuAction::ClearFileHistory:
      return _can_clear_file_history();

    case MenuAction::Undo:
      return Locator<CommandSystem>::get().is_undo_possible(get_registry());

    case MenuAction::Redo:
      return Locator<CommandSystem>::get().is_redo_possible(get_registry());

    case MenuAction::EnableStamp:
      return _is_tool_available(ToolType::Stamp);

    case MenuAction::EnableEraser:
      return _is_tool_available(ToolType::Eraser);

    case MenuAction::EnableBucket:
      return _is_tool_available(ToolType::Bucket);

    case MenuAction::EnableObjectSelector:
      return _is_tool_available(ToolType::ObjectSelection);

    case MenuAction::EnableRectangle:
      return _is_tool_available(ToolType::Rectangle);

    case MenuAction::EnableEllipse:
      return _is_tool_available(ToolType::Ellipse);

    case MenuAction::EnablePoint:
      return _is_tool_available(ToolType::Point);

    case MenuAction::DecreaseZoom:
      return sys::is_viewport_zoom_out_possible(get_registry());

    case MenuAction::IncreaseFontSize:
      return Locator<SettingsSystem>::get().can_increase_font_size();

    case MenuAction::DecreaseFontSize:
      return Locator<SettingsSystem>::get().can_decrease_font_size();

    case MenuAction::ResetFontSize:
      return Locator<SettingsSystem>::get().can_reset_font_size();

    case MenuAction::RemoveRow:
      return sys::can_tile_row_be_removed(get_registry());

    case MenuAction::RemoveColumn:
      return sys::can_tile_column_be_removed(get_registry());
  }

  return false;
}

auto ModelView::_can_clear_file_history() const -> bool
{
  const auto& file_history = get_registry().get<FileHistory>();
  return has_active_document() && !file_history.entries.empty();
}

auto ModelView::_can_reopen_last_closed_file() const -> bool
{
  const auto& file_history = get_registry().get<FileHistory>();
  return sys::is_last_closed_file_valid(file_history);
}

auto ModelView::_is_tool_available(const ToolType type) const -> bool
{
  const auto& tool_system = Locator<ToolSystem>::get();
  return tool_system.is_tool_available(get_registry(), type);
}

}  // namespace tactile
