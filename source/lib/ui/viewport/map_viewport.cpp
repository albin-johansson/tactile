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

#include <entt/signal/dispatcher.hpp>
#include <imgui.h>
#include <imgui_internal.h>

#include "core/color.hpp"
#include "core/layer/group_layer.hpp"
#include "core/layer/object_layer.hpp"
#include "document_viewport_offset_handler.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "map_viewport_overlay.hpp"
#include "map_viewport_toolbar.hpp"
#include "model/document/map_document.hpp"
#include "model/event/object_events.hpp"
#include "model/event/property_events.hpp"
#include "model/event/tool_events.hpp"
#include "model/event/viewport_events.hpp"
#include "model/model.hpp"
#include "model/settings.hpp"
#include "ui/conversions.hpp"
#include "ui/render/render.hpp"
#include "ui/render/renderer.hpp"
#include "ui/viewport/preview/tool_preview_renderer.hpp"
#include "ui/widget/scoped.hpp"
#include "viewport_cursor_info.hpp"

namespace tactile::ui {
namespace {

inline constexpr auto kViewportObjectContextMenuId = "##MapViewObjectContextMenu";

struct MapViewportState final {
  bool will_be_centered {};
  bool will_open_object_context_menu {};
};

inline constinit MapViewportState gViewportState;

/// Creates a mouse info struct, but does not set the button member.
[[nodiscard]] auto make_mouse_info(const ViewportCursorInfo& cursor) -> MouseInfo
{
  MouseInfo info;

  info.pos.x = cursor.raw_position.x;
  info.pos.y = cursor.raw_position.y;
  info.position_in_viewport = cursor.map_position;
  info.is_within_contents = cursor.is_within_map;

  return info;
}

template <typename Event, typename T>
void check_for(const ViewportCursorInfo& cursor, entt::dispatcher& dispatcher, T&& query)
{
  const auto left = query(ImGuiMouseButton_Left);
  const auto mid = query(ImGuiMouseButton_Middle);
  const auto right = query(ImGuiMouseButton_Right);
  if (left || mid || right) {
    auto info = make_mouse_info(cursor);

    if (left) {
      info.button = cen::mouse_button::left;
    }
    else if (mid) {
      info.button = cen::mouse_button::middle;
    }
    else /*if (right)*/ {
      info.button = cen::mouse_button::right;
    }

    dispatcher.enqueue<Event>(info);
  }
}

void center_viewport(const Viewport& viewport,
                     const CanvasInfo& canvas_info,
                     entt::dispatcher& dispatcher)
{
  const auto& cell = viewport.tile_size();
  const auto& offset = viewport.get_offset();

  const auto width = canvas_info.col_count * cell.x;
  const auto height = canvas_info.row_count * cell.y;

  const auto dx = std::round(((canvas_info.canvas_size.x - width) / 2.0f) - offset.x);
  const auto dy = std::round(((canvas_info.canvas_size.y - height) / 2.0f) - offset.y);
  const Float2 delta {dx, dy};

  dispatcher.enqueue<OffsetDocumentViewportEvent>(delta);
}

void draw_cursor_gizmos(const Renderer& renderer,
                        const DocumentModel& model,
                        const MapDocument& document,
                        const ViewportCursorInfo& cursor)
{
  const auto& map = document.get_map();

  if (cursor.is_within_map && map.is_active_layer(LayerType::TileLayer)) {
    draw_shadowed_rect(to_vec(cursor.clamped_position),
                       renderer.get_canvas_info().grid_size,
                       Color {0, 0xFF, 0, 200},
                       2.0f);
  }

  ToolPreviewRenderer preview_renderer {model, renderer, make_mouse_info(cursor)};

  const auto& tools = document.get_tools();
  tools.accept(preview_renderer);
}

void poll_mouse(entt::dispatcher& dispatcher, const ViewportCursorInfo& cursor)
{
  if (ImGui::IsPopupOpen(kViewportObjectContextMenuId, ImGuiPopupFlags_AnyPopup)) {
    return;
  }

  if (is_map_toolbar_hovered()) {
    return;
  }

  // FIXME crash: we need to track origin dock for these mouse events, otherwise we might
  // end up emitting dragged events without an initial pressed event, etc.
  if (Window::contains_mouse()) {
    check_for<ViewportMousePressedEvent>(cursor, dispatcher, [](ImGuiMouseButton button) {
      return ImGui::IsMouseClicked(button);
    });

    check_for<ViewportMouseDraggedEvent>(cursor, dispatcher, [](ImGuiMouseButton button) {
      const auto& io = ImGui::GetIO();
      return ImGui::IsMouseDragging(button) && to_vec(io.MouseDelta) != Vec2 {0, 0};
    });

    check_for<ViewportMouseReleasedEvent>(
        cursor,
        dispatcher,
        [](ImGuiMouseButton button) { return ImGui::IsMouseReleased(button); });
  }
}

void update_viewport_context_menu(const Map& map, entt::dispatcher& dispatcher)
{
  constexpr auto flags =
      ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverExistingPopup;

  if (const auto popup = Popup::for_item("##MapViewContextMenu", flags);
      popup.is_open()) {
    const auto& lang = get_current_language();

    if (ImGui::MenuItem(lang.action.inspect_map.c_str())) {
      dispatcher.enqueue<InspectContextEvent>(map.get_uuid());
    }

    ImGui::Separator();

    if (ImGui::MenuItem(lang.action.center_viewport.c_str())) {
      dispatcher.enqueue<CenterViewportEvent>();
    }

    ImGui::Separator();

    if (ImGui::MenuItem(lang.action.reset_zoom.c_str())) {
      dispatcher.enqueue<ResetZoomEvent>();
    }
  }
}

void update_object_context_menu(const Map& map, entt::dispatcher& dispatcher)
{
  if (const Popup popup {kViewportObjectContextMenuId}; popup.is_open()) {
    const auto& lang = get_current_language();
    const auto& layer =
        map.get_invisible_root().get_object_layer(map.get_active_layer_id().value());

    const auto object_id = layer.active_object_id().value();
    const auto& object = layer.get_object(object_id);

    if (ImGui::MenuItem(lang.action.inspect_object.c_str())) {
      dispatcher.enqueue<InspectContextEvent>(object_id);
    }

    ImGui::Separator();
    if (ImGui::MenuItem(lang.action.toggle_object_visibility.c_str(),
                        nullptr,
                        object.is_visible())) {
      dispatcher.enqueue<SetObjectVisibleEvent>(object_id, !object.is_visible());
    }

    // TODO implement the object actions

    ImGui::Separator();

    if (const Disable disable; ImGui::MenuItem(lang.action.duplicate_object.c_str())) {
      // dispatcher.enqueue<DuplicateObjectEvent>(objectId);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(lang.action.remove_object.c_str())) {
      dispatcher.enqueue<RemoveObjectEvent>(object_id);
    }
  }

  if (gViewportState.will_open_object_context_menu) {
    ImGui::OpenPopup(kViewportObjectContextMenuId,
                     ImGuiPopupFlags_AnyPopup | ImGuiPopupFlags_MouseButtonRight);
    gViewportState.will_open_object_context_menu = false;
  }
}

}  // namespace

void show_map_viewport(const DocumentModel& model,
                       const MapDocument& document,
                       entt::dispatcher& dispatcher)
{
  const auto& map = document.get_map();
  const auto& viewport = document.get_viewport();

  const Renderer renderer {viewport, map};
  update_document_viewport_offset(from_vec(renderer.get_canvas_info().canvas_size),
                                  dispatcher);

  renderer.clear(get_settings().get_viewport_bg_color());
  renderer.push_clip();

  // TODO viewport should be centered by default
  if (gViewportState.will_be_centered) {
    center_viewport(viewport, renderer.get_canvas_info(), dispatcher);
    gViewportState.will_be_centered = false;
  }

  renderer.render_map(document.get_map());

  const auto cursor = get_viewport_cursor_info(renderer.get_canvas_info());
  poll_mouse(dispatcher, cursor);

  if (Window::contains_mouse()) {
    draw_cursor_gizmos(renderer, model, document, cursor);
  }

  renderer.pop_clip();

  update_map_viewport_toolbar(model, dispatcher);
  update_map_viewport_overlay(map, cursor);

  update_viewport_context_menu(map, dispatcher);
  update_object_context_menu(map, dispatcher);
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
