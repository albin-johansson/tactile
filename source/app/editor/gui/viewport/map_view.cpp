#include "map_view.hpp"

#include <cmath>  // round

#include <imgui.h>
#include <imgui_internal.h>

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
#include "io/persistence/preferences.hpp"
#include "misc/assert.hpp"
#include "viewport_cursor_info.hpp"
#include "viewport_overlay.hpp"

namespace tactile {
namespace {

constexpr auto gTileHighlightColor = IM_COL32(0, 255, 0, 200);

constinit bool gCenterViewport = false;

template <typename Event, typename T>
void CheckFor(const ViewportCursorInfo& cursor, entt::dispatcher& dispatcher, T&& query)
{
  const auto left = query(ImGuiMouseButton_Left);
  const auto mid = query(ImGuiMouseButton_Middle);
  const auto right = query(ImGuiMouseButton_Right);
  if (left || mid || right) {
    mouse_info info;
    info.x = cursor.raw_position.x;
    info.y = cursor.raw_position.y;
    info.position_in_viewport = cursor.map_position;

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

void CenterViewport(entt::dispatcher& dispatcher,
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

void UpdateCursorGizmos(const entt::registry& registry,
                        entt::dispatcher& dispatcher,
                        const ViewportCursorInfo& cursor,
                        const render_info& info)
{
  TACTILE_ASSERT(cursor.is_within_map);

  if (!ImGui::IsWindowFocused() && !ImGui::IsWindowHovered()) {
    return;
  }

  if (sys::is_tile_layer_active(registry)) {
    ImGui::GetWindowDrawList()->AddRect(cursor.clamped_position,
                                        cursor.clamped_position + info.grid_size,
                                        gTileHighlightColor,
                                        0,
                                        0,
                                        2);
  }

  if (ImGui::IsMouseHoveringRect(ImGui::GetWindowPos(),
                                 ImGui::GetWindowPos() + ImGui::GetWindowSize())) {
    CheckFor<mouse_pressed_event>(cursor, dispatcher, [](ImGuiMouseButton button) {
      return ImGui::IsMouseClicked(button);
    });

    CheckFor<mouse_drag_event>(cursor, dispatcher, [](ImGuiMouseButton button) {
      return ImGui::IsMouseDragging(button);
    });

    CheckFor<mouse_released_event>(cursor, dispatcher, [](ImGuiMouseButton button) {
      return ImGui::IsMouseReleased(button);
    });
  }

  if (sys::is_tool_enabled(registry, tool_type::stamp) &&
      sys::is_tileset_selection_not_empty(registry)) {
    RenderStampPreview(registry, cursor.map_position, info);
  }
}

void UpdateContextMenu([[maybe_unused]] const entt::registry& registry,
                       entt::dispatcher& dispatcher,
                       [[maybe_unused]] const ViewportCursorInfo& cursor)
{
  constexpr auto flags =
      ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverExistingPopup;

  if (const auto popup = scoped::popup::for_item("##MapViewContextMenu", flags);
      popup.is_open()) {
    if (ImGui::MenuItem(TAC_ICON_INSPECT " Inspect Map")) {
      dispatcher.enqueue<inspect_map_event>();
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

void UpdateMapView(const entt::registry& registry, entt::dispatcher& dispatcher)
{
  const auto& viewport = registry.ctx<Viewport>();
  const auto& map = registry.ctx<MapInfo>();

  const auto info = get_render_info(viewport, map);
  UpdateViewportOffset(info.canvas_br - info.canvas_tl, dispatcher);

  graphics_ctx graphics{info};

  graphics.set_draw_color(get_preferences().viewport_bg());
  graphics.clear();

  graphics.push_clip();

  // TODO viewport should be centered by default
  if (gCenterViewport) {
    CenterViewport(dispatcher,
                   viewport,
                   info.canvas_br - info.canvas_tl,
                   info.row_count,
                   info.col_count);
    gCenterViewport = false;
  }

  RenderMap(graphics, registry);

  const auto cursor = GetViewportCursorInfo(info);
  if (cursor.is_within_map) {
    UpdateCursorGizmos(registry, dispatcher, cursor, info);
  }

  graphics.pop_clip();

  UpdateViewportOverlay(registry, cursor);
  UpdateContextMenu(registry, dispatcher, cursor);
}

void UpdateMapViewObjectContextMenu(const entt::registry& registry,
                                    entt::dispatcher& dispatcher)
{
  if (scoped::popup popup{"##MapViewObjectContextMenu"}; popup.is_open()) {
    const auto active = registry.ctx<comp::active_object>();

    TACTILE_ASSERT(active.entity != entt::null);
    const auto& object = registry.get<comp::object>(active.entity);

    if (ImGui::MenuItem(TAC_ICON_INSPECT " Inspect Object")) {
      dispatcher.enqueue<inspect_context_event>(active.entity);
    }

    ImGui::Separator();
    if (ImGui::MenuItem(TAC_ICON_VISIBILITY " Toggle Object Visibility",
                        nullptr,
                        object.visible)) {
      dispatcher.enqueue<set_object_visibility_event>(object.id, !object.visible);
    }

    // TODO implement the object actions
    scoped::disable disable;

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_DUPLICATE " Duplicate Object")) {
      dispatcher.enqueue<duplicate_object_event>(object.id);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_REMOVE " Remove Object")) {
      dispatcher.enqueue<remove_object_event>(object.id);
    }
  }
}

// TODO the declaration of this function isn't in map_view.hpp
void CenterMapViewport()
{
  gCenterViewport = true;
}

void OpenObjectContextMenu()
{
  ImGui::OpenPopup("##MapViewObjectContextMenu",
                   ImGuiPopupFlags_AnyPopup | ImGuiPopupFlags_MouseButtonRight);
}

}  // namespace tactile
