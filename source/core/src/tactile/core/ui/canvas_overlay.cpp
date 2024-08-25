// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/ui/canvas_overlay.hpp"

#include <imgui.h>

#include "tactile/base/numeric/vec.hpp"
#include "tactile/base/numeric/vec_common.hpp"
#include "tactile/core/ui/canvas_renderer.hpp"
#include "tactile/core/ui/imgui_compat.hpp"
#include "tactile/core/ui/viewport.hpp"

namespace tactile::ui {

void push_viewport_info_section(const CViewport& viewport)
{
  ImGui::SeparatorText("Viewport");
  ImGui::Text("Position: (%.2f, %.2f)", viewport.pos.x(), viewport.pos.y());
  ImGui::Text("Size: (%.2f, %.2f)", viewport.size.x(), viewport.size.y());
  ImGui::Text("Scale: %.2f", viewport.scale);
}

void push_canvas_info_section(const CanvasRenderer& canvas_renderer)
{
  const auto& visible_tiles = canvas_renderer.get_visible_tiles();
  const auto& render_bounds = canvas_renderer.get_render_bounds();

  const auto tile_count = (render_bounds.end.col - render_bounds.begin.col) *
                          (render_bounds.end.row - render_bounds.begin.row);

  ImGui::SeparatorText("Canvas");
  ImGui::Text("Range: (%d:%d) to (%d:%d)",
              visible_tiles.begin.row,
              visible_tiles.begin.col,
              visible_tiles.end.row,
              visible_tiles.end.col);
  ImGui::Text("Render bounds: (%d:%d) to (%d:%d)",
              render_bounds.begin.row,
              render_bounds.begin.col,
              render_bounds.end.row,
              render_bounds.end.col);
  ImGui::Text("Tile count: %d", tile_count);
}

void push_viewport_mouse_info_section(const CanvasRenderer& canvas_renderer)
{
  const auto canvas_pos = canvas_renderer.get_canvas_position();
  const auto canvas_size = canvas_renderer.get_canvas_size();
  const auto canvas_tile_size = canvas_renderer.get_canvas_tile_size();

  const auto local_mouse_pos = to_float2(ImGui::GetMousePos()) - canvas_pos;
  const auto local_mouse_world_pos = canvas_renderer.to_world_pos(local_mouse_pos);

  ImGui::SeparatorText("Mouse");
  if (local_mouse_pos.x() >= 0.0f &&             //
      local_mouse_pos.y() >= 0.0f &&             //
      local_mouse_pos.x() <= canvas_size.x() &&  //
      local_mouse_pos.y() <= canvas_size.y()) {
    const auto local_mouse_tile =
        vec_cast<Int2>(floor(local_mouse_world_pos / canvas_tile_size));

    ImGui::Text("Position: (%.2f, %.2f)",
                local_mouse_world_pos.x(),
                local_mouse_world_pos.y());
    ImGui::Text("Index: (%d:%d)", local_mouse_tile.x(), local_mouse_tile.y());
  }
  else {
    ImGui::TextUnformatted("Position: -");
    ImGui::TextUnformatted("Index: -");
  }
}

}  // namespace tactile::ui
