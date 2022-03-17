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

#include "map_view.hpp"

#include <cmath>  // round

#include <imgui.h>
#include <imgui_internal.h>

#include "core/components/tools.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "core/systems/tileset_system.hpp"
#include "core/systems/tools/tool_system.hpp"
#include "core/viewport.hpp"
#include "editor/events/map_events.hpp"
#include "editor/events/object_events.hpp"
#include "editor/events/property_events.hpp"
#include "editor/events/tool_events.hpp"
#include "editor/events/viewport_events.hpp"
#include "editor/gui/common/mouse_tracker.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/rendering/graphics.hpp"
#include "editor/gui/rendering/render_info.hpp"
#include "editor/gui/rendering/render_map.hpp"
#include "editor/gui/rendering/render_stamp_preview.hpp"
#include "editor/gui/scoped.hpp"
#include "editor/gui/viewport/viewport_cursor_info.hpp"
#include "editor/gui/viewport/viewport_overlay.hpp"
#include "io/persistence/preferences.hpp"
#include "misc/assert.hpp"

namespace tactile {
namespace {

constexpr auto _object_context_menu_id = "##MapViewObjectContextMenu";
constinit bool _will_center_viewport = false;

template <typename Event, typename T>
void _check_for(const ViewportCursorInfo& cursor, entt::dispatcher& dispatcher, T&& query)
{
  const auto left = query(ImGuiMouseButton_Left);
  const auto mid = query(ImGuiMouseButton_Middle);
  const auto right = query(ImGuiMouseButton_Right);
  if (left || mid || right) {
    MouseInfo info;
    info.x = cursor.raw_position.x;
    info.y = cursor.raw_position.y;
    info.position_in_viewport = cursor.map_position;
    info.is_within_contents = cursor.is_within_map;

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

void _center_viewport(entt::dispatcher& dispatcher,
                      const Viewport& viewport,
                      const ImVec2& canvasSize,
                      const float nRows,
                      const float nCols)
{
  const auto width = nCols * viewport.tile_width;
  const auto height = nRows * viewport.tile_height;

  const auto dx = std::round(((canvasSize.x - width) / 2.0f) - viewport.x_offset);
  const auto dy = std::round(((canvasSize.y - height) / 2.0f) - viewport.y_offset);

  dispatcher.enqueue<OffsetViewportEvent>(dx, dy);
}

void _draw_cursor_gizmos(GraphicsCtx& graphics,
                         const entt::registry& registry,
                         const ViewportCursorInfo& cursor,
                         const RenderInfo& info)
{
  if (cursor.is_within_map && sys::is_tile_layer_active(registry)) {
    graphics.set_draw_color(cen::colors::lime.with_alpha(200));
    graphics.set_line_thickness(2);
    graphics.draw_rect_with_shadow(cursor.clamped_position, info.grid_size);
  }

  if (cursor.is_within_map &&  //
      sys::is_tool_enabled(registry, ToolType::stamp) &&
      sys::is_tileset_selection_not_empty(registry)) {
    render_stamp_preview(registry, cursor.map_position, info);
  }
  else if (sys::is_tool_enabled(registry, ToolType::rectangle)) {
    if (const auto* stroke = registry.try_ctx<comp::CurrentRectangleStroke>()) {
      const ImVec2 pos{stroke->start_x, stroke->start_y};
      const ImVec2 size{stroke->current_x - stroke->start_x,
                        stroke->current_y - stroke->start_y};

      graphics.set_draw_color(cen::colors::yellow);
      graphics.set_line_thickness(1);
      graphics.draw_translated_rect_with_shadow(pos, size);
    }
  }
  else if (sys::is_tool_enabled(registry, ToolType::ellipse)) {
    if (const auto* stroke = registry.try_ctx<comp::CurrentEllipseStroke>()) {
      const ImVec2 radius{(stroke->current_x - stroke->start_x),
                          (stroke->current_y - stroke->start_y)};
      const ImVec2 center{stroke->start_x + radius.x, stroke->start_y + radius.y};

      graphics.set_draw_color(cen::colors::yellow);
      graphics.set_line_thickness(1);
      graphics.draw_translated_ellipse_with_shadow(center, radius);
    }
  }
}

void _poll_mouse(entt::dispatcher& dispatcher, const ViewportCursorInfo& cursor)
{
  if (ImGui::IsPopupOpen(_object_context_menu_id, ImGuiPopupFlags_AnyPopup)) {
    return;
  }

  if (ImGui::IsMouseHoveringRect(ImGui::GetWindowContentRegionMin(),
                                 ImGui::GetWindowContentRegionMax())) {
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

void _update_context_menu([[maybe_unused]] const entt::registry& registry,
                          entt::dispatcher& dispatcher,
                          [[maybe_unused]] const ViewportCursorInfo& cursor)
{
  constexpr auto flags =
      ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverExistingPopup;

  if (const auto popup = scoped::Popup::for_item("##MapViewContextMenu", flags);
      popup.is_open()) {
    if (ImGui::MenuItem(TAC_ICON_INSPECT " Inspect Map")) {
      dispatcher.enqueue<InspectMapEvent>();
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

}  // namespace

void update_map_view(const entt::registry& registry, entt::dispatcher& dispatcher)
{
  const auto& viewport = registry.ctx<Viewport>();
  const auto& map = registry.ctx<MapInfo>();

  const auto info = get_render_info(viewport, map);
  update_viewport_offset(info.canvas_br - info.canvas_tl, dispatcher);

  GraphicsCtx graphics{info};

  graphics.set_draw_color(get_preferences().viewport_bg());
  graphics.clear();

  graphics.push_clip();

  // TODO viewport should be centered by default
  if (_will_center_viewport) {
    _center_viewport(dispatcher,
                     viewport,
                     info.canvas_br - info.canvas_tl,
                     info.row_count,
                     info.col_count);
    _will_center_viewport = false;
  }

  render_map(graphics, registry);

  const auto cursor = GetViewportCursorInfo(info);
  _poll_mouse(dispatcher, cursor);

  if (ImGui::IsWindowHovered()) {
    _draw_cursor_gizmos(graphics, registry, cursor, info);
  }

  graphics.pop_clip();

  update_viewport_overlay(registry, cursor);
  _update_context_menu(registry, dispatcher, cursor);
}

void update_map_view_object_context_menu(const entt::registry& registry,
                                         entt::dispatcher& dispatcher)
{
  if (scoped::Popup popup{_object_context_menu_id}; popup.is_open()) {
    const auto active = registry.ctx<comp::ActiveObject>();

    TACTILE_ASSERT(active.entity != entt::null);
    const auto& object = registry.get<comp::Object>(active.entity);

    if (ImGui::MenuItem(TAC_ICON_INSPECT " Inspect Object")) {
      dispatcher.enqueue<InspectContextEvent>(active.entity);
    }

    ImGui::Separator();
    if (ImGui::MenuItem(TAC_ICON_VISIBILITY " Toggle Object Visibility",
                        nullptr,
                        object.visible)) {
      dispatcher.enqueue<SetObjectVisibilityEvent>(object.id, !object.visible);
    }

    // TODO implement the object actions
    scoped::Disable disable;

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_DUPLICATE " Duplicate Object")) {
      dispatcher.enqueue<DuplicateObjectEvent>(object.id);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_REMOVE " Remove Object")) {
      dispatcher.enqueue<RemoveObjectEvent>(object.id);
    }
  }
}

// TODO the declaration of this function isn't in map_view.hpp
void center_map_viewport()
{
  _will_center_viewport = true;
}

void open_object_context_menu()
{
  ImGui::OpenPopup(_object_context_menu_id,
                   ImGuiPopupFlags_AnyPopup | ImGuiPopupFlags_MouseButtonRight);
}

}  // namespace tactile
