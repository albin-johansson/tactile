#include "map_view.hpp"

#include <cassert>  // assert
#include <cmath>    // round

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
#include "editor/gui/rendering/canvas.hpp"
#include "editor/gui/rendering/render_bounds.hpp"
#include "editor/gui/rendering/render_info.hpp"
#include "editor/gui/rendering/render_map.hpp"
#include "editor/gui/rendering/render_stamp_preview.hpp"
#include "editor/gui/scoped.hpp"
#include "viewport_cursor_info.hpp"
#include "viewport_overlay.hpp"

namespace Tactile {
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
    MouseInfo info;
    info.x = cursor.raw_position.x;
    info.y = cursor.raw_position.y;
    info.position_in_map = cursor.map_position;

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
                        const RenderInfo& info)
{
  assert(cursor.is_within_map);

  if (!ImGui::IsWindowFocused() && !ImGui::IsWindowHovered()) {
    return;
  }

  if (Sys::IsTileLayerActive(registry)) {
    ImGui::GetWindowDrawList()->AddRect(cursor.clamped_position,
                                        cursor.clamped_position + info.grid_size,
                                        gTileHighlightColor,
                                        0,
                                        0,
                                        2);
  }

  if (ImGui::IsMouseHoveringRect(ImGui::GetWindowPos(),
                                 ImGui::GetWindowPos() + ImGui::GetWindowSize()))
  {
    CheckFor<MousePressedEvent>(cursor, dispatcher, [](ImGuiMouseButton button) {
      return ImGui::IsMouseClicked(button);
    });

    CheckFor<MouseDragEvent>(cursor, dispatcher, [](ImGuiMouseButton button) {
      return ImGui::IsMouseDragging(button);
    });

    CheckFor<MouseReleasedEvent>(cursor, dispatcher, [](ImGuiMouseButton button) {
      return ImGui::IsMouseReleased(button);
    });
  }

  if (Sys::IsStampEnabled(registry) && Sys::HasNonEmptyTilesetSelection(registry)) {
    RenderStampPreview(registry, cursor.map_position, info);
  }
}

void UpdateContextMenu([[maybe_unused]] const entt::registry& registry,
                       entt::dispatcher& dispatcher,
                       [[maybe_unused]] const ViewportCursorInfo& cursor)
{
  constexpr auto flags =
      ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverExistingPopup;

  if (const auto popup = Scoped::Popup::ForItem("##MapViewContextMenu", flags);
      popup.IsOpen()) {
    if (ImGui::MenuItem(TAC_ICON_PROPERTIES " Show Map Properties")) {
      dispatcher.enqueue<ShowMapPropertiesEvent>();
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
  const auto canvas = GetCanvasInfo();
  ClearBackground(canvas);
  UpdateViewportOffset(canvas, dispatcher);

  auto* drawList = ImGui::GetWindowDrawList();
  drawList->PushClipRect(canvas.tl, canvas.br, true);

  const auto& viewport = registry.ctx<Viewport>();
  const auto info = GetRenderInfo(registry, canvas);

  // TODO viewport should be centered by default
  if (gCenterViewport) {
    CenterViewport(dispatcher, viewport, canvas.size, info.row_count, info.col_count);
    gCenterViewport = false;
  }

  RenderMap(registry, info);

  const auto cursor = GetViewportCursorInfo(info);
  if (cursor.is_within_map) {
    UpdateCursorGizmos(registry, dispatcher, cursor, info);
  }

  drawList->PopClipRect();
  UpdateViewportOverlay(registry, cursor);
  UpdateContextMenu(registry, dispatcher, cursor);
}

void UpdateMapViewObjectContextMenu(const entt::registry& registry,
                                    entt::dispatcher& dispatcher)
{
  if (Scoped::Popup popup{"##MapViewObjectContextMenu"}; popup.IsOpen()) {
    const auto active = registry.ctx<ActiveObject>();

    assert(active.entity != entt::null);
    const auto& object = registry.get<Object>(active.entity);

    if (ImGui::MenuItem(TAC_ICON_PROPERTIES " Show Object Properties")) {
      dispatcher.enqueue<SetPropertyContextEvent>(active.entity);
    }

    ImGui::Separator();
    if (ImGui::MenuItem(TAC_ICON_VISIBILITY " Toggle Object Visibility",
                        nullptr,
                        object.visible)) {
      dispatcher.enqueue<SetObjectVisibilityEvent>(object.id, !object.visible);
    }

    // TODO implement the object actions
    Scoped::Disable disable;

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
void CenterMapViewport()
{
  gCenterViewport = true;
}

void OpenObjectContextMenu()
{
  ImGui::OpenPopup("##MapViewObjectContextMenu",
                   ImGuiPopupFlags_AnyPopup | ImGuiPopupFlags_MouseButtonRight);
}

}  // namespace Tactile
