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

#include <imgui_internal.h>

#include "core/region.hpp"
#include "editor/ui/conversions.hpp"
#include "editor/ui/render/render.hpp"
#include "editor/ui/style/colors.hpp"
#include "editor/ui/textures.hpp"
#include "misc/assert.hpp"
#include "render_info.hpp"

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

void Graphics::push_clip()
{
  auto* list = ImGui::GetWindowDrawList();
  list->PushClipRect(mInfo.canvas_tl, mInfo.canvas_br, true);
}

void Graphics::pop_clip()
{
  auto* list = ImGui::GetWindowDrawList();
  list->PopClipRect();
}

void Graphics::clear(const uint32 color)
{
  ui::fill_rect(mInfo.canvas_tl, mInfo.canvas_br - mInfo.canvas_tl, color);
}

void Graphics::draw_translated_rect(const ImVec2& position,
                                    const ImVec2& size,
                                    const uint32 color,
                                    const float thickness)
{
  ui::draw_rect(translate(position), size, color, thickness);
}

void Graphics::draw_translated_rect(const ImVec2& position,
                                    const ImVec2& size,
                                    const cen::color& color,
                                    const float thickness)
{
  const auto col32 = IM_COL32(color.red(), color.green(), color.blue(), color.alpha());
  draw_translated_rect(position, size, col32, thickness);
}

void Graphics::fill_translated_rect(const ImVec2& position,
                                    const ImVec2& size,
                                    const uint32 color)
{
  fill_rect(translate(position), size, color);
}

void Graphics::draw_rect_with_shadow(const ImVec2& position,
                                     const ImVec2& size,
                                     const uint32 color,
                                     const float thickness)
{
  auto* list = ImGui::GetWindowDrawList();

  const auto offset_position = position + ImVec2 {thickness, thickness};
  list->AddRect(offset_position, offset_position + size, IM_COL32_BLACK, 0, 0, thickness);
  list->AddRect(position, position + size, color, 0, 0, thickness);
}

void Graphics::draw_translated_rect_with_shadow(const ImVec2& position,
                                                const ImVec2& size,
                                                const uint32 color,
                                                const float thickness)
{
  draw_rect_with_shadow(translate(position), size, color, thickness);
}

void Graphics::draw_translated_circle_with_shadow(const ImVec2& center,
                                                  const float radius,
                                                  const uint32 color,
                                                  const float thickness)
{
  ui::draw_shadowed_circle(translate(center), radius, color, thickness);
}

void Graphics::draw_translated_shadowed_ellipse(const Float2& center,
                                                const Float2& radius,
                                                const cen::color& color,
                                                float thickness)
{
  ui::draw_shadowed_ellipse(translate(center), radius, color, thickness);
}

void Graphics::draw_translated_ellipse_with_shadow(const ImVec2& center,
                                                   const ImVec2& radius,
                                                   const uint32 color,
                                                   const float thickness)
{
  ui::draw_shadowed_ellipse(translate(center), radius, color, thickness);
}

void Graphics::render_image(const uint texture,
                            const ImVec2& position,
                            const ImVec2& size)
{
  auto* list = ImGui::GetWindowDrawList();
  list->AddImage(to_texture_id(texture), position, position + size);
}

void Graphics::render_image(const uint texture,
                            const Float2& pos,
                            const Float2& size,
                            const Float2& uv_min,
                            const Float2& uv_max,
                            const uint8 opacity)
{
  auto* list = ImGui::GetWindowDrawList();

  const auto im_min = from_vec(pos);
  const auto im_max = im_min + from_vec(size);

  list->AddImage(to_texture_id(texture),
                 im_min,
                 im_max,
                 from_vec(uv_min),
                 from_vec(uv_max),
                 IM_COL32(0xFF, 0xFF, 0xFF, opacity));
}

void Graphics::render_image(const uint texture,
                            const ImVec4& source,
                            const ImVec2& position,
                            const ImVec2& uv,
                            const float opacity)
{
  const ImVec2 index {source.x / source.z, source.y / source.w};

  const auto uv_min = index * uv;
  const auto uv_max = uv_min + uv;

  auto* list = ImGui::GetWindowDrawList();
  list->AddImage(to_texture_id(texture),
                 position,
                 position + mInfo.grid_size,
                 uv_min,
                 uv_max,
                 IM_COL32(0xFF, 0xFF, 0xFF, static_cast<uint8>(255 * opacity)));
}

void Graphics::render_translated_image(const uint texture,
                                       const ImVec4& source,
                                       const ImVec2& position,
                                       const ImVec2& uv,
                                       const float opacity)
{
  render_image(texture, source, translate(position), uv, opacity);
}

void Graphics::render_text(const char* text, const ImVec2& position, const uint32 color)
{
  TACTILE_ASSERT(text);
  ImGui::GetWindowDrawList()->AddText(position, color, text);
}

void Graphics::render_translated_text(const char* text,
                                      const ImVec2& position,
                                      const uint32 color)
{
  render_text(text, translate(position), color);
}

void Graphics::render_centered_text(const char* text,
                                    const ImVec2& center,
                                    const uint32 color)
{
  TACTILE_ASSERT(text);

  const auto size = ImGui::CalcTextSize(text);

  const auto x = center.x - (size.x / 2.0f);
  const auto y = center.y + (size.y / 2.0f);

  ImGui::GetWindowDrawList()->AddText({x, y}, color, text);
}

void Graphics::render_translated_grid(const uint32 color)
{
  const auto end_row = mInfo.bounds.end.row();
  const auto end_col = mInfo.bounds.end.col();

  for (auto row = mInfo.bounds.begin.row(); row < end_row; ++row) {
    for (auto col = mInfo.bounds.begin.col(); col < end_col; ++col) {
      draw_translated_rect(from_matrix_to_absolute(row, col), mInfo.grid_size, color);
    }
  }
}

void Graphics::render_translated_grid(const cen::color& color)
{
  const auto col32 = IM_COL32(color.red(), color.green(), color.blue(), color.alpha());
  render_translated_grid(col32);
}

void Graphics::render_infinite_grid(const cen::color& color)
{
  const auto origin_tile_pos = ImFloor(mInfo.origin / mInfo.grid_size);
  const auto col_offset = static_cast<int32>(origin_tile_pos.x);
  const auto row_offset = static_cast<int32>(origin_tile_pos.y);

  const auto begin_row = -(row_offset + 1);
  const auto begin_col = -(col_offset + 1);
  const auto end_row = (mInfo.tiles_in_viewport_y - row_offset) + 3;  // A little extra
  const auto end_col = (mInfo.tiles_in_viewport_x - col_offset) + 1;

  for (auto row = begin_row; row < end_row; ++row) {
    for (auto col = begin_col; col < end_col; ++col) {
      const auto pos = from_matrix_to_absolute(row, col);
      draw_translated_rect(pos, mInfo.grid_size, color);
    }
  }
}

void Graphics::outline_contents(const cen::color& color)
{
  const auto col32 = IM_COL32(color.red(), color.green(), color.blue(), color.alpha());
  ui::draw_rect(mInfo.origin, mInfo.contents_size, col32);
}

auto Graphics::from_matrix_to_absolute(const int32 row, const int32 column) const
    -> ImVec2
{
  return mInfo.grid_size * ImVec2 {static_cast<float>(column), static_cast<float>(row)};
}

auto Graphics::is_intersecting_bounds(const ImVec2& position, const ImVec2& size) const
    -> bool
{
  const auto translated = translate(position);
  const cen::frect rect {translated.x, translated.y, size.x, size.y};
  return cen::intersects(mBoundsRect, rect);
}

auto Graphics::is_within_translated_bounds(const ImVec2& position) const -> bool
{
  const auto translated = translate(position);
  return mBoundsRect.contains({translated.x, translated.y});
}

auto Graphics::translate(const Float2& position) const -> Float2
{
  return to_vec(mInfo.origin) + position;
}

auto Graphics::translate(const ImVec2& position) const -> ImVec2
{
  return mInfo.origin + position;
}

}  // namespace tactile::ui