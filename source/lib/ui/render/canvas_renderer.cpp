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

#include "canvas_renderer.hpp"

#include <cmath>  // fmod

#include <imgui.h>
#include <imgui_internal.h>

#include "ui/render/primitives.hpp"

namespace tactile::ui {
namespace {

[[nodiscard]] auto _to_graphical_pos(const CanvasInfo& canvas, const TilePos& tile_pos)
    -> ImVec2
{
  const ImVec2 vec_pos {static_cast<float>(tile_pos.col()),
                        static_cast<float>(tile_pos.row())};
  return canvas.graphical_tile_size * vec_pos;
}

}  // namespace

void push_scissor(const CanvasInfo& canvas)
{
  auto* commands = ImGui::GetWindowDrawList();
  commands->PushClipRect(canvas.top_left, canvas.bottom_right, true);
}

void pop_scissor()
{
  auto* commands = ImGui::GetWindowDrawList();
  commands->PopClipRect();
}

void clear_canvas(const CanvasInfo& canvas, const UColor& color)
{
  fill_rect(canvas.top_left, canvas.size, color);
}

void render_infinite_grid(const CanvasInfo& canvas, const UColor& line_color)
{
  const auto cell_size = canvas.graphical_tile_size;
  const auto origin_tile_pos = ImFloor(canvas.top_left / cell_size);

  const auto origin_col = static_cast<int32>(origin_tile_pos.x);
  const auto origin_row = static_cast<int32>(origin_tile_pos.y);

  const auto begin_row = origin_row - 1;
  const auto begin_col = origin_col - 1;
  const auto end_row = origin_row + canvas.tiles_in_viewport_y + 1;
  const auto end_col = origin_col + canvas.tiles_in_viewport_x + 1;

  // This offset ensures that the rendered grid is aligned over the underlying grid
  const ImVec2 offset {std::fmod(canvas.origin_pos.x, cell_size.x),
                       std::fmod(canvas.origin_pos.y, cell_size.y)};

  const auto end_x = (static_cast<float>(end_col) * cell_size.x) + offset.x;
  const auto end_y = (static_cast<float>(end_row) * cell_size.y) + offset.y;

  auto* commands = ImGui::GetWindowDrawList();
  const auto u32_line_color = to_u32(line_color);

  for (auto row = begin_row; row < end_row; ++row) {
    const auto row_y = (static_cast<float>(row) * cell_size.y) + offset.y;
    commands->AddLine(ImVec2 {0, row_y}, ImVec2 {end_x, row_y}, u32_line_color);
  }

  for (auto col = begin_col; col < end_col; ++col) {
    const auto col_x = (static_cast<float>(col) * cell_size.x) + offset.x;
    commands->AddLine(ImVec2 {col_x, 0}, ImVec2 {col_x, end_y}, u32_line_color);
  }
}

void render_translated_grid(const CanvasInfo& canvas, const UColor& line_color)
{
  const auto begin_row = canvas.bounds.begin.row();
  const auto begin_col = canvas.bounds.begin.col();

  const auto end_row = canvas.bounds.end.row();
  const auto end_col = canvas.bounds.end.col();

  // TODO performance: draw lines as in render_infinite_grid
  for (auto row = begin_row; row < end_row; ++row) {
    for (auto col = begin_col; col < end_col; ++col) {
      const auto graphical_pos = _to_graphical_pos(canvas, TilePos {row, col});
      const auto translated_pos = translate_pos(canvas, graphical_pos);
      draw_rect(translated_pos, canvas.graphical_tile_size, line_color);
    }
  }
}

void render_outline(const CanvasInfo& canvas, const UColor& outline_color)
{
  draw_rect(canvas.origin_pos, canvas.contents_size, outline_color);
}

void render_tile_image(const CanvasInfo& canvas,
                       ImTextureID texture,
                       const Float2& uv_size,
                       const TilePos& pos_in_tileset,
                       const Float2& position,
                       const float opacity)
{
  const auto uv_min = pos_in_tileset.as_vec2f() * uv_size;
  const auto uv_max = uv_min + uv_size;

  render_image(texture,
               position,
               as_float2(canvas.graphical_tile_size),
               uv_min,
               uv_max,
               opacity_cast(opacity));
}

auto translate_tile_pos(const CanvasInfo& canvas, const TilePos& tile_pos) -> Float2
{
  return as_float2(translate_pos(canvas, _to_graphical_pos(canvas, tile_pos)));
}

auto translate_pos(const CanvasInfo& canvas, const ImVec2& position) -> ImVec2
{
  return canvas.origin_pos + position;
}

}  // namespace tactile::ui
