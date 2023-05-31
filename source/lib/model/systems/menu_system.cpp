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

#include "menu_system.hpp"

#include <utility>  // move

#include <imgui_internal.h>

#include "common/type/ecs.hpp"
#include "common/type/maybe.hpp"
#include "common/type/string.hpp"
#include "common/util/lookup.hpp"
#include "components/file_history.hpp"
#include "model/event/menu_events.hpp"
#include "model/systems/document_system.hpp"
#include "model/systems/file_history_system.hpp"
#include "model/systems/font_system.hpp"
#include "model/systems/language_system.hpp"
#include "model/systems/map_system.hpp"
#include "model/systems/viewport_system.hpp"
#include "ui/shortcut/mappings.hpp"

namespace tactile::sys {
namespace {

[[nodiscard]] auto _to_shortcut_label(const ImGuiKeyChord chord) -> String
{
  String label;
  label.reserve(32);

  if (chord & ImGuiMod_Super) {
    label += TACTILE_PRIMARY_MOD;
  }

  if (chord & ImGuiMod_Ctrl) {
    if (!label.empty()) {
      label += "+";
    }

    label += TACTILE_PRIMARY_MOD;
  }

  if (chord & ImGuiMod_Shift) {
    if (!label.empty()) {
      label += "+";
    }

    label += "Shift";
  }

  if (chord & ImGuiMod_Alt) {
    if (!label.empty()) {
      label += "+";
    }

    label += TACTILE_SECONDARY_MOD;
  }

  if (!label.empty()) {
    label += "+";
  }

  const ImGuiKey key {chord & ~ImGuiMod_Mask_};
  label += ImGui::GetKeyName(key);

  return label;
}

auto _add_menu_item(Model& model,
                    const MenuAction action,
                    String label,
                    Maybe<ImGuiKeyChord> shortcut,
                    Maybe<MenuItemEnabledFn> enabled_fn) -> Entity
{
  const auto item_entity = model.create_entity();

  auto& item = model.add<MenuItem>(item_entity);
  item.action = action;
  item.label = std::move(label);
  item.enabled = true;
  item.checked = false;
  item.shortcut = shortcut;

  if (shortcut.has_value()) {
    item.shortcut_label = _to_shortcut_label(*shortcut);
  }

  if (enabled_fn.has_value()) {
    auto& item_callbacks = model.add<MenuItemCallbacks>(item_entity);
    item_callbacks.enabled_fn = std::move(*enabled_fn);
  }

  auto& menu_items = model.get<MenuItems>();
  menu_items.items[action] = item_entity;

  return item_entity;
}

auto _add_menu_item(Model& model,
                    const MenuAction action,
                    Maybe<MenuItemEnabledFn> enabled_fn = nothing) -> Entity
{
  const auto& strings = get_current_language_strings(model);
  return _add_menu_item(model,
                        action,
                        get_string(strings, action),
                        nothing,
                        std::move(enabled_fn));
}

auto _add_menu_item(Model& model,
                    const MenuAction action,
                    const ImGuiKeyChord shortcut,
                    Maybe<MenuItemEnabledFn> enabled_fn = nothing) -> Entity
{
  const auto& strings = get_current_language_strings(model);
  return _add_menu_item(model,
                        action,
                        get_string(strings, action),
                        shortcut,
                        std::move(enabled_fn));
}

void _init_file_menu(Model& model)
{
  _add_menu_item(model, MenuAction::NewMap, ImGuiMod_Super | ImGuiKey_N);
  _add_menu_item(model, MenuAction::OpenMap, ImGuiMod_Super | ImGuiKey_O);

  _add_menu_item(model, MenuAction::Save, ImGuiMod_Super | ImGuiKey_S, &is_save_possible);
  _add_menu_item(model,
                 MenuAction::SaveAs,
                 ImGuiMod_Super | ImGuiMod_Shift | ImGuiKey_S,
                 &has_active_document);

  _add_menu_item(model, MenuAction::Close, &has_active_document);

  _add_menu_item(model, MenuAction::Quit);

  _add_menu_item(model, MenuAction::ReopenLastFile, [](const Model& model) {
    const auto& file_history = model.get<FileHistory>();
    return sys::is_last_closed_file_valid(file_history);
  });

  _add_menu_item(model, MenuAction::ClearFileHistory, [](const Model& model) {
    const auto& file_history = model.get<FileHistory>();
    return sys::has_active_document(model) && !file_history.entries.empty();
  });
}

void _init_edit_menu(Model& model)
{
  _add_menu_item(model, MenuAction::Undo, ImGuiMod_Super | ImGuiKey_Z, &is_undo_possible);
  _add_menu_item(model, MenuAction::Redo, ImGuiMod_Super | ImGuiKey_Y, &is_redo_possible);

  // TODO tool validation functions
  _add_menu_item(model, MenuAction::EnableStamp, ImGuiKey_S);
  _add_menu_item(model, MenuAction::EnableBucket, ImGuiKey_B);
  _add_menu_item(model, MenuAction::EnableEraser, ImGuiKey_E);
  _add_menu_item(model, MenuAction::EnableObjectSelector, ImGuiKey_Q);
  _add_menu_item(model, MenuAction::EnableRectangle, ImGuiKey_R);
  _add_menu_item(model, MenuAction::EnableEllipse, ImGuiKey_T);
  _add_menu_item(model, MenuAction::EnablePoint, ImGuiKey_Y);

  _add_menu_item(model,
                 MenuAction::OpenComponentEditor,
                 ImGuiMod_Super | ImGuiMod_Shift | ImGuiKey_C,
                 &is_map_document_active);

  _add_menu_item(model, MenuAction::OpenSettings, ImGuiMod_Super | ImGuiKey_Comma);
}

void _init_view_menu(Model& model)
{
  _add_menu_item(model,
                 MenuAction::CenterViewport,
                 ImGuiMod_Shift | ImGuiKey_Space,
                 &has_active_document);
  _add_menu_item(model, MenuAction::ToggleGrid, ImGuiMod_Super | ImGuiKey_G);

  _add_menu_item(model,
                 MenuAction::IncreaseZoom,
                 ImGuiMod_Super | ImGuiKey_0,
                 &has_active_document);
  _add_menu_item(model,
                 MenuAction::DecreaseZoom,
                 ImGuiMod_Super | ImGuiKey_9,
                 &is_viewport_zoom_out_possible);
  _add_menu_item(model, MenuAction::ResetZoom, &has_active_document);

  _add_menu_item(model,
                 MenuAction::IncreaseFontSize,
                 ImGuiMod_Super | ImGuiMod_Shift | ImGuiKey_0,
                 &can_increase_font_size);
  _add_menu_item(model,
                 MenuAction::DecreaseFontSize,
                 ImGuiMod_Super | ImGuiMod_Shift | ImGuiKey_9,
                 &can_decrease_font_size);
  _add_menu_item(model, MenuAction::ResetFontSize, &can_reset_font_size);

  _add_menu_item(model,
                 MenuAction::PanUp,
                 ImGuiMod_Super | ImGuiMod_Shift | ImGuiKey_UpArrow,
                 &has_active_document);
  _add_menu_item(model,
                 MenuAction::PanDown,
                 ImGuiMod_Super | ImGuiMod_Shift | ImGuiKey_DownArrow,
                 &has_active_document);
  _add_menu_item(model,
                 MenuAction::PanLeft,
                 ImGuiMod_Super | ImGuiMod_Shift | ImGuiKey_LeftArrow,
                 &has_active_document);
  _add_menu_item(model,
                 MenuAction::PanRight,
                 ImGuiMod_Super | ImGuiMod_Shift | ImGuiKey_RightArrow,
                 &has_active_document);

  _add_menu_item(model, MenuAction::HighlightLayer, ImGuiKey_H, &is_map_document_active);
  _add_menu_item(model, MenuAction::ToggleUi, ImGuiKey_Tab, &has_active_document);
}

void _init_map_menu(Model& model)
{
  _add_menu_item(model, MenuAction::InspectMap, &is_map_document_active);
  _add_menu_item(model,
                 MenuAction::CreateTileset,
                 ImGuiMod_Super | ImGuiKey_T,
                 &is_map_document_active);

  _add_menu_item(model,
                 MenuAction::AddRow,
                 ImGuiMod_Alt | ImGuiKey_R,
                 &is_map_document_active);
  _add_menu_item(model,
                 MenuAction::AddColumn,
                 ImGuiMod_Alt | ImGuiKey_C,
                 &is_map_document_active);
  _add_menu_item(model,
                 MenuAction::RemoveRow,
                 ImGuiMod_Alt | ImGuiMod_Shift | ImGuiKey_R,
                 &can_tile_row_be_removed);
  _add_menu_item(model,
                 MenuAction::RemoveColumn,
                 ImGuiMod_Alt | ImGuiMod_Shift | ImGuiKey_C,
                 &can_tile_column_be_removed);

  _add_menu_item(model, MenuAction::FixInvalidTiles);
  _add_menu_item(model, MenuAction::ResizeMap);
  _add_menu_item(model, MenuAction::ExportGodotScene, &is_map_document_active);
}

void _init_tileset_menu(Model& model)
{
  _add_menu_item(model, MenuAction::InspectTileset, &is_tileset_document_active);
}

void _init_help_menu(Model& model)
{
  _add_menu_item(model, MenuAction::ShowAbout);
  _add_menu_item(model, MenuAction::ShowAboutImGui);
  _add_menu_item(model, MenuAction::ReportIssue);
  _add_menu_item(model, MenuAction::ShowCredits);
}

}  // namespace

void init_menus(Model& model)
{
  _init_file_menu(model);
  _init_edit_menu(model);
  _init_view_menu(model);
  _init_map_menu(model);
  _init_tileset_menu(model);
  _init_help_menu(model);
}

void update_menu_items(Model& model, Dispatcher& dispatcher)
{
  for (auto [item_entity, item]: model.each<MenuItem>()) {
    if (const auto* callbacks = model.try_get<MenuItemCallbacks>(item_entity)) {
      if (callbacks->enabled_fn) {
        item.enabled = callbacks->enabled_fn(model);
      }
    }

    if (item.enabled && item.shortcut.has_value() &&
        ImGui::Shortcut(*item.shortcut, 0, ImGuiInputFlags_RouteGlobalLow)) {
      dispatcher.enqueue<MenuActionEvent>(item.action);
    }
  }
}

void translate_menus(Model& model, const Strings& strings)
{
  auto& menu_items = model.get<MenuItems>();

  for (auto [menu_action, item_entity]: menu_items.items) {
    auto& menu_item = model.get<MenuItem>(item_entity);
    menu_item.label = get_string(strings, menu_action);
  }
}

void retranslate_menus(Model& model)
{
  translate_menus(model, get_current_language_strings(model));
}

auto get_menu_item(const Model& model, const MenuAction action) -> const MenuItem&
{
  const auto& menu_items = model.get<MenuItems>();
  const auto menu_item_entity = lookup_in(menu_items.items, action);
  return model.get<MenuItem>(menu_item_entity);
}

}  // namespace tactile::sys
