#include "map_view.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include <cassert>  // assert
#include <cmath>    // round

#include "core/ctx/viewport.hpp"
#include "core/systems/tileset_system.hpp"
#include "core/systems/tools/tool_system.hpp"
#include "events/tool_events.hpp"
#include "events/viewport_events.hpp"
#include "gui/rendering/canvas.hpp"
#include "gui/rendering/render_bounds.hpp"
#include "gui/rendering/render_info.hpp"
#include "gui/rendering/render_map.hpp"
#include "gui/rendering/render_stamp_preview.hpp"
#include "gui/widgets/common/mouse_tracker.hpp"
#include "viewport_cursor_info.hpp"
#include "viewport_overlay.hpp"

namespace Tactile {
namespace {

constexpr auto tile_highlight_color = IM_COL32(0, 255, 0, 200);

constinit bool center_viewport = false;

template <typename Event, typename T>
void CheckFor(const ViewportCursorInfo& cursor, entt::dispatcher& dispatcher, T&& query)
{
  const auto left = query(ImGuiMouseButton_Left);
  const auto mid = query(ImGuiMouseButton_Middle);
  const auto right = query(ImGuiMouseButton_Right);
  if (left || mid || right) {
    MouseInfo info;
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

void RenderCursorGizmos(const entt::registry& registry,
                        entt::dispatcher& dispatcher,
                        const ViewportCursorInfo& cursor,
                        const RenderInfo& info)
{
  assert(cursor.is_within_map);

  if (!ImGui::IsWindowFocused() && !ImGui::IsWindowHovered()) {
    return;
  }

  ImGui::GetWindowDrawList()->AddRect(cursor.clamped_position,
                                      cursor.clamped_position + info.grid_size,
                                      tile_highlight_color,
                                      0,
                                      0,
                                      2);

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

}  // namespace

void UpdateMapView(const entt::registry& registry, entt::dispatcher& dispatcher)
{
  const auto canvas = GetCanvasInfo();
  FillBackground(canvas);
  UpdateViewportOffset(canvas, dispatcher);

  auto* drawList = ImGui::GetWindowDrawList();
  drawList->PushClipRect(canvas.tl, canvas.br, true);

  const auto& viewport = registry.ctx<Viewport>();
  const auto info = GetRenderInfo(registry, canvas);

  // TODO viewport should be centered by default
  if (center_viewport) {
    CenterViewport(dispatcher, viewport, canvas.size, info.row_count, info.col_count);
    center_viewport = false;
  }

  RenderMap(registry, info);

  const auto cursor = GetViewportCursorInfo(info);
  if (cursor.is_within_map) {
    RenderCursorGizmos(registry, dispatcher, cursor, info);
  }

  drawList->PopClipRect();

  ViewportOverlay(registry, canvas, cursor);
}

void CenterMapViewport()
{
  center_viewport = true;
}

}  // namespace Tactile
