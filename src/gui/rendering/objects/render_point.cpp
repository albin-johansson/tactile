#include "render_point.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include <cassert>      // assert
#include <string_view>  // string_view

#include "gui/rendering/common_rendering.hpp"

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
                 const cen::frect& bounds,
                 const uint32 color,
                 const float gridWidth)
{
  assert(object.type == ObjectType::Point);
  if (bounds.contains(cen::fpoint{position.x, position.y}))
  {
    RenderShadowedCircle(position, radius, color, thickness);

    // TODO
    //    if (const auto name = object.n; !name.empty())
    //    {
    //      RenderName(name, position, gridWidth);
    //    }
  }
}

}  // namespace Tactile
