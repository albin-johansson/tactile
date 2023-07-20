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

#include "map_editor_viewport.hpp"

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
#include "model/services/language_service.hpp"
#include "model/services/service_locator.hpp"
#include "model/services/settings/settings.hpp"
#include "model/view/render_system.hpp"
#include "model/viewports/viewport_components.hpp"
#include "ui/conversions.hpp"
#include "ui/dock/editor/document_viewport_offset_handler.hpp"
#include "ui/dock/editor/map_viewport_overlay.hpp"
#include "ui/dock/editor/map_viewport_toolbar.hpp"
#include "ui/render/canvas.hpp"
#include "ui/render/canvas_renderer.hpp"
#include "ui/render/primitives.hpp"
#include "ui/widget/scoped.hpp"

namespace tactile {
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

void _draw_cursor_gizmos(ModelView& model,
                         const ui::CanvasInfo& canvas,
                         const ViewportMouseInfo& mouse)
{
  if (mouse.over_content && model.is_tile_layer_active()) {
    ui::draw_shadowed_rect(as_imvec2(mouse.clamped_pos),
                           canvas.graphical_tile_size,
                           Color {0, 0xFF, 0, 0xC8},
                           2.0f);
  }

  // ToolPreviewRenderer preview_renderer {registry, renderer, make_mouse_info(cursor)};
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
  if (ui::Window::contains_mouse()) {
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

void _push_viewport_context_menu(ModelView& model,
                                 const Strings& strings,
                                 const Entity map_document_entity,
                                 const MapDocument& map_document)
{
  constexpr auto flags =
      ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverExistingPopup;

  if (const auto popup = ui::Popup::for_item("##MapViewContextMenu", flags);
      popup.is_open()) {
    if (ImGui::MenuItem(strings.action.inspect_map.c_str())) {
      model.enqueue<InspectContextEvent>(map_document.map);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(strings.action.center_viewport.c_str())) {
      model.enqueue<CenterViewportEvent>(map_document_entity);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(strings.action.reset_zoom.c_str())) {
      model.enqueue<ResetViewportZoomEvent>(map_document_entity);
    }
  }
}

void _push_object_context_menu(ModelView& model,
                               const Strings& strings,
                               const Map& map,
                               MapEditorViewportState& state)
{
  if (const ui::Popup popup {kViewportObjectContextMenuId}; popup.is_open()) {
    const auto& registry = model.get_registry();
    const auto& object_layer = registry.get<ObjectLayer>(map.active_layer);
    const auto& object = registry.get<Object>(object_layer.active_object);

    if (ImGui::MenuItem(strings.action.inspect_object.c_str())) {
      model.enqueue<InspectContextEvent>(object_layer.active_object);
    }

    ImGui::Separator();
    if (ImGui::MenuItem(strings.action.toggle_object_visibility.c_str(),
                        nullptr,
                        object.visible)) {
      model.enqueue<SetObjectVisibleEvent>(object_layer.active_object, !object.visible);
    }

    // TODO implement the object actions

    ImGui::Separator();

    if (const ui::Disable disable;
        ImGui::MenuItem(strings.action.duplicate_object.c_str())) {
      model.enqueue<DuplicateObjectEvent>(object_layer.active_object);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(strings.action.remove_object.c_str())) {
      model.enqueue<RemoveObjectEvent>(object_layer.active_object);
    }
  }

  if (state.should_open_object_context_menu) {
    ImGui::OpenPopup(kViewportObjectContextMenuId,
                     ImGuiPopupFlags_AnyPopup | ImGuiPopupFlags_MouseButtonRight);
    state.should_open_object_context_menu = false;
  }
}

}  // namespace

void push_map_editor_viewport(ModelView& model,
                              MapEditorViewportState& state,
                              const Entity map_document_entity)
{
  auto& dispatcher = model.get_dispatcher();
  const auto& registry = model.get_registry();
  const auto& strings = model.get_language_strings();
  const auto& settings = model.get_settings();

  const auto& map_document = registry.get<MapDocument>(map_document_entity);
  const auto& viewport = registry.get<Viewport>(map_document_entity);
  const auto& map = registry.get<Map>(map_document.map);

  const auto canvas = ui::create_canvas_info(viewport, map.tile_size, map.extent);

  update_dynamic_viewport_info(map_document_entity, canvas, dispatcher);
  ui::update_document_viewport_offset(map_document_entity, canvas.size, dispatcher);

  clear_canvas(canvas, settings.get_viewport_bg_color());
  push_scissor(canvas);

  sys::render_map(registry, canvas, map);

  const auto mouse_info = get_viewport_mouse_info(canvas);
  _poll_mouse(mouse_info, dispatcher);

  if (ui::Window::contains_mouse()) {
    _draw_cursor_gizmos(model, canvas, mouse_info);
  }

  ui::pop_scissor();

  push_map_viewport_toolbar(model);
  push_map_viewport_overlay(model, map, mouse_info);

  _push_viewport_context_menu(model, strings, map_document_entity, map_document);
  _push_object_context_menu(model, strings, map, state);

  if (model.is_available(MenuAction::EnableStamp) &&
      ImGui::Shortcut(ImGuiKey_S, ImGuiKeyOwner_Any, ImGuiInputFlags_RouteFocused)) {
    model.enqueue<SelectToolEvent>(ToolType::Stamp);
  }

  if (model.is_available(MenuAction::EnableBucket) &&
      ImGui::Shortcut(ImGuiKey_B, ImGuiKeyOwner_Any, ImGuiInputFlags_RouteFocused)) {
    model.enqueue<SelectToolEvent>(ToolType::Bucket);
  }

  // TODO more tool shortcuts
}

}  // namespace tactile
