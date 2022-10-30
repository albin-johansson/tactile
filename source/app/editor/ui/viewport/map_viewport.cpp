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

#include "core/layer/object_layer.hpp"
#include "document_viewport_offset_handler.hpp"
#include "editor/ui/conversions.hpp"
#include "editor/ui/render/graphics.hpp"
#include "editor/ui/render/render.hpp"
#include "editor/ui/render/render_info.hpp"
#include "editor/ui/render/render_map.hpp"
#include "editor/ui/viewport/map_viewport_overlay.hpp"
#include "editor/ui/viewport/map_viewport_toolbar.hpp"
#include "editor/ui/viewport/preview/tool_preview_renderer.hpp"
#include "editor/ui/viewport/viewport_cursor_info.hpp"
#include "editor/ui/widget/scoped.hpp"
#include "io/proto/preferences.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/document/map_document.hpp"
#include "model/event/object_events.hpp"
#include "model/event/property_events.hpp"
#include "model/event/tool_events.hpp"
#include "model/event/viewport_events.hpp"
#include "model/model.hpp"

namespace tactile::ui {
namespace {

constexpr auto viewport_object_context_menu_id = "##MapViewObjectContextMenu";
constinit bool viewport_will_be_centered = false;
constinit bool viewport_will_open_object_context_menu = false;

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
                     const ImVec2& canvas_size,
                     const float row_count,
                     const float col_count,
                     entt::dispatcher& dispatcher)
{
  const auto& cell = viewport.tile_size();
  const auto& offset = viewport.get_offset();

  const auto width = col_count * cell.x;
  const auto height = row_count * cell.y;

  const auto dx = std::round(((canvas_size.x - width) / 2.0f) - offset.x);
  const auto dy = std::round(((canvas_size.y - height) / 2.0f) - offset.y);
  const Float2 delta {dx, dy};

  dispatcher.enqueue<OffsetDocumentViewportEvent>(delta);
}

void draw_cursor_gizmos(Graphics& graphics,
                        const DocumentModel& model,
                        const MapDocument& document,
                        const ViewportCursorInfo& cursor,
                        const RenderInfo& info)
{
  const auto& map = document.get_map();

  if (cursor.is_within_map && map.is_active_layer(LayerType::TileLayer)) {
    draw_shadowed_rect(cursor.clamped_position,
                       info.grid_size,
                       IM_COL32(0, 0xFF, 0, 200),
                       2.0f);
  }

  ToolPreviewRenderer preview_renderer {model, graphics, make_mouse_info(cursor)};

  const auto& tools = document.get_tools();
  tools.accept(preview_renderer);
}

void poll_mouse(entt::dispatcher& dispatcher, const ViewportCursorInfo& cursor)
{
  if (ImGui::IsPopupOpen(viewport_object_context_menu_id, ImGuiPopupFlags_AnyPopup)) {
    return;
  }

  if (is_map_toolbar_hovered()) {
    return;
  }

  // FIXME crash: we need to track origin dock for these mouse events, otherwise we might
  // end up emitting dragged events without an initial pressed event, etc.
  if (Window::contains_mouse()) {
    check_for<ToolPressedEvent>(cursor, dispatcher, [](ImGuiMouseButton button) {
      return ImGui::IsMouseClicked(button);
    });

    check_for<ToolDraggedEvent>(cursor, dispatcher, [](ImGuiMouseButton button) {
      return ImGui::IsMouseDragging(button);
    });

    check_for<ToolReleasedEvent>(cursor, dispatcher, [](ImGuiMouseButton button) {
      return ImGui::IsMouseReleased(button);
    });
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
      dispatcher.enqueue<InspectContextEvent>(map.uuid());
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
  if (Popup popup {viewport_object_context_menu_id}; popup.is_open()) {
    const auto& lang = get_current_language();
    const auto& layer = map.invisible_root().object_layer(map.active_layer_id().value());

    const auto object_id = layer.active_object_id().value();
    const auto& object = layer.get_object(object_id);

    if (ImGui::MenuItem(lang.action.inspect_object.c_str())) {
      dispatcher.enqueue<InspectContextEvent>(object_id);
    }

    ImGui::Separator();
    if (ImGui::MenuItem(lang.action.toggle_object_visibility.c_str(),
                        nullptr,
                        object.visible())) {
      dispatcher.enqueue<SetObjectVisibleEvent>(object_id, !object.visible());
    }

    // TODO implement the object actions

    ImGui::Separator();

    if (Disable disable; ImGui::MenuItem(lang.action.duplicate_object.c_str())) {
      // dispatcher.enqueue<DuplicateObjectEvent>(objectId);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(lang.action.remove_object.c_str())) {
      dispatcher.enqueue<RemoveObjectEvent>(object_id);
    }
  }

  if (viewport_will_open_object_context_menu) {
    ImGui::OpenPopup(viewport_object_context_menu_id,
                     ImGuiPopupFlags_AnyPopup | ImGuiPopupFlags_MouseButtonRight);
    viewport_will_open_object_context_menu = false;
  }
}

}  // namespace

void show_map_viewport(const DocumentModel& model,
                       const MapDocument& document,
                       entt::dispatcher& dispatcher)
{
  const auto& map = document.get_map();
  const auto& viewport = document.get_viewport();

  const auto info = get_render_info(viewport, map);
  update_document_viewport_offset(info.canvas_br - info.canvas_tl, dispatcher);

  Graphics graphics {info};

  graphics.clear(to_u32(io::get_preferences().viewport_background));
  graphics.push_canvas_clip();

  // TODO viewport should be centered by default
  if (viewport_will_be_centered) {
    center_viewport(viewport,
                    info.canvas_br - info.canvas_tl,
                    info.row_count,
                    info.col_count,
                    dispatcher);
    viewport_will_be_centered = false;
  }

  render_map(graphics, document);

  const auto cursor = get_viewport_cursor_info(info);
  poll_mouse(dispatcher, cursor);

  if (Window::contains_mouse()) {
    draw_cursor_gizmos(graphics, model, document, cursor, info);
  }

  graphics.pop_clip();

  update_map_viewport_toolbar(model, dispatcher);
  update_map_viewport_overlay(map, cursor);

  update_viewport_context_menu(map, dispatcher);
  update_object_context_menu(map, dispatcher);
}

void center_map_viewport()
{
  viewport_will_be_centered = true;
}

void open_object_context_menu()
{
  viewport_will_open_object_context_menu = true;
}

}  // namespace tactile::ui
