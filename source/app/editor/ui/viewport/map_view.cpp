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

#include <entt/entity/registry.hpp>
#include <entt/signal/dispatcher.hpp>
#include <imgui.h>
#include <imgui_internal.h>

#include "core/common/ecs.hpp"
#include "core/components/viewport.hpp"
#include "core/documents/map_document.hpp"
#include "core/events/map_events.hpp"
#include "core/events/object_events.hpp"
#include "core/events/property_events.hpp"
#include "core/events/tool_events.hpp"
#include "core/events/viewport_events.hpp"
#include "core/model.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "core/tools/tool_manager.hpp"
#include "editor/ui/common/mouse_tracker.hpp"
#include "editor/ui/icons.hpp"
#include "editor/ui/rendering/graphics.hpp"
#include "editor/ui/rendering/render_info.hpp"
#include "editor/ui/rendering/render_map.hpp"
#include "editor/ui/scoped.hpp"
#include "editor/ui/viewport/toolbar.hpp"
#include "editor/ui/viewport/viewport_cursor_info.hpp"
#include "editor/ui/viewport/viewport_overlay.hpp"
#include "io/persistence/preferences.hpp"
#include "misc/assert.hpp"

namespace tactile::ui {
namespace {

constexpr auto _object_context_menu_id = "##MapViewObjectContextMenu";
constinit bool _will_center_viewport = false;

/* Creates a mouse info struct, but does not set the button member */
[[nodiscard]] auto _make_mouse_info(const ViewportCursorInfo& cursor) -> MouseInfo
{
  MouseInfo info;

  info.x = cursor.raw_position.x;
  info.y = cursor.raw_position.y;
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

void _center_viewport(entt::dispatcher& dispatcher,
                      const comp::Viewport& viewport,
                      const ImVec2& canvasSize,
                      const float nRows,
                      const float nCols)
{
  const auto width = nCols * viewport.tile_size.x;
  const auto height = nRows * viewport.tile_size.y;

  const auto dx = std::round(((canvasSize.x - width) / 2.0f) - viewport.offset.x);
  const auto dy = std::round(((canvasSize.y - height) / 2.0f) - viewport.offset.y);

  dispatcher.enqueue<OffsetViewportEvent>(dx, dy);
}

void _draw_cursor_gizmos(GraphicsCtx& graphics,
                         const DocumentModel& model,
                         const MapDocument& document,
                         const ViewportCursorInfo& cursor,
                         const RenderInfo& info)
{
  const auto& registry = document.get_registry();
  if (cursor.is_within_map && sys::is_tile_layer_active(registry)) {
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

void _update_context_menu(entt::dispatcher& dispatcher)
{
  constexpr auto flags =
      ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverExistingPopup;

  if (const auto popup = Popup::for_item("##MapViewContextMenu", flags);
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

void update_map_view(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  const auto& map = model.view_map(model.active_document_id().value());

  const auto& registry = map.get_registry();
  const auto& viewport = map.viewport();

  const auto info = get_render_info(viewport, map.info());
  update_viewport_offset(info.canvas_br - info.canvas_tl, dispatcher);

  GraphicsCtx graphics{info};

  graphics.set_draw_color(io::get_preferences().viewport_bg());
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

  render_map(graphics, model, map);

  const auto cursor = GetViewportCursorInfo(info);  // TODO rename function
  _poll_mouse(dispatcher, cursor);

  if (ImGui::IsMouseHoveringRect(ImGui::GetWindowContentRegionMin(),
                                 ImGui::GetWindowContentRegionMax())) {
    _draw_cursor_gizmos(graphics, model, map, cursor, info);
  }

  graphics.pop_clip();

  update_viewport_toolbar(model, dispatcher);
  update_viewport_overlay(registry, cursor);

  _update_context_menu(dispatcher);
}

void update_map_view_object_context_menu(const entt::registry& registry,
                                         entt::dispatcher& dispatcher)
{
  if (Popup popup{_object_context_menu_id}; popup.is_open()) {
    const auto active = ctx_get<comp::ActiveObject>(registry);

    TACTILE_ASSERT(active.entity != entt::null);
    const auto& object = checked_get<comp::Object>(registry, active.entity);

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
    Disable disable;

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

void center_map_viewport()
{
  _will_center_viewport = true;
}

void open_object_context_menu()
{
  ImGui::OpenPopup(_object_context_menu_id,
                   ImGuiPopupFlags_AnyPopup | ImGuiPopupFlags_MouseButtonRight);
}

}  // namespace tactile::ui
