#include "graphics.hpp"

#include <algorithm>  // clamp
#include <cmath>      // sin, cos

#include <imgui_internal.h>

#include "assert.hpp"
#include "core/region.hpp"
#include "core/utils/color_utils.hpp"
#include "editor/gui/texture_utils.hpp"
#include "render_info.hpp"

namespace Tactile {
namespace {

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

}  // namespace

Graphics::Graphics(const RenderInfo& info)
    : mOrigin{info.map_position}
    , mViewportTileSize{info.grid_size}
    , mLogicalTileSize{info.tile_size}
    , mTileSizeRatio{info.ratio}
    , mBounds{info.bounds}
    , mBoundsRect{info.bounds_rect}
{}

void Graphics::DrawRect(const ImVec2& position, const ImVec2& size)
{
  auto* list = ImGui::GetWindowDrawList();
  list->AddRect(position, position + size, GetDrawColor(), 0, 0, mLineThickness);
}

void Graphics::DrawTranslatedRect(const ImVec2& position, const ImVec2& size)
{
  DrawRect(Translate(position), size);
}

void Graphics::DrawRectWithShadow(const ImVec2& position, const ImVec2& size)
{
  auto* list = ImGui::GetWindowDrawList();

  const auto offsetPosition = position + ImVec2{mLineThickness, mLineThickness};
  list->AddRect(offsetPosition,
                offsetPosition + size,
                GetShadowDrawColor(),
                0,
                0,
                mLineThickness);
  list->AddRect(position, position + size, GetDrawColor(), 0, 0, mLineThickness);
}

void Graphics::DrawTranslatedRectWithShadow(const ImVec2& position, const ImVec2& size)
{
  DrawRectWithShadow(Translate(position), size);
}

void Graphics::DrawCircleWithShadow(const ImVec2& center, const float radius)
{
  auto* list = ImGui::GetWindowDrawList();
  list->AddCircle(center + ImVec2{0, mLineThickness},
                  radius,
                  GetShadowDrawColor(),
                  0,
                  mLineThickness);
  list->AddCircle(center, radius, GetDrawColor(), 0, mLineThickness);
}

void Graphics::DrawTranslatedCircleWithShadow(const ImVec2& center, const float radius)
{
  DrawCircleWithShadow(Translate(center), radius);
}

void Graphics::DrawEllipseWithShadow(const ImVec2& center, const ImVec2& radius)
{
  constexpr int nSegments = 50;

  if (radius.x == radius.y) {
    DrawCircleWithShadow(center, radius.x);
  }
  else {
    auto* list = ImGui::GetWindowDrawList();
    AddEllipse(list,
               center + ImVec2{0, mLineThickness},
               radius,
               GetShadowDrawColor(),
               nSegments,
               mLineThickness);
    AddEllipse(list, center, radius, GetDrawColor(), nSegments, mLineThickness);
  }
}

void Graphics::DrawTranslatedEllipseWithShadow(const ImVec2& center, const ImVec2& radius)
{
  DrawEllipseWithShadow(Translate(center), radius);
}

void Graphics::RenderImage(const uint texture,
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
                 ColorToU32(cen::colors::white.with_alpha(mOpacity)));
}

void Graphics::RenderTranslatedImage(const uint texture,
                                     const ImVec4& source,
                                     const ImVec2& position,
                                     const ImVec2& uv)
{
  RenderImage(texture, source, Translate(position), uv);
}

void Graphics::RenderText(const CStr text, const ImVec2& position)
{
  TACTILE_ASSERT(text);
  ImGui::GetWindowDrawList()->AddText(position, GetDrawColor(), text);
}

void Graphics::RenderTranslatedText(const CStr text, const ImVec2& position)
{
  RenderText(text, Translate(position));
}

void Graphics::RenderCenteredText(const CStr text, const ImVec2& center)
{
  TACTILE_ASSERT(text);

  const auto size = ImGui::CalcTextSize(text);

  const auto x = center.x - (size.x / 2.0f);
  const auto y = center.y + (size.y / 2.0f);

  ImGui::GetWindowDrawList()->AddText({x, y}, GetDrawColor(), text);
}

void Graphics::RenderTranslatedGrid()
{
  const auto endRow = mBounds.end.GetRow();
  const auto endCol = mBounds.end.GetColumn();

  for (auto row = mBounds.begin.GetRow(); row < endRow; ++row) {
    for (auto col = mBounds.begin.GetColumn(); col < endCol; ++col) {
      DrawTranslatedRect(FromMatrixToAbsolute(row, col), mViewportTileSize);
    }
  }
}

void Graphics::SetDrawColor(const cen::color& color)
{
  mDrawColor = color;
}

void Graphics::SetOpacity(const float opacity)
{
  mOpacity = static_cast<uint8>(255.0f * std::clamp(opacity, 0.0f, 1.0f));
}

void Graphics::SetLineThickness(const float thickness)
{
  mLineThickness = std::clamp(thickness, 0.0f, thickness);
}

auto Graphics::FromMatrixToAbsolute(const int32 row, const int32 column) const -> ImVec2
{
  return mViewportTileSize * ImVec2{static_cast<float>(column), static_cast<float>(row)};
}

auto Graphics::IsIntersectingBounds(const ImVec2& position, const ImVec2& size) const
    -> bool
{
  const auto translated = Translate(position);
  const cen::frect rect{translated.x, translated.y, size.x, size.y};
  return cen::intersects(mBoundsRect, rect);
}

auto Graphics::IsWithinTranslatedBounds(const ImVec2& position) const -> bool
{
  const auto translated = Translate(position);
  return mBoundsRect.contains({translated.x, translated.y});
}

auto Graphics::Translate(const ImVec2& position) const -> ImVec2
{
  return mOrigin + position;
}

auto Graphics::GetDrawColor() const -> uint32
{
  return ColorToU32(mDrawColor);
}

auto Graphics::GetShadowDrawColor() const -> uint32
{
  return ColorToU32(cen::colors::black.with_alpha(mOpacity));
}

}  // namespace Tactile