#include "map_view.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include <cassert>  // assert

#include "core/model.hpp"
#include "events/tools/mouse_drag_event.hpp"
#include "events/tools/mouse_pressed_event.hpp"
#include "events/tools/mouse_released_event.hpp"
#include "events/viewport/offset_viewport_event.hpp"
#include "gui/rendering/canvas.hpp"
#include "gui/rendering/render_bounds.hpp"
#include "gui/rendering/render_info.hpp"
#include "gui/rendering/render_map.hpp"
#include "gui/widgets/common/mouse_tracker.hpp"
#include "map_content_overlay.hpp"
#include "render_stamp_preview.hpp"
#include "viewport_cursor_info.hpp"

namespace Tactile {
namespace {

constexpr auto tile_highlight_color = IM_COL32(0, 255, 0, 200);

constinit bool center_viewport = false;

template <typename Event, typename T>
void CheckFor(const ViewportCursorInfo& cursor,
              entt::dispatcher& dispatcher,
              T&& query)
{
  const auto left = query(ImGuiMouseButton_Left);
  const auto mid = query(ImGuiMouseButton_Middle);
  const auto right = query(ImGuiMouseButton_Right);
  if (left || mid || right)
  {
    MouseInfo info;
    info.mouse_position_in_map = cursor.map_position;

    if (left)
    {
      info.button = cen::mouse_button::left;
    }
    else if (mid)
    {
      info.button = cen::mouse_button::middle;
    }
    else /*if (right)*/
    {
      info.button = cen::mouse_button::right;
    }

    dispatcher.enqueue<Event>(info);
  }
}

void CenterViewport(entt::dispatcher& dispatcher,
                    const ViewportInfo& viewportInfo,
                    const ImVec2& canvasSize,
                    const float nRows,
                    const float nCols)
{
  const auto width = nCols * viewportInfo.tile_width;
  const auto height = nRows * viewportInfo.tile_height;

  const auto dx = ((canvasSize.x - width) / 2.0f) - viewportInfo.x_offset;
  const auto dy = ((canvasSize.y - height) / 2.0f) - viewportInfo.y_offset;
  dispatcher.enqueue<OffsetViewportEvent>(dx, dy);
}

void RenderCursorGizmos(const Model& model,
                        const MapDocument& document,
                        entt::dispatcher& dispatcher,
                        const ViewportCursorInfo& cursor,
                        const RenderInfo& info)
{
  assert(cursor.is_within_map);

  if (!ImGui::IsWindowFocused() && !ImGui::IsWindowHovered())
  {
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

  const auto& tilesets = document.GetTilesets();
  const auto* tileset = tilesets.GetActiveTileset();
  if (model.GetActiveTool() == MouseToolType::Stamp && tileset &&
      tileset->GetSelection())
  {
    RenderStampPreview(info.map_position,
                       info.grid_size,
                       document.GetMap(),
                       *tileset,
                       cursor.map_position);
  }
}

}  // namespace

void MapView(const Model& model,
             const MapDocument& document,
             entt::dispatcher& dispatcher)
{
  const auto canvas = GetCanvasInfo();
  FillBackground(canvas);
  UpdateViewportOffset(canvas, dispatcher);

  auto* drawList = ImGui::GetWindowDrawList();
  drawList->PushClipRect(canvas.tl, canvas.br, true);

  const auto viewportInfo = document.GetViewportInfo();
  const auto info = GetRenderInfo(document, canvas);

  // TODO viewport should be centered by default
  if (center_viewport)
  {
    CenterViewport(dispatcher,
                   viewportInfo,
                   canvas.size,
                   info.row_count,
                   info.col_count);
    center_viewport = false;
  }

  // TODO ShowGrid(state, canvas, IM_COL32(200, 200, 200, 15));

  RenderMap(document, info);

  const auto cursor = GetViewportCursorInfo(info);
  if (cursor.is_within_map)
  {
    RenderCursorGizmos(model, document, dispatcher, cursor, info);
  }

  drawList->PopClipRect();

  MapContentOverlay(document, canvas, cursor);
}

void CenterMapContentViewport()
{
  center_viewport = true;
}

}  // namespace Tactile
