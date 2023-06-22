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

#include "central_map_viewport.hpp"

#include <concepts>  // predicate

#include <imgui.h>

#include "common/color.hpp"
#include "model/documents/document_components.hpp"
#include "model/events/object_events.hpp"
#include "model/events/property_events.hpp"
#include "model/events/tool_events.hpp"
#include "model/events/viewport_events.hpp"
#include "model/layers/layer_components.hpp"
#include "model/maps/map_components.hpp"
#include "model/objects/object_components.hpp"
#include "model/persistence/settings.hpp"
#include "model/systems/language_system.hpp"
#include "model/systems/render_system.hpp"
#include "model/viewports/viewport_components.hpp"
#include "ui/conversions.hpp"
#include "ui/dock/editor/document_viewport_offset_handler.hpp"
#include "ui/dock/editor/map_viewport_overlay.hpp"
#include "ui/dock/editor/map_viewport_toolbar.hpp"
#include "ui/render/canvas.hpp"
#include "ui/render/canvas_renderer.hpp"
#include "ui/render/primitives.hpp"
#include "ui/widget/scoped.hpp"

namespace tactile::ui {
namespace {

constexpr auto kViewportObjectContextMenuId = "##MapViewObjectContextMenu";

template <typename Event, std::predicate<ImGuiMouseButton> T>
void _check_for(ViewportMouseInfo mouse_info, Dispatcher& dispatcher, T&& predicate)
{
  const auto lmb = predicate(ImGuiMouseButton_Left);
  const auto mmb = predicate(ImGuiMouseButton_Middle);
  const auto rmb = predicate(ImGuiMouseButton_Right);

  if (lmb || mmb || rmb) {
    if (lmb) {
      mouse_info.button = MouseButton::Left;
    }

    if (mmb) {
      mouse_info.button = MouseButton::Middle;
    }

    if (rmb) {
      mouse_info.button = MouseButton::Right;
    }

    dispatcher.enqueue<Event>(mouse_info);
  }
}

void _draw_cursor_gizmos(const Model& model,
                         const CanvasInfo& canvas,
                         const Map& map,
                         const ViewportMouseInfo& mouse)
{
  const auto is_tile_layer_active =
      map.active_layer != kNullEntity && model.has<TileLayer>(map.active_layer);

  if (mouse.in_viewport && is_tile_layer_active) {
    draw_shadowed_rect(as_imvec2(mouse.clamped_pos),
                       canvas.graphical_tile_size,
                       Color {0, 0xFF, 0, 0xC8},
                       2.0f);
  }

  // ToolPreviewRenderer preview_renderer {model, renderer, make_mouse_info(cursor)};
  // TODO const auto& tools = document.get_tools();
  // TODO tools.accept(preview_renderer);
}

void _poll_mouse(const ViewportMouseInfo& mouse_info, Dispatcher& dispatcher)
{
  if (ImGui::IsPopupOpen(kViewportObjectContextMenuId, ImGuiPopupFlags_AnyPopup)) {
    return;
  }

  if (is_map_toolbar_hovered()) {
    return;
  }

  // FIXME crash: we need to track origin dock for these mouse events, otherwise we
  // might end up emitting dragged events without an initial pressed event, etc.
  if (Window::contains_mouse()) {
    using PressedEvent = ViewportMousePressedEvent;
    using DraggedEvent = ViewportMouseDraggedEvent;
    using ReleasedEvent = ViewportMouseReleasedEvent;

    _check_for<PressedEvent>(mouse_info, dispatcher, [](const ImGuiMouseButton button) {
      return ImGui::IsMouseClicked(button);
    });

    _check_for<DraggedEvent>(mouse_info, dispatcher, [](const ImGuiMouseButton button) {
      const auto& io = ImGui::GetIO();
      return ImGui::IsMouseDragging(button) && as_float2(io.MouseDelta) != Float2 {0, 0};
    });

    _check_for<ReleasedEvent>(mouse_info, dispatcher, [](const ImGuiMouseButton button) {
      return ImGui::IsMouseReleased(button);
    });
  }
}

void _push_viewport_context_menu(const Strings& strings,
                                 const Entity map_document_entity,
                                 const MapDocument& map_document,
                                 Dispatcher& dispatcher)
{
  constexpr auto flags =
      ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverExistingPopup;

  if (const auto popup = Popup::for_item("##MapViewContextMenu", flags);
      popup.is_open()) {
    if (ImGui::MenuItem(strings.action.inspect_map.c_str())) {
      dispatcher.enqueue<InspectContextEvent>(map_document.map);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(strings.action.center_viewport.c_str())) {
      dispatcher.enqueue<CenterViewportEvent>(map_document_entity);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(strings.action.reset_zoom.c_str())) {
      dispatcher.enqueue<ResetViewportZoomEvent>(map_document_entity);
    }
  }
}

void _push_object_context_menu(const Model& model,
                               const Strings& strings,
                               const Map& map,
                               CentralMapViewportState& state,
                               Dispatcher& dispatcher)
{
  if (const Popup popup {kViewportObjectContextMenuId}; popup.is_open()) {
    const auto& object_layer = model.get<ObjectLayer>(map.active_layer);
    const auto& object = model.get<Object>(object_layer.active_object);

    if (ImGui::MenuItem(strings.action.inspect_object.c_str())) {
      dispatcher.enqueue<InspectContextEvent>(object_layer.active_object);
    }

    ImGui::Separator();
    if (ImGui::MenuItem(strings.action.toggle_object_visibility.c_str(),
                        nullptr,
                        object.visible)) {
      dispatcher.enqueue<SetObjectVisibleEvent>(object_layer.active_object,
                                                !object.visible);
    }

    // TODO implement the object actions

    ImGui::Separator();

    if (const Disable disable; ImGui::MenuItem(strings.action.duplicate_object.c_str())) {
      dispatcher.enqueue<DuplicateObjectEvent>(object_layer.active_object);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(strings.action.remove_object.c_str())) {
      dispatcher.enqueue<RemoveObjectEvent>(object_layer.active_object);
    }
  }

  if (state.should_open_object_context_menu) {
    ImGui::OpenPopup(kViewportObjectContextMenuId,
                     ImGuiPopupFlags_AnyPopup | ImGuiPopupFlags_MouseButtonRight);
    state.should_open_object_context_menu = false;
  }
}

}  // namespace

void push_central_map_viewport(const Model& model,
                               CentralMapViewportState& state,
                               const Entity map_document_entity,
                               Dispatcher& dispatcher)
{
  const auto& strings = sys::get_current_language_strings(model);
  const auto& settings = model.get<Settings>();

  const auto& map_document = model.get<MapDocument>(map_document_entity);
  const auto& viewport = model.get<Viewport>(map_document_entity);
  const auto& map = model.get<Map>(map_document.map);

  const auto canvas = create_canvas_info(viewport, map.tile_size, map.extent);

  update_dynamic_viewport_info(map_document_entity, canvas, dispatcher);
  update_document_viewport_offset(map_document_entity, canvas.size, dispatcher);

  clear_canvas(canvas, settings.get_viewport_bg_color());
  push_scissor(canvas);

  sys::render_map(model, canvas, map);

  const auto mouse_info = get_viewport_mouse_info(canvas);
  _poll_mouse(mouse_info, dispatcher);

  if (Window::contains_mouse()) {
    _draw_cursor_gizmos(model, canvas, map, mouse_info);
  }

  pop_scissor();

  show_map_viewport_toolbar(model, dispatcher);
  show_map_viewport_overlay(model, map, mouse_info, dispatcher);

  _push_viewport_context_menu(strings, map_document_entity, map_document, dispatcher);
  _push_object_context_menu(model, strings, map, state, dispatcher);
}

}  // namespace tactile::ui
