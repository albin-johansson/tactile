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

#include "map_viewport.hpp"

#include <cmath>  // round

#include <entt/signal/dispatcher.hpp>
#include <imgui.h>
#include <imgui_internal.h>

#include "core/documents/map_document.hpp"
#include "core/events/map_events.hpp"
#include "core/events/object_events.hpp"
#include "core/events/property_events.hpp"
#include "core/events/tool_events.hpp"
#include "core/events/viewport_events.hpp"
#include "core/layers/object_layer.hpp"
#include "core/model.hpp"
#include "core/tools/tool_manager.hpp"
#include "editor/ui/common/mouse_tracker.hpp"
#include "editor/ui/icons.hpp"
#include "editor/ui/rendering/graphics.hpp"
#include "editor/ui/rendering/render_info.hpp"
#include "editor/ui/rendering/render_map.hpp"
#include "editor/ui/scoped.hpp"
#include "editor/ui/viewport/map_viewport_overlay.hpp"
#include "editor/ui/viewport/toolbar.hpp"
#include "editor/ui/viewport/viewport_cursor_info.hpp"
#include "io/persistence/preferences.hpp"

using namespace tactile::core;

namespace tactile::ui {
namespace {

constexpr auto _object_context_menu_id = "##MapViewObjectContextMenu";
constinit bool _will_center_viewport = false;
constinit bool _open_object_context_menu = false;

/* Creates a mouse info struct, but does not set the button member */
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
void _check_for(const ViewportCursorInfo& cursor, entt::dispatcher& dispatcher, T&& query)
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
    else /*if (right)*/ {
      info.button = cen::mouse_button::right;
    }

    dispatcher.enqueue<Event>(info);
  }
}

void _center_viewport(const Viewport&   viewport,
                      const ImVec2&     canvasSize,
                      const float       nRows,
                      const float       nCols,
                      entt::dispatcher& dispatcher)
{
  const auto& cell = viewport.get_cell_size();
  const auto& offset = viewport.get_offset();

  const auto width = nCols * cell.x;
  const auto height = nRows * cell.y;

  const auto     dx = std::round(((canvasSize.x - width) / 2.0f) - offset.x);
  const auto     dy = std::round(((canvasSize.y - height) / 2.0f) - offset.y);
  const Vector2f delta{dx, dy};

  dispatcher.enqueue<OffsetDocumentViewportEvent>(delta);
}

void _draw_cursor_gizmos(GraphicsCtx&              graphics,
                         const DocumentModel&      model,
                         const MapDocument&        document,
                         const ViewportCursorInfo& cursor,
                         const RenderInfo&         info)
{
  const auto& map = document.get_map();

  if (cursor.is_within_map && map.is_active_layer(LayerType::TileLayer)) {
    graphics.set_draw_color(cen::colors::lime.with_alpha(200));
    graphics.set_line_thickness(2);
    graphics.draw_rect_with_shadow(cursor.clamped_position, info.grid_size);
  }

  const auto& tools = document.get_tools();
  tools.draw_gizmos(model, graphics, _make_mouse_info(cursor));
}

void _poll_mouse(entt::dispatcher& dispatcher, const ViewportCursorInfo& cursor)
{
  if (ImGui::IsPopupOpen(_object_context_menu_id, ImGuiPopupFlags_AnyPopup)) {
    return;
  }

  if (is_toolbar_hovered()) {
    return;
  }

  if (Window::contains_mouse()) {
    _check_for<ToolPressedEvent>(cursor, dispatcher, [](ImGuiMouseButton button) {
      return ImGui::IsMouseClicked(button);
    });

    _check_for<ToolDraggedEvent>(cursor, dispatcher, [](ImGuiMouseButton button) {
      return ImGui::IsMouseDragging(button);
    });

    _check_for<ToolReleasedEvent>(cursor, dispatcher, [](ImGuiMouseButton button) {
      return ImGui::IsMouseReleased(button);
    });
  }
}

void _update_context_menu(const Map& map, entt::dispatcher& dispatcher)
{
  constexpr auto flags =
      ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverExistingPopup;

  if (const auto popup = Popup::for_item("##MapViewContextMenu", flags);
      popup.is_open()) {
    if (ImGui::MenuItem(TAC_ICON_INSPECT " Inspect Map")) {
      dispatcher.enqueue<InspectContextEvent>(map.get_uuid());
    }

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_CENTER " Center Viewport")) {
      dispatcher.enqueue<CenterViewportEvent>();
    }

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_RESET_ZOOM " Reset Viewport Zoom")) {
      dispatcher.enqueue<ResetZoomEvent>();
    }
  }
}

void _update_map_view_object_context_menu(const Map& map, entt::dispatcher& dispatcher)
{
  if (Popup popup{_object_context_menu_id}; popup.is_open()) {
    const auto& layer = map.view_object_layer(map.active_layer_id().value());

    const auto  objectId = layer.active_object_id().value();
    const auto& object = layer.get_object(objectId);

    if (ImGui::MenuItem(TAC_ICON_INSPECT " Inspect Object")) {
      dispatcher.enqueue<InspectContextEvent>(objectId);
    }

    ImGui::Separator();
    if (ImGui::MenuItem(TAC_ICON_VISIBILITY " Toggle Object Visibility",
                        nullptr,
                        object.is_visible())) {
      // dispatcher.enqueue<SetObjectVisibilityEvent>(objectId, !object.is_visible());
    }

    // TODO implement the object actions
    Disable disable;

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_DUPLICATE " Duplicate Object")) {
      // dispatcher.enqueue<DuplicateObjectEvent>(objectId);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_REMOVE " Remove Object")) {
      // dispatcher.enqueue<RemoveObjectEvent>(objectId);
    }
  }

  if (_open_object_context_menu) {
    ImGui::OpenPopup(_object_context_menu_id,
                     ImGuiPopupFlags_AnyPopup | ImGuiPopupFlags_MouseButtonRight);
    _open_object_context_menu = false;
  }
}

}  // namespace

void show_map_viewport(const DocumentModel& model,
                       const MapDocument&   document,
                       entt::dispatcher&    dispatcher)
{
  const auto& map = document.get_map();
  const auto& viewport = document.get_viewport();

  const auto info = get_render_info(viewport, map);
  update_document_viewport_offset(info.canvas_br - info.canvas_tl, dispatcher);

  GraphicsCtx graphics{info};

  graphics.set_draw_color(io::get_preferences().viewport_bg());
  graphics.clear();

  graphics.push_clip();

  // TODO viewport should be centered by default
  if (_will_center_viewport) {
    _center_viewport(viewport,
                     info.canvas_br - info.canvas_tl,
                     info.row_count,
                     info.col_count,
                     dispatcher);
    _will_center_viewport = false;
  }

  render_map(graphics, document);

  const auto cursor = get_viewport_cursor_info(info);
  _poll_mouse(dispatcher, cursor);

  if (Window::contains_mouse()) {
    _draw_cursor_gizmos(graphics, model, document, cursor, info);
  }

  graphics.pop_clip();

  update_viewport_toolbar(model, dispatcher);
  update_map_viewport_overlay(map, cursor);

  _update_context_menu(map, dispatcher);
  _update_map_view_object_context_menu(map, dispatcher);
}

void center_map_viewport()
{
  _will_center_viewport = true;
}

void open_object_context_menu()
{
  _open_object_context_menu = true;
}

}  // namespace tactile::ui
