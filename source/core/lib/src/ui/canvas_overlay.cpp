// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/ui/canvas_overlay.hpp"

#include <imgui.h>

#include "tactile/base/container/buffer.hpp"
#include "tactile/base/numeric/vec.hpp"
#include "tactile/base/numeric/vec_common.hpp"
#include "tactile/base/numeric/vec_format.hpp"
#include "tactile/base/util/format.hpp"
#include "tactile/core/ui/canvas_renderer.hpp"
#include "tactile/core/ui/common/text.hpp"
#include "tactile/core/ui/imgui_compat.hpp"
#include "tactile/core/ui/viewport.hpp"

namespace tactile::core::ui {

void push_viewport_info_section(const CViewport& viewport)
{
  ImGui::SeparatorText("Viewport");
  push_formatted_text<64>("Position: {}", viewport.pos);
  push_formatted_text<64>("Size: {}", viewport.size);
  push_formatted_text<64>("Scale: {:.2f}", viewport.scale);
}

void push_canvas_info_section(const CanvasRenderer& canvas_renderer)
{
  const auto& visible_tiles = canvas_renderer.get_visible_tiles();
  const auto& render_bounds = canvas_renderer.get_render_bounds();

  const auto tile_count = (render_bounds.end.x - render_bounds.begin.x) *
                          (render_bounds.end.y - render_bounds.begin.y);

  ImGui::SeparatorText("Canvas");
  push_formatted_text<80>("Range: {} to {}", visible_tiles.begin, visible_tiles.end);
  push_formatted_text<80>("Render bounds: {} to {}", render_bounds.begin, render_bounds.end);
  push_formatted_text<64>("Tile count: {}", tile_count);
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
    push_formatted_text<64>("Position: {}", local_mouse_world_pos);

    const auto local_mouse_tile =
        vec_cast<Int2>(floor(local_mouse_world_pos / canvas_tile_size));

    if (const auto mouse_index = Index2D::from_vec(local_mouse_tile)) {
      const auto& render_bounds = canvas_renderer.get_render_bounds();
      if (mouse_index->x < render_bounds.end.x && mouse_index->y < render_bounds.end.y) {
        push_formatted_text<64>("Index: {}", *mouse_index);
      }
      else {
        ImGui::TextUnformatted("Index: -");
      }
    }
  }
  else {
    ImGui::TextUnformatted("Position: -");
    ImGui::TextUnformatted("Index: -");
  }
}

}  // namespace tactile::core::ui
