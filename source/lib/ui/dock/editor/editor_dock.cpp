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

#include "editor_dock.hpp"

#include <centurion/events/mouse_events.hpp>
#include <centurion/input/keyboard.hpp>
#include <imgui.h>
#include <imgui_internal.h>

#include "cmd/command_stack.hpp"
#include "core/formatters/fmt_string.hpp"
#include "model/documents/document_components.hpp"
#include "model/documents/document_system.hpp"
#include "model/events/file_events.hpp"
#include "model/events/map_events.hpp"
#include "model/events/tool_events.hpp"
#include "model/events/viewport_events.hpp"
#include "model/services/language_service.hpp"
#include "model/services/service_locator.hpp"
#include "model/textures/texture_components.hpp"
#include "model/tools/bucket_tool.hpp"
#include "model/tools/stamp_tool.hpp"
#include "model/viewports/viewport_components.hpp"
#include "model/viewports/viewport_ops.hpp"
#include "ui/conversions.hpp"
#include "ui/dock/editor/map_editor_viewport.hpp"
#include "ui/dock/editor/tileset_editor_viewport.hpp"
#include "ui/shortcut/mappings.hpp"
#include "ui/style/alignment.hpp"
#include "ui/style/icons.hpp"
#include "ui/widget/scoped.hpp"
#include "ui/widget/widgets.hpp"
#include "ui/widget/windows.hpp"

namespace tactile {
namespace {

void _push_document_tab(ModelView& model,
                        const Entity document_entity,
                        MapEditorViewportState& map_viewport_state,
                        TilesetEditorViewportState& tileset_viewport_state)
{
  const ui::Scope document_scope {document_entity};

  const auto is_document_active = model.is_document_active(document_entity);
  const char* document_icon = model.is_map_document(document_entity) ? TAC_ICON_MAP  //
                                                                     : TAC_ICON_TILESET;

  const auto document_name = model.get_document_name(document_entity);
  const FmtString<256> name_with_icon {"{} {}", document_icon, document_name};

  ImGuiTabItemFlags flags = 0;
  if (is_document_active) {
    flags |= ImGuiTabItemFlags_SetSelected;

    if (model.is_available(MenuAction::Save)) {
      flags |= ImGuiTabItemFlags_UnsavedDocument;
    }
  }

  bool opened = true;
  if (const ui::TabItem item {name_with_icon.data(), &opened, flags}; item.is_open()) {
    if (is_document_active) {
      if (model.is_map_document(document_entity)) {
        push_map_editor_viewport(model, map_viewport_state, document_entity);
      }
      else if (model.is_tileset_document(document_entity)) {
        push_tileset_editor_viewport(model, tileset_viewport_state, document_entity);
      }
    }
  }

  if (!opened) {
    model.enqueue<CloseDocumentEvent>(document_entity);
  }
  else if (ImGui::IsItemActivated()) {
    model.enqueue<SelectDocumentEvent>(document_entity);
  }
}

void _push_document_tab_bar(ModelView& model,
                            MapEditorViewportState& map_viewport_state,
                            TilesetEditorViewportState& tileset_viewport_state)
{
  if (const ui::TabBar bar {"##DocumentTabs", ImGuiTabBarFlags_Reorderable};
      bar.is_open()) {
    for (const auto document_entity: model.get_open_documents()) {
      _push_document_tab(model,
                         document_entity,
                         map_viewport_state,
                         tileset_viewport_state);
    }
  }
}

void _push_start_page(ModelView& model, const Strings& strings)
{
  ui::prepare_vertical_alignment_center(4);

  ImGui::SetCursorPos(ImGui::GetCursorPos() - ImVec2 {0, 64});

  ui::center_next_item_horizontally(128);
  ImGui::Image(model.get_tactile_icon_texture(), {128, 128});

  ImGui::Spacing();
  ImGui::Spacing();

  if (ui::push_centered_button(strings.action.create_map.c_str())) {
    model.enqueue<ShowNewMapDialogEvent>();
  }

  ImGui::Spacing();
  if (ui::push_centered_button(strings.action.open_map.c_str())) {
    model.enqueue<ShowOpenMapDialogEvent>();
  }
}

}  // namespace

void push_editor_dock_widget(ModelView& model, EditorDockState& state)
{
  const auto& strings = model.get_language_strings();

  ui::StyleVar padding {ImGuiStyleVar_WindowPadding, {4, 4}};
  ui::remove_tab_bar_from_next_window();

  const ui::Window window {"Viewport",
                           ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse};
  state.is_focused = window.has_focus();
  state.is_hovered = window.is_hovered();

  if (window.is_open()) {
    padding.pop();

    if (model.has_active_document()) {
      _push_document_tab_bar(model, state.central_map_viewport, state.tileset_viewport);

      if (model.has_active_map_document()) {
        if (window.mouse_entered()) {
          model.enqueue<ViewportMouseEnteredEvent>();
        }

        if (window.mouse_exited()) {
          model.enqueue<ViewportMouseExitedEvent>();
        }
      }
    }
    else {
      _push_start_page(model, strings);
    }
  }
}

void on_mouse_wheel_event_in_central_viewport(const Entity viewport_entity,
                                              const Viewport& viewport,
                                              Dispatcher& dispatcher,
                                              const cen::mouse_wheel_event& event)
{
  if (cen::is_active(kPrimaryModifier)) {
    const auto y = event.precise_y();
    if (y > 0) {
      dispatcher.enqueue<IncreaseViewportZoomEvent>(viewport_entity,
                                                    as_float2(ImGui::GetIO().MousePos));
    }
    else if (y < 0 && sys::can_zoom_out_in(viewport)) {
      dispatcher.enqueue<DecreaseViewportZoomEvent>(viewport_entity,
                                                    as_float2(ImGui::GetIO().MousePos));
    }
  }
  else {
    const Float2 precise {event.precise_x(), event.precise_y()};

    const float scaling = 4.0f;
    auto delta = precise * (viewport.tile_size / scaling);
    delta.x = -delta.x;

    dispatcher.enqueue<OffsetViewportEvent>(viewport_entity, delta);
  }
}

}  // namespace tactile