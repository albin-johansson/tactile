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

#include <algorithm>  // clamp
#include <cmath>      // sin, cos

#include <imgui_internal.h>

#include "core/region.hpp"
#include "editor/gui/common/colors.hpp"
#include "editor/gui/textures.hpp"
#include "misc/assert.hpp"
#include "render_info.hpp"

namespace tactile {
namespace {

[[nodiscard]] auto _convert_bounds_to_rect(const RenderInfo& info) -> cen::frect
{
  const auto begin = info.bounds.begin;

  const auto gridWidth = info.grid_size.x;
  const auto gridHeight = info.grid_size.y;

  const ImVec2 index{static_cast<float>(begin.col()), static_cast<float>(begin.row())};
  const auto pos = info.origin + (index * info.grid_size);

  const auto size = info.bounds.end - info.bounds.begin;
  const auto width = static_cast<float>(size.col()) * gridWidth;
  const auto height = static_cast<float>(size.row()) * gridHeight;

  return {pos.x, pos.y, width, height};
}

void _path_elliptical_arc_to(ImDrawList* self,
                             const ImVec2& center,
                             const ImVec2& radius,
                             const float arcMin,
                             const float arcMax,
                             const int nSegments = 10)
{
  for (int i = 0; i <= nSegments; ++i) {
    const auto diff = arcMax - arcMin;
    const float a =
        arcMin + (static_cast<float>(i) / static_cast<float>(nSegments)) * diff;

    const ImVec2 pos{center.x + std::cos(a) * radius.x,
                     center.y + std::sin(a) * radius.y};
    self->PathLineTo(pos);
  }
}

void _add_ellipse(ImDrawList* self,
                  const ImVec2& center,
                  const ImVec2& radius,
                  const ImU32 color,
                  const int nSegments = 12,
                  const float thickness = 1.0f)
{
  if ((color & IM_COL32_A_MASK) == 0 || nSegments <= 2) {
    return;
  }

  // Because we are filling a closed shape we remove 1 from the count of segments/points
  const float arcMax = IM_PI * 2.0f * (static_cast<float>(nSegments) - 1.0f) /
                       static_cast<float>(nSegments);
  _path_elliptical_arc_to(self, center, radius, 0.0f, arcMax, nSegments - 1);

  self->PathStroke(color, true, thickness);
}

/*
void AddFilledEllipse(ImDrawList* self,
                      const ImVec2& center,
                      const ImVec2& radius,
                      const ImU32 color,
                      const int nSegments = 12)
{
  if ((color & IM_COL32_A_MASK) == 0 || nSegments <= 2) {
    return;
  }

  // Because we are filling a closed shape we remove 1 from the count of segments/points
  const float arcMax = IM_PI * 2.0f * (static_cast<float>(nSegments) - 1.0f) /
                       static_cast<float>(nSegments);
  PathEllipticalArcTo(self, center, radius, 0.0f, arcMax, nSegments - 1);

  self->PathFillConvex(color);
}
*/

}  // namespace

GraphicsCtx::GraphicsCtx(const RenderInfo& info)
    : mCanvasTL{info.canvas_tl}
    , mCanvasBR{info.canvas_br}
    , mOrigin{info.origin}
    , mViewportTileSize{info.grid_size}
    , mLogicalTileSize{info.tile_size}
    , mTileSizeRatio{info.ratio}
    , mBounds{info.bounds}
    , mBoundsRect{_convert_bounds_to_rect(info)}
{}

void GraphicsCtx::push_clip()
{
  auto* list = ImGui::GetWindowDrawList();
  list->PushClipRect(mCanvasTL, mCanvasBR, true);
}

void GraphicsCtx::pop_clip()
{
  auto* list = ImGui::GetWindowDrawList();
  list->PopClipRect();
}

void GraphicsCtx::clear()
{
  fill_rect(mCanvasTL, mCanvasBR - mCanvasTL);
}

void GraphicsCtx::draw_rect(const ImVec2& position, const ImVec2& size)
{
  auto* list = ImGui::GetWindowDrawList();
  list->AddRect(position, position + size, get_draw_color(), 0, 0, mLineThickness);
}

void GraphicsCtx::draw_rect(const Vector2f& pos,
                            const Vector2f& size,
                            const cen::color& color,
                            const float thickness)
{
  auto* list = ImGui::GetWindowDrawList();

  const auto imMin = ImVec2{pos.x, pos.y};
  const auto imMax = imMin + ImVec2{size.x, size.y};

  list->AddRect(imMin, imMax, color_to_u32(color), 0.0f, 0, thickness);
}

void GraphicsCtx::fill_rect(const ImVec2& position, const ImVec2& size)
{
  auto* list = ImGui::GetWindowDrawList();
  list->AddRectFilled(position, position + size, get_draw_color());
}

void GraphicsCtx::draw_ellipse(const Vector2f& center,
                               const Vector2f& radius,
                               const cen::color& color,
                               const float thickness)
{
  const ImVec2 imCenter{center.x, center.y};
  const ImVec2 imRadius{radius.x, radius.y};

  set_draw_color(color);          // TODO remove
  set_line_thickness(thickness);  // TODO remove
  draw_translated_ellipse_with_shadow(imCenter, imRadius);
}

void GraphicsCtx::draw_translated_rect(const ImVec2& position, const ImVec2& size)
{
  draw_rect(translate(position), size);
}

void GraphicsCtx::fill_translated_rect(const ImVec2& position, const ImVec2& size)
{
  fill_rect(translate(position), size);
}

void GraphicsCtx::draw_rect_with_shadow(const ImVec2& position, const ImVec2& size)
{
  auto* list = ImGui::GetWindowDrawList();

  const auto offsetPosition = position + ImVec2{mLineThickness, mLineThickness};
  list->AddRect(offsetPosition,
                offsetPosition + size,
                get_shadow_draw_color(),
                0,
                0,
                mLineThickness);
  list->AddRect(position, position + size, get_draw_color(), 0, 0, mLineThickness);
}

void GraphicsCtx::draw_translated_rect_with_shadow(const ImVec2& position,
                                                   const ImVec2& size)
{
  draw_rect_with_shadow(translate(position), size);
}

void GraphicsCtx::draw_circle_with_shadow(const ImVec2& center, const float radius)
{
  auto* list = ImGui::GetWindowDrawList();
  list->AddCircle(center + ImVec2{0, mLineThickness},
                  radius,
                  get_shadow_draw_color(),
                  0,
                  mLineThickness);
  list->AddCircle(center, radius, get_draw_color(), 0, mLineThickness);
}

void GraphicsCtx::draw_translated_circle_with_shadow(const ImVec2& center,
                                                     const float radius)
{
  draw_circle_with_shadow(translate(center), radius);
}

void GraphicsCtx::draw_ellipse_with_shadow(const ImVec2& center, const ImVec2& radius)
{
  constexpr int nSegments = 50;

  if (radius.x == radius.y) {
    draw_circle_with_shadow(center, radius.x);
  }
  else {
    auto* list = ImGui::GetWindowDrawList();
    _add_ellipse(list,
                 center + ImVec2{0, mLineThickness},
                 radius,
                 get_shadow_draw_color(),
                 nSegments,
                 mLineThickness);
    _add_ellipse(list, center, radius, get_draw_color(), nSegments, mLineThickness);
  }
}

void GraphicsCtx::draw_translated_ellipse_with_shadow(const ImVec2& center,
                                                      const ImVec2& radius)
{
  draw_ellipse_with_shadow(translate(center), radius);
}

void GraphicsCtx::render_image(const uint texture,
                               const ImVec2& position,
                               const ImVec2& size)
{
  auto* list = ImGui::GetWindowDrawList();
  list->AddImage(to_texture_id(texture), position, position + size);
}

void GraphicsCtx::render_image(const uint texture,
                               const Vector2f& pos,
                               const Vector2f& size,
                               const Vector2f& uvMin,
                               const Vector2f& uvMax,
                               const uint8 opacity)
{
  auto* list = ImGui::GetWindowDrawList();

  const auto imMin = ImVec2{pos.x, pos.y};
  const auto imMax = imMin + ImVec2{size.x, size.y};

  const ImVec2 imUvMin{uvMin.x, uvMin.y};
  const ImVec2 imUvMax{uvMax.x, uvMax.y};

  list->AddImage(to_texture_id(texture),
                 imMin,
                 imMax,
                 imUvMin,
                 imUvMax,
                 IM_COL32(0xFF, 0xFF, 0xFF, opacity));
}

void GraphicsCtx::render_image(const uint texture,
                               const ImVec4& source,
                               const ImVec2& position,
                               const ImVec2& uv)
{
  const auto row = source.y / source.w;
  const auto col = source.x / source.z;

  const auto uvMin = ImVec2{col * uv.x, row * uv.y};
  const auto uvMax = uvMin + ImVec2{uv.x, uv.y};

  auto* list = ImGui::GetWindowDrawList();
  list->AddImage(to_texture_id(texture),
                 position,
                 position + mViewportTileSize,
                 uvMin,
                 uvMax,
                 color_to_u32(cen::colors::white.with_alpha(mOpacity)));
}

void GraphicsCtx::render_translated_image(const uint texture,
                                          const ImVec4& source,
                                          const ImVec2& position,
                                          const ImVec2& uv)
{
  render_image(texture, source, translate(position), uv);
}

void GraphicsCtx::render_text(const char* text, const ImVec2& position)
{
  TACTILE_ASSERT(text);
  ImGui::GetWindowDrawList()->AddText(position, get_draw_color(), text);
}

void GraphicsCtx::render_translated_text(const char* text, const ImVec2& position)
{
  render_text(text, translate(position));
}

void GraphicsCtx::render_centered_text(const char* text, const ImVec2& center)
{
  TACTILE_ASSERT(text);

  const auto size = ImGui::CalcTextSize(text);

  const auto x = center.x - (size.x / 2.0f);
  const auto y = center.y + (size.y / 2.0f);

  ImGui::GetWindowDrawList()->AddText({x, y}, get_draw_color(), text);
}

void GraphicsCtx::render_translated_grid()
{
  const auto endRow = mBounds.end.row();
  const auto endCol = mBounds.end.col();

  for (auto row = mBounds.begin.row(); row < endRow; ++row) {
    for (auto col = mBounds.begin.col(); col < endCol; ++col) {
      draw_translated_rect(from_matrix_to_absolute(row, col), mViewportTileSize);
    }
  }
}

void GraphicsCtx::set_draw_color(const cen::color& color)
{
  mDrawColor = color;
}

void GraphicsCtx::set_opacity(const float opacity)
{
  mOpacity = static_cast<uint8>(255.0f * std::clamp(opacity, 0.0f, 1.0f));
}

void GraphicsCtx::set_line_thickness(const float thickness)
{
  mLineThickness = std::clamp(thickness, 0.0f, thickness);
}

auto GraphicsCtx::from_matrix_to_absolute(const int32 row, const int32 column) const
    -> ImVec2
{
  return mViewportTileSize * ImVec2{static_cast<float>(column), static_cast<float>(row)};
}

auto GraphicsCtx::is_intersecting_bounds(const ImVec2& position, const ImVec2& size) const
    -> bool
{
  const auto translated = translate(position);
  const cen::frect rect{translated.x, translated.y, size.x, size.y};
  return cen::intersects(mBoundsRect, rect);
}

auto GraphicsCtx::is_within_translated_bounds(const ImVec2& position) const -> bool
{
  const auto translated = translate(position);
  return mBoundsRect.contains({translated.x, translated.y});
}

auto GraphicsCtx::translate(const ImVec2& position) const -> ImVec2
{
  return mOrigin + position;
}

auto GraphicsCtx::get_draw_color() const -> uint32
{
  return color_to_u32(mDrawColor);
}

auto GraphicsCtx::get_shadow_draw_color() const -> uint32
{
  return color_to_u32(cen::colors::black.with_alpha(mOpacity));
}

}  // namespace tactile