#include "render_point.hpp"

#include <imgui_internal.h>

#include <string_view>  // string_view

#include "core/map/layers/object.hpp"
#include "gui/rendering/common.hpp"

namespace Tactile {
namespace {

constexpr float radius = 6.0f;
constexpr float thickness = 2.0f;

void RenderName(const std::string_view name,
                const ImVec2& position,
                const float gridWidth)
{
  const auto textSize = ImGui::CalcTextSize(name.data());
  if (textSize.x <= gridWidth)
  {
    const auto textX = position.x - (textSize.x / 2.0f);
    const auto textY = position.y + radius + 4.0f;
    ImGui::GetWindowDrawList()->AddText({textX, textY}, IM_COL32_WHITE, name.data());
  }
}

}  // namespace

void RenderPoint(const Object& object,
                 const ImVec2& position,
                 const cen::frect& boundsRect,
                 const ImU32 color,
                 const float gridWidth)
{
  if (boundsRect.contains(cen::fpoint{position.x, position.y}))
  {
    RenderShadowedCircle(position, radius, color, thickness);
    if (const auto name = object.GetName(); !name.empty())
    {
      RenderName(name, position, gridWidth);
    }
  }
}

}  // namespace Tactile
