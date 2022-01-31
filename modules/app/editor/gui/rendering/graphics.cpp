#include "graphics.hpp"

#include <algorithm>  // clamp
#include <cmath>      // sin, cos

#include <imgui_internal.h>

#include "assert.hpp"
#include "core/region.hpp"
#include "core/utils/colors.hpp"
#include "editor/gui/texture_utils.hpp"
#include "render_info.hpp"

namespace tactile {
namespace {

[[nodiscard]] auto ConvertBoundsToRect(const RenderInfo& info) -> cen::frect
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

void PathEllipticalArcTo(ImDrawList* self,
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

void AddEllipse(ImDrawList* self,
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
  PathEllipticalArcTo(self, center, radius, 0.0f, arcMax, nSegments - 1);

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

graphics_ctx::graphics_ctx(const RenderInfo& info)
    : mCanvasTL{info.canvas_tl}
    , mCanvasBR{info.canvas_br}
    , mOrigin{info.origin}
    , mViewportTileSize{info.grid_size}
    , mLogicalTileSize{info.tile_size}
    , mTileSizeRatio{info.ratio}
    , mBounds{info.bounds}
    , mBoundsRect{ConvertBoundsToRect(info)}
{}

void graphics_ctx::push_clip()
{
  auto* list = ImGui::GetWindowDrawList();
  list->PushClipRect(mCanvasTL, mCanvasBR, true);
}

void graphics_ctx::pop_clip()
{
  auto* list = ImGui::GetWindowDrawList();
  list->PopClipRect();
}

void graphics_ctx::clear()
{
  fill_rect(mCanvasTL, mCanvasBR - mCanvasTL);
}

void graphics_ctx::draw_rect(const ImVec2& position, const ImVec2& size)
{
  auto* list = ImGui::GetWindowDrawList();
  list->AddRect(position, position + size, get_draw_color(), 0, 0, mLineThickness);
}

void graphics_ctx::fill_rect(const ImVec2& position, const ImVec2& size)
{
  auto* list = ImGui::GetWindowDrawList();
  list->AddRectFilled(position, position + size, get_draw_color());
}

void graphics_ctx::draw_translated_rect(const ImVec2& position, const ImVec2& size)
{
  draw_rect(translate(position), size);
}

void graphics_ctx::fill_translated_rect(const ImVec2& position, const ImVec2& size)
{
  fill_rect(translate(position), size);
}

void graphics_ctx::draw_rect_with_shadow(const ImVec2& position, const ImVec2& size)
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

void graphics_ctx::draw_translated_rect_with_shadow(const ImVec2& position,
                                                    const ImVec2& size)
{
  draw_rect_with_shadow(translate(position), size);
}

void graphics_ctx::draw_circle_with_shadow(const ImVec2& center, const float radius)
{
  auto* list = ImGui::GetWindowDrawList();
  list->AddCircle(center + ImVec2{0, mLineThickness},
                  radius,
                  get_shadow_draw_color(),
                  0,
                  mLineThickness);
  list->AddCircle(center, radius, get_draw_color(), 0, mLineThickness);
}

void graphics_ctx::draw_translated_circle_with_shadow(const ImVec2& center,
                                                      const float radius)
{
  draw_circle_with_shadow(translate(center), radius);
}

void graphics_ctx::draw_ellipse_with_shadow(const ImVec2& center, const ImVec2& radius)
{
  constexpr int nSegments = 50;

  if (radius.x == radius.y) {
    draw_circle_with_shadow(center, radius.x);
  }
  else {
    auto* list = ImGui::GetWindowDrawList();
    AddEllipse(list,
               center + ImVec2{0, mLineThickness},
               radius,
               get_shadow_draw_color(),
               nSegments,
               mLineThickness);
    AddEllipse(list, center, radius, get_draw_color(), nSegments, mLineThickness);
  }
}

void graphics_ctx::draw_translated_ellipse_with_shadow(const ImVec2& center,
                                                       const ImVec2& radius)
{
  draw_ellipse_with_shadow(translate(center), radius);
}

void graphics_ctx::render_image(const uint texture,
                                const ImVec2& position,
                                const ImVec2& size)
{
  auto* list = ImGui::GetWindowDrawList();
  list->AddImage(ToTextureID(texture), position, position + size);
}

void graphics_ctx::render_image(const uint texture,
                                const ImVec4& source,
                                const ImVec2& position,
                                const ImVec2& uv)
{
  const auto row = source.y / source.w;
  const auto col = source.x / source.z;

  const auto uvMin = ImVec2{col * uv.x, row * uv.y};
  const auto uvMax = uvMin + ImVec2{uv.x, uv.y};

  auto* list = ImGui::GetWindowDrawList();
  list->AddImage(ToTextureID(texture),
                 position,
                 position + mViewportTileSize,
                 uvMin,
                 uvMax,
                 color_to_u32(cen::colors::white.with_alpha(mOpacity)));
}

void graphics_ctx::render_translated_image(const uint texture,
                                           const ImVec4& source,
                                           const ImVec2& position,
                                           const ImVec2& uv)
{
  render_image(texture, source, translate(position), uv);
}

void graphics_ctx::render_text(const c_str text, const ImVec2& position)
{
  TACTILE_ASSERT(text);
  ImGui::GetWindowDrawList()->AddText(position, get_draw_color(), text);
}

void graphics_ctx::render_translated_text(const c_str text, const ImVec2& position)
{
  render_text(text, translate(position));
}

void graphics_ctx::render_centered_text(const c_str text, const ImVec2& center)
{
  TACTILE_ASSERT(text);

  const auto size = ImGui::CalcTextSize(text);

  const auto x = center.x - (size.x / 2.0f);
  const auto y = center.y + (size.y / 2.0f);

  ImGui::GetWindowDrawList()->AddText({x, y}, get_draw_color(), text);
}

void graphics_ctx::render_translated_grid()
{
  const auto endRow = mBounds.end.row();
  const auto endCol = mBounds.end.col();

  for (auto row = mBounds.begin.row(); row < endRow; ++row) {
    for (auto col = mBounds.begin.col(); col < endCol; ++col) {
      draw_translated_rect(from_matrix_to_absolute(row, col), mViewportTileSize);
    }
  }
}

void graphics_ctx::set_draw_color(const cen::color& color)
{
  mDrawColor = color;
}

void graphics_ctx::set_opacity(const float opacity)
{
  mOpacity = static_cast<uint8>(255.0f * std::clamp(opacity, 0.0f, 1.0f));
}

void graphics_ctx::set_line_thickness(const float thickness)
{
  mLineThickness = std::clamp(thickness, 0.0f, thickness);
}

auto graphics_ctx::from_matrix_to_absolute(const int32 row, const int32 column) const
    -> ImVec2
{
  return mViewportTileSize * ImVec2{static_cast<float>(column), static_cast<float>(row)};
}

auto graphics_ctx::is_intersecting_bounds(const ImVec2& position,
                                          const ImVec2& size) const -> bool
{
  const auto translated = translate(position);
  const cen::frect rect{translated.x, translated.y, size.x, size.y};
  return cen::intersects(mBoundsRect, rect);
}

auto graphics_ctx::is_within_translated_bounds(const ImVec2& position) const -> bool
{
  const auto translated = translate(position);
  return mBoundsRect.contains({translated.x, translated.y});
}

auto graphics_ctx::translate(const ImVec2& position) const -> ImVec2
{
  return mOrigin + position;
}

auto graphics_ctx::get_draw_color() const -> uint32
{
  return color_to_u32(mDrawColor);
}

auto graphics_ctx::get_shadow_draw_color() const -> uint32
{
  return color_to_u32(cen::colors::black.with_alpha(mOpacity));
}

}  // namespace tactile