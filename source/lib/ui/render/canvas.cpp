/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
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

#include "canvas.hpp"

#include <algorithm>  // min, max
#include <cmath>      // trunc

#include <imgui.h>
#include <imgui_internal.h>

#include "model/events/viewport_events.hpp"
#include "ui/conversions.hpp"

namespace tactile::ui {
namespace {

[[nodiscard]] auto _convert_bounds_to_rect(const CanvasInfo& canvas) -> ImVec4
{
  const auto bounds_offset = canvas.bounds.begin.as_vec2f();
  const auto bounds_size = (canvas.bounds.end - canvas.bounds.begin).as_vec2f();

  const auto pos =
      canvas.origin_pos + (as_imvec2(bounds_offset) * canvas.graphical_tile_size);
  const auto size = as_imvec2(bounds_size) * canvas.graphical_tile_size;

  return {pos.x, pos.y, size.x, size.y};
}

[[nodiscard]] auto _determine_canvas_render_bounds(const CanvasInfo& canvas) -> Region
{
  const auto begin = (canvas.top_left - canvas.origin_pos) / canvas.graphical_tile_size;
  const auto end = (canvas.bottom_right - canvas.origin_pos) / canvas.graphical_tile_size;

  const auto begin_row = std::max(0, static_cast<int32>(begin.y));
  const auto begin_col = std::max(0, static_cast<int32>(begin.x));

  const auto end_row = static_cast<int32>(std::min(canvas.row_count, end.y + 1));
  const auto end_col = static_cast<int32>(std::min(canvas.col_count, end.x + 1));

  Region bounds;

  bounds.begin = {begin_row, begin_col};
  bounds.end = {end_row, end_col};

  return bounds;
}

}  // namespace

auto CanvasInfo::contains(const ImVec2& pos) const -> bool
{
  const auto bounds_max_x = bounds_rect.x + bounds_rect.z;
  const auto bounds_max_y = bounds_rect.y + bounds_rect.w;

  return bounds_rect.x < pos.x &&  //
         bounds_rect.y < pos.y &&  //
         pos.x < bounds_max_x &&   //
         pos.y < bounds_max_y;
}

auto CanvasInfo::intersects(const ImVec4& rect) const -> bool
{
  const ImVec2 rect_pos {rect.x, rect.y};
  const ImVec2 rect_size = {rect.z, rect.w};

  const auto bounds_max_x = bounds_rect.x + bounds_rect.z;
  const auto bounds_max_y = bounds_rect.y + bounds_rect.w;

  const auto rect_max_x = rect_pos.x + rect_size.x;
  const auto rect_max_y = rect_pos.y + rect_size.y;

  return bounds_rect.x < rect_max_x &&  //
         bounds_rect.y < rect_max_y &&  //
         rect_pos.x < bounds_max_x &&   //
         rect_pos.y < bounds_max_y;
}

void update_dynamic_viewport_info(const Entity viewport_entity,
                                  const CanvasInfo& canvas,
                                  Dispatcher& dispatcher)
{
  dispatcher.enqueue(SetDynamicViewportInfoEvent {
      .viewport = viewport_entity,
      .info =
          {
              .total_size = as_float2(canvas.size),
              .content_size = as_float2(canvas.contents_size),
          },
  });
}

auto create_canvas_info(const Viewport& viewport,
                        const Float2& logical_tile_size,
                        const TileExtent& extent) -> CanvasInfo
{
  CanvasInfo canvas;

  canvas.top_left = ImGui::GetCursorScreenPos();
  canvas.bottom_right = canvas.top_left + ImGui::GetContentRegionAvail();
  canvas.size = canvas.bottom_right - canvas.top_left;

  canvas.origin_pos = canvas.top_left + as_imvec2(viewport.offset);

  canvas.logical_tile_size = as_imvec2(logical_tile_size);
  canvas.graphical_tile_size = as_imvec2(viewport.tile_size);
  canvas.tile_ratio = canvas.graphical_tile_size / canvas.logical_tile_size;

  const auto tiles_in_viewport = canvas.size / canvas.graphical_tile_size;
  canvas.tiles_in_viewport_x = static_cast<int32>(tiles_in_viewport.x) + 1;
  canvas.tiles_in_viewport_y = static_cast<int32>(tiles_in_viewport.y) + 1;

  canvas.row_count = static_cast<float>(extent.rows);
  canvas.col_count = static_cast<float>(extent.cols);
  canvas.contents_size =
      ImVec2 {canvas.col_count, canvas.row_count} * canvas.graphical_tile_size;

  canvas.bounds = _determine_canvas_render_bounds(canvas);
  canvas.bounds_rect = _convert_bounds_to_rect(canvas);

  return canvas;
}

auto get_viewport_mouse_info(const CanvasInfo& canvas) -> ViewportMouseInfo
{
  ViewportMouseInfo cursor;

  cursor.raw_pos = as_float2(ImGui::GetMousePos() - canvas.origin_pos);
  cursor.scaled_pos = cursor.raw_pos / as_float2(canvas.tile_ratio);

  const auto index = cursor.raw_pos / as_float2(canvas.graphical_tile_size);
  const auto row = std::trunc(index.y);
  const auto col = std::trunc(index.x);

  cursor.over_content = index.y >= 0 &&            //
                        index.x >= 0 &&            //
                        row < canvas.row_count &&  //
                        col < canvas.col_count;

  if (cursor.over_content) {
    cursor.tile_pos = {static_cast<int32>(row), static_cast<int32>(col)};
  }

  cursor.clamped_pos = {canvas.origin_pos.x + (col * canvas.graphical_tile_size.x),
                        canvas.origin_pos.y + (row * canvas.graphical_tile_size.y)};

  return cursor;
}

}  // namespace tactile::ui
