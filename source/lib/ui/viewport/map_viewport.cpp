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

#include "map_viewport.hpp"

#include <cmath>  // round

#include <imgui.h>
#include <imgui_internal.h>

#include "common/color.hpp"
#include "components/document.hpp"
#include "components/map.hpp"
#include "core/layer.hpp"
#include "core/object.hpp"
#include "core/viewport.hpp"
#include "model/context.hpp"
#include "model/event/object_events.hpp"
#include "model/event/property_events.hpp"
#include "model/event/tool_events.hpp"
#include "model/event/viewport_events.hpp"
#include "model/settings.hpp"
#include "model/systems/render_system.hpp"
#include "systems/language_system.hpp"
#include "ui/conversions.hpp"
#include "ui/render/canvas.hpp"
#include "ui/render/canvas_renderer.hpp"
#include "ui/render/primitives.hpp"
#include "ui/viewport/document_viewport_offset_handler.hpp"
#include "ui/viewport/map_viewport_overlay.hpp"
#include "ui/viewport/map_viewport_toolbar.hpp"
#include "ui/viewport/viewport_cursor_info.hpp"
#include "ui/widget/scoped.hpp"

namespace tactile::ui {
namespace {

inline constexpr auto kViewportObjectContextMenuId = "##MapViewObjectContextMenu";

struct MapViewportState final {
  bool will_be_centered {};
  bool will_open_object_context_menu {};
};

inline constinit MapViewportState gViewportState;

/// Creates a mouse info struct, but does not set the button member.
[[nodiscard]] auto _make_mouse_info(const ViewportCursorInfo& cursor) -> MouseInfo
{
  MouseInfo info;

  info.pos.x = cursor.raw_position.x;
  info.pos.y = cursor.raw_position.y;
  info.position_in_viewport = cursor.map_position;
  info.is_within_contents = cursor.is_within_map;

  return info;
}

template <typename Event, typename T>
void _check_for(const ViewportCursorInfo& cursor, Dispatcher& dispatcher, T&& query)
{
  const auto left = query(ImGuiMouseButton_Left);
  const auto mid = query(ImGuiMouseButton_Middle);
  const auto right = query(ImGuiMouseButton_Right);
  if (left || mid || right) {
    auto info = _make_mouse_info(cursor);

    if (left) {
      info.button = cen::mouse_button::left;
    }
    else if (mid) {
      info.button = cen::mouse_button::middle;
    }
    else /* if (right) */ {
      info.button = cen::mouse_button::right;
    }

    dispatcher.enqueue<Event>(info);
  }
}

void _center_viewport(const Viewport& viewport,
                      const CanvasInfo& canvas_info,
                      Dispatcher& dispatcher)
{
  const auto width = canvas_info.col_count * viewport.tile_size.x;
  const auto height = canvas_info.row_count * viewport.tile_size.y;

  const auto dx = std::round(((canvas_info.size.x - width) / 2.0f) - viewport.offset.x);
  const auto dy = std::round(((canvas_info.size.y - height) / 2.0f) - viewport.offset.y);
  const Float2 delta {dx, dy};

  dispatcher.enqueue<OffsetDocumentViewportEvent>(delta);
}

void _draw_cursor_gizmos(const Model& model,
                         const CanvasInfo& canvas,
                         const Map& map,
                         const ViewportCursorInfo& cursor)
{
  const auto is_tile_layer_active =
      map.active_layer != kNullEntity && model.has<TileLayer>(map.active_layer);

  if (cursor.is_within_map && is_tile_layer_active) {
    draw_shadowed_rect(cursor.clamped_position,
                       canvas.graphical_tile_size,
                       Color {0, 0xFF, 0, 200},
                       2.0f);
  }

  // ToolPreviewRenderer preview_renderer {model, renderer, make_mouse_info(cursor)};
  // TODO const auto& tools = document.get_tools();
  // TODO tools.accept(preview_renderer);
}

void _poll_mouse(Dispatcher& dispatcher, const ViewportCursorInfo& cursor)
{
  if (ImGui::IsPopupOpen(kViewportObjectContextMenuId, ImGuiPopupFlags_AnyPopup)) {
    return;
  }

  if (is_map_toolbar_hovered()) {
    return;
  }

  // FIXME crash: we need to track origin dock for these mouse events, otherwise we
  // might// end up emitting dragged events without an initial pressed event, etc.
  if (Window::contains_mouse()) {
    _check_for<ViewportMousePressedEvent>(
        cursor,
        dispatcher,
        [](ImGuiMouseButton button) { return ImGui::IsMouseClicked(button); });

    _check_for<
        ViewportMouseDraggedEvent>(cursor, dispatcher, [](ImGuiMouseButton button) {
      const auto& io = ImGui::GetIO();
      return ImGui::IsMouseDragging(button) && as_float2(io.MouseDelta) != Float2 {0, 0};
    });

    _check_for<ViewportMouseReleasedEvent>(
        cursor,
        dispatcher,
        [](ImGuiMouseButton button) { return ImGui::IsMouseReleased(button); });
  }
}

void _push_viewport_context_menu(const Strings& strings,
                                 const Entity map_entity,
                                 Dispatcher& dispatcher)
{
  constexpr auto flags =
      ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverExistingPopup;

  if (const auto popup = Popup::for_item("##MapViewContextMenu", flags);
      popup.is_open()) {
    if (ImGui::MenuItem(strings.action.inspect_map.c_str())) {
      dispatcher.enqueue<InspectContextEvent>(map_entity);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(strings.action.center_viewport.c_str())) {
      dispatcher.enqueue<CenterViewportEvent>();
    }

    ImGui::Separator();

    if (ImGui::MenuItem(strings.action.reset_zoom.c_str())) {
      dispatcher.enqueue<ResetZoomEvent>();
    }
  }
}

void _push_object_context_menu(const Model& model,
                               const Strings& strings,
                               const Map& map,
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

  if (gViewportState.will_open_object_context_menu) {
    ImGui::OpenPopup(kViewportObjectContextMenuId,
                     ImGuiPopupFlags_AnyPopup | ImGuiPopupFlags_MouseButtonRight);
    gViewportState.will_open_object_context_menu = false;
  }
}

}  // namespace

void show_map_viewport(const Model& model,
                       const Entity map_document_entity,
                       Dispatcher& dispatcher)
{
  const auto& strings = sys::get_current_language_strings(model);
  const auto& settings = model.get<Settings>();

  const auto& map_document = model.get<MapDocument>(map_document_entity);
  const auto& viewport = model.get<Viewport>(map_document_entity);
  const auto& map = model.get<Map>(map_document.map);

  const auto canvas = create_canvas_info(viewport, map.tile_size, map.extent);
  update_document_viewport_offset(canvas.size, dispatcher);

  clear_canvas(canvas, settings.get_viewport_bg_color());
  push_scissor(canvas);

  // TODO viewport should be centered by default
  if (gViewportState.will_be_centered) {
    _center_viewport(viewport, canvas, dispatcher);
    gViewportState.will_be_centered = false;
  }

  sys::render_map(model, canvas, map);

  const auto cursor = get_viewport_cursor_info(canvas);
  _poll_mouse(dispatcher, cursor);

  if (Window::contains_mouse()) {
    _draw_cursor_gizmos(model, canvas, map, cursor);
  }

  pop_scissor();

  show_map_viewport_toolbar(model, dispatcher);
  show_map_viewport_overlay(model, map, cursor, dispatcher);

  _push_viewport_context_menu(strings, map_document.map, dispatcher);
  _push_object_context_menu(model, strings, map, dispatcher);
}

void center_map_viewport()
{
  gViewportState.will_be_centered = true;
}

void open_object_context_menu()
{
  gViewportState.will_open_object_context_menu = true;
}

}  // namespace tactile::ui
