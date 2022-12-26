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

#include "graphics.hpp"

#include <cmath>  // fmod

#include <imgui_internal.h>

#include "core/region.hpp"
#include "core/tile/tileset.hpp"
#include "render_info.hpp"
#include "ui/conversions.hpp"
#include "ui/style/colors.hpp"
#include "ui/textures.hpp"

namespace tactile::ui {
namespace {

[[nodiscard]] auto convert_bounds_to_rect(const RenderInfo& info) -> cen::frect
{
  const auto pos = info.origin + (from_pos(info.bounds.begin) * info.grid_size);
  const auto size = from_pos(info.bounds.end - info.bounds.begin) * info.grid_size;
  return {pos.x, pos.y, size.x, size.y};
}

}  // namespace

Graphics::Graphics(const RenderInfo& info)
    : mInfo {info},
      mBoundsRect {convert_bounds_to_rect(info)}
{
}

void Graphics::push_canvas_clip() const
{
  auto* list = ImGui::GetWindowDrawList();
  list->PushClipRect(mInfo.canvas_tl, mInfo.canvas_br, true);
}

void Graphics::pop_clip() const
{
  auto* list = ImGui::GetWindowDrawList();
  list->PopClipRect();
}

void Graphics::clear(const uint32 color) const
{
  fill_rect(mInfo.canvas_tl, mInfo.canvas_size, color);
}

void Graphics::render_tile(const Tileset& tileset,
                           const TilePos& tile_position,
                           const ImVec2& position,
                           const uint8 opacity) const
{
  const auto& uv_size = tileset.uv_size();

  const auto uv_min = tile_position.as_vec2f() * uv_size;
  const auto uv_max = uv_min + uv_size;

  render_image(tileset.texture(),
               position,
               mInfo.grid_size,
               from_vec(uv_min),
               from_vec(uv_max),
               opacity);
}

void Graphics::render_translated_grid(const uint32 color)
{
  const auto end_row = mInfo.bounds.end.row();
  const auto end_col = mInfo.bounds.end.col();

  //  invoke(ex_range(mInfo.bounds.begin.row(), mInfo.bounds.end.row()),
  //         ex_range(mInfo.bounds.begin.col(), mInfo.bounds.end.col()),
  //         [=, this](int32 row, int32 col) {
  //           draw_translated_rect(from_matrix_to_absolute(row, col),
  //                                mInfo.grid_size,
  //                                u32_color);
  //         });

  for (auto row = mInfo.bounds.begin.row(); row < end_row; ++row) {
    for (auto col = mInfo.bounds.begin.col(); col < end_col; ++col) {
      draw_translated_rect(from_matrix_to_absolute(TilePos {row, col}),
                           mInfo.grid_size,
                           color);
    }
  }
}

void Graphics::render_infinite_grid(const uint32 color)
{
  const auto origin_tile_pos = ImFloor(mInfo.canvas_tl / mInfo.grid_size);
  const auto origin_col = static_cast<int32>(origin_tile_pos.x);
  const auto origin_row = static_cast<int32>(origin_tile_pos.y);

  const auto begin_row = origin_row - 1;
  const auto begin_col = origin_col - 1;
  const auto end_row = origin_row + mInfo.tiles_in_viewport_y + 1;
  const auto end_col = origin_col + mInfo.tiles_in_viewport_x + 1;

  // This offset ensures that the rendered grid is aligned over the underlying grid
  const ImVec2 offset {std::fmod(mInfo.origin.x, mInfo.grid_size.x),
                       std::fmod(mInfo.origin.y, mInfo.grid_size.y)};

  const auto end_x = (static_cast<float>(end_col) * mInfo.grid_size.x) + offset.x;
  const auto end_y = (static_cast<float>(end_row) * mInfo.grid_size.y) + offset.y;

  auto* list = ImGui::GetWindowDrawList();
  for (auto row = begin_row; row < end_row; ++row) {
    const auto row_y = (static_cast<float>(row) * mInfo.grid_size.y) + offset.y;
    list->AddLine(ImVec2 {0, row_y}, ImVec2 {end_x, row_y}, color);
  }

  for (auto col = begin_col; col < end_col; ++col) {
    const auto col_x = (static_cast<float>(col) * mInfo.grid_size.x) + offset.x;
    list->AddLine(ImVec2 {col_x, 0}, ImVec2 {col_x, end_y}, color);
  }
}

void Graphics::outline_contents(const uint32 color) const
{
  draw_rect(mInfo.origin, mInfo.contents_size, color);
}

auto Graphics::from_matrix_to_absolute(const TilePos pos) const -> ImVec2
{
  return mInfo.grid_size * from_vec(pos.as_vec2f());
}

auto Graphics::is_intersecting_bounds(const ImVec2 position, const ImVec2 size) const
    -> bool
{
  const auto translated = translate(position);
  const cen::frect rect {translated.x, translated.y, size.x, size.y};
  return cen::intersects(mBoundsRect, rect);
}

auto Graphics::is_within_translated_bounds(const ImVec2 position) const -> bool
{
  const auto translated = translate(position);
  return mBoundsRect.contains({translated.x, translated.y});
}

}  // namespace tactile::ui