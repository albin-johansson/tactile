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

#include "tileset_dock.hpp"

#include <centurion/events/mouse_events.hpp>
#include <imgui.h>

#include "model/contexts/context_components.hpp"
#include "model/documents/document_components.hpp"
#include "model/documents/document_system.hpp"
#include "model/events/file_events.hpp"
#include "model/events/property_events.hpp"
#include "model/events/setting_events.hpp"
#include "model/events/tileset_events.hpp"
#include "model/events/viewport_events.hpp"
#include "model/maps/map_components.hpp"
#include "model/services/language_service.hpp"
#include "model/services/service_locator.hpp"
#include "model/tilesets/tileset_components.hpp"
#include "model/viewports/viewport_components.hpp"
#include "tactile/core/common/assert.hpp"
#include "ui/dock/tileset/tileset_view.hpp"
#include "ui/style/alignment.hpp"
#include "ui/style/icons.hpp"
#include "ui/widget/scoped.hpp"
#include "ui/widget/widgets.hpp"

namespace tactile {
namespace {

void _push_tab_context_menu(ModelView& model,
                            const Strings& strings,
                            const Entity tileset_document_entity,
                            const Entity attached_tileset_entity)
{
  const auto& registry = model.get_registry();

  if (auto popup = ui::Popup::for_item("##TilesetTabContext"); popup.is_open()) {
    if (ImGui::MenuItem(strings.action.create_tileset.c_str())) {
      model.enqueue<ShowNewTilesetDialogEvent>();
    }

    ImGui::Separator();

    if (ImGui::MenuItem(strings.action.inspect_tileset.c_str())) {
      model.enqueue<InspectContextEvent>(attached_tileset_entity);
    }

    ImGui::Separator();

    if (const ui::Disable disable_if {model.is_document_open(tileset_document_entity)};
        ImGui::MenuItem(strings.action.open_tileset.c_str())) {
      model.enqueue<OpenDocumentEvent>(tileset_document_entity);
      model.enqueue<SelectDocumentEvent>(tileset_document_entity);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(strings.action.remove_tileset.c_str())) {
      model.enqueue<DetachTilesetEvent>(sys::get_active_map(registry),
                                        attached_tileset_entity);
    }
  }
}

void _push_tileset_tab_bar(ModelView& model)
{
  TACTILE_ASSERT(model.has_active_map_document());

  const auto& registry = model.get_registry();
  const auto& strings = model.get_language_strings();

  const ImGuiTabBarFlags tab_bar_flags =
      ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_NoCloseWithMiddleMouseButton;

  if (const ui::TabBar bar {"##TilesetTabBar", tab_bar_flags}; bar.is_open()) {
    if (ImGui::TabItemButton(TAC_ICON_ADD "##AddTilesetButton",
                             ImGuiTabItemFlags_Trailing)) {
      model.enqueue<ShowNewTilesetDialogEvent>();
    }

    const auto map_document_entity = sys::get_active_document(registry);

    const auto& map_document = registry.get<MapDocument>(map_document_entity);
    const auto& map = registry.get<Map>(map_document.map);

    for (const auto attached_tileset_entity: map.attached_tilesets) {
      const ui::Scope scope {attached_tileset_entity};
      const auto& attached_tileset =
          registry.get<AttachedTileset>(attached_tileset_entity);

      const auto tileset_entity = attached_tileset.tileset;
      const auto tileset_document_entity =
          sys::get_associated_tileset_document(registry, tileset_entity);
      TACTILE_ASSERT(tileset_document_entity != kNullEntity);

      const auto tileset_name = sys::get_document_name(registry, tileset_document_entity);
      const auto is_tileset_active = map.active_tileset == attached_tileset_entity;

      if (const ui::TabItem item {tileset_name.c_str(),
                                  nullptr,
                                  is_tileset_active ? ImGuiTabItemFlags_SetSelected : 0};
          item.is_open()) {
        render_attached_tileset_tab_contents(model, attached_tileset_entity);
      }

      if (ImGui::IsItemClicked(ImGuiMouseButton_Left) ||
          ImGui::IsItemClicked(ImGuiMouseButton_Right) || ImGui::IsItemActivated()) {
        model.enqueue<SelectTilesetEvent>(attached_tileset_entity);
      }
      else {
        _push_tab_context_menu(model,
                               strings,
                               tileset_document_entity,
                               attached_tileset_entity);
      }
    }
  }
}

}  // namespace

void push_tileset_dock_widget(ModelView& model, TilesetDockState& state)
{
  const auto& registry = model.get_registry();
  const auto& strings = model.get_language_strings();
  const auto& settings = model.get_settings();

  if (!settings.test_flag(SETTINGS_SHOW_TILESET_DOCK_BIT)) {
    return;
  }

  bool show_tileset_dock = true;
  const ui::Window dock {strings.window.tileset_dock.c_str(),
                         ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar,
                         &show_tileset_dock};

  if (show_tileset_dock != settings.test_flag(SETTINGS_SHOW_TILESET_DOCK_BIT)) {
    model.enqueue<SetFlagSettingEvent>(SETTINGS_SHOW_TILESET_DOCK_BIT, show_tileset_dock);
  }

  // We intentionally do not use the window is_hovered function here
  state.has_focus = dock.has_focus(ImGuiFocusedFlags_RootAndChildWindows);
  state.has_hover = ImGui::IsWindowHovered(ImGuiFocusedFlags_RootAndChildWindows);

  if (dock.is_open()) {
    const auto document_entity = sys::get_active_document(registry);
    const auto& map_document = registry.get<MapDocument>(document_entity);
    const auto& map = registry.get<Map>(map_document.map);

    if (map.attached_tilesets.empty()) {
      ui::prepare_vertical_alignment_center(2);
      ui::push_centered_label(strings.misc.map_has_no_tilesets.c_str());

      ImGui::Spacing();

      if (ui::push_centered_button(strings.action.add_tileset.c_str())) {
        model.enqueue<ShowNewTilesetDialogEvent>();
      }
    }
    else {
      _push_tileset_tab_bar(model);
    }
  }
}

auto is_tileset_dock_enabled(const Registry& registry) -> bool
{
  return sys::is_map_document_active(registry);
}

void on_mouse_wheel_event_in_tileset_dock(const Registry& registry,
                                          const Entity attached_tileset_entity,
                                          const cen::mouse_wheel_event& event,
                                          Dispatcher& dispatcher)
{
  const auto& viewport = registry.get<Viewport>(attached_tileset_entity);

  const Float2 scaling {4, 4};
  const Float2 precise {event.precise_x(), event.precise_y()};

  auto delta = precise * (viewport.tile_size / scaling);
  delta.x = -delta.x;

  dispatcher.enqueue<OffsetViewportEvent>(attached_tileset_entity, delta);
}

}  // namespace tactile
