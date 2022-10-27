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

#include <cmath>  // sin, cos

#include <imgui_internal.h>

#include "core/region.hpp"
#include "editor/ui/conversions.hpp"
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

void path_elliptical_arc_to(ImDrawList* self,
                            const ImVec2& center,
                            const ImVec2& radius,
                            const float arc_min,
                            const float arc_max,
                            const int n_segments = 10)
{
  for (int i = 0; i <= n_segments; ++i) {
    const auto diff = arc_max - arc_min;
    const float a =
        arc_min + (static_cast<float>(i) / static_cast<float>(n_segments)) * diff;

    const ImVec2 pos {center.x + std::cos(a) * radius.x,
                      center.y + std::sin(a) * radius.y};
    self->PathLineTo(pos);
  }
}

void add_ellipse(ImDrawList* self,
                 const ImVec2& center,
                 const ImVec2& radius,
                 const ImU32 color,
                 const int n_segments = 12,
                 const float thickness = 1.0f)
{
  if ((color & IM_COL32_A_MASK) == 0 || n_segments <= 2) {
    return;
  }

  // Because we are filling a closed shape we remove 1 from the count of segments/points
  const float arcMax = IM_PI * 2.0f * (static_cast<float>(n_segments) - 1.0f) /
                       static_cast<float>(n_segments);
  path_elliptical_arc_to(self, center, radius, 0.0f, arcMax, n_segments - 1);

  self->PathStroke(color, true, thickness);
}

}  // namespace

Graphics::Graphics(const RenderInfo& info)
    : mInfo {info},
      mCanvasTL {info.canvas_tl},
      mCanvasBR {info.canvas_br},
      mOrigin {info.origin},
      mViewportTileSize {info.grid_size},
      mLogicalTileSize {info.tile_size},
      mTileSizeRatio {info.ratio},
      mBounds {info.bounds},
      mBoundsRect {convert_bounds_to_rect(info)}
{
}

void Graphics::push_clip()
{
  auto* list = ImGui::GetWindowDrawList();
  list->PushClipRect(mCanvasTL, mCanvasBR, true);
}

void Graphics::pop_clip()
{
  auto* list = ImGui::GetWindowDrawList();
  list->PopClipRect();
}

void Graphics::clear(const uint32 color)
{
  fill_rect(mCanvasTL, mCanvasBR - mCanvasTL, color);
}

void Graphics::draw_rect(const ImVec2& position,
                         const ImVec2& size,
                         const uint32 color,
                         const float thickness)
{
  auto* list = ImGui::GetWindowDrawList();
  list->AddRect(position, position + size, color, 0, 0, thickness);
}

void Graphics::draw_rect(const Float2& pos,
                         const Float2& size,
                         const uint32 color,
                         const float thickness)
{
  auto* list = ImGui::GetWindowDrawList();

  const auto im_min = from_vec(pos);
  const auto im_max = im_min + from_vec(size);

  list->AddRect(im_min, im_max, color, 0.0f, 0, thickness);
}

void Graphics::fill_rect(const ImVec2& position, const ImVec2& size, const uint32 color)
{
  auto* list = ImGui::GetWindowDrawList();
  list->AddRectFilled(position, position + size, color);
}

void Graphics::draw_ellipse(const Float2& center,
                            const Float2& radius,
                            const uint32 color,
                            const float thickness)
{
  const auto im_center = from_vec(center);
  const auto im_radius = from_vec(radius);

  draw_translated_ellipse_with_shadow(im_center, im_radius, color, thickness);
}

void Graphics::draw_translated_rect(const ImVec2& position,
                                    const ImVec2& size,
                                    const uint32 color,
                                    const float thickness)
{
  draw_rect(translate(position), size, color, thickness);
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

void Graphics::draw_circle_with_shadow(const ImVec2& center,
                                       const float radius,
                                       const uint32 color,
                                       const float thickness)
{
  auto* list = ImGui::GetWindowDrawList();
  list->AddCircle(center + ImVec2 {0, thickness}, radius, IM_COL32_BLACK, 0, thickness);
  list->AddCircle(center, radius, color, 0, thickness);
}

void Graphics::draw_translated_circle_with_shadow(const ImVec2& center,
                                                  const float radius,
                                                  const uint32 color,
                                                  const float thickness)
{
  draw_circle_with_shadow(translate(center), radius, color, thickness);
}

void Graphics::draw_ellipse_with_shadow(const ImVec2& center,
                                        const ImVec2& radius,
                                        const uint32 color,
                                        const float thickness)
{
  constexpr int n_segments = 50;

  if (radius.x == radius.y) {
    draw_circle_with_shadow(center, radius.x, color, thickness);
  }
  else {
    auto* list = ImGui::GetWindowDrawList();
    add_ellipse(list,
                center + ImVec2 {0, thickness},
                radius,
                IM_COL32_BLACK,
                n_segments,
                thickness);
    add_ellipse(list, center, radius, color, n_segments, thickness);
  }
}

void Graphics::draw_translated_ellipse_with_shadow(const ImVec2& center,
                                                   const ImVec2& radius,
                                                   const uint32 color,
                                                   const float thickness)
{
  draw_ellipse_with_shadow(translate(center), radius, color, thickness);
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
                 position + mViewportTileSize,
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
  const auto end_row = mBounds.end.row();
  const auto end_col = mBounds.end.col();

  for (auto row = mBounds.begin.row(); row < end_row; ++row) {
    for (auto col = mBounds.begin.col(); col < end_col; ++col) {
      draw_translated_rect(from_matrix_to_absolute(row, col), mViewportTileSize, color);
    }
  }
}

auto Graphics::from_matrix_to_absolute(const int32 row, const int32 column) const
    -> ImVec2
{
  return mViewportTileSize * ImVec2 {static_cast<float>(column), static_cast<float>(row)};
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

auto Graphics::translate(const ImVec2& position) const -> ImVec2
{
  return mOrigin + position;
}

}  // namespace tactile::ui