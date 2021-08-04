#include "render_rect.hpp"

#include <imgui_internal.h>

#include "core/map/layers/object.hpp"
#include "gui/rendering/common.hpp"

namespace Tactile {
namespace {

void RenderName(const std::string_view name,
                const ImVec2& position,
                const ImVec2& rectSize)
{
  const auto textSize = ImGui::CalcTextSize(name.data());
  if (textSize.x <= rectSize.x)
  {
    const auto textX = (rectSize.x - textSize.x) / 2.0f;
    ImGui::GetWindowDrawList()->AddText(position + ImVec2{textX, rectSize.y + 4.0f},
                                        IM_COL32_WHITE,
                                        name.data());
  }
}

}  // namespace

void RenderRect(const Object& object,
                const ImVec2& position,
                const cen::frect& boundsRect,
                const ImU32 color,
                const ImVec2& ratio)
{
  const auto size = ImVec2{object.GetWidth(), object.GetHeight()} * ratio;
  const auto rect = cen::frect{position.x, position.y, size.x, size.y};
  if (cen::intersects(boundsRect, rect))
  {
    RenderShadowedRect(position, size, color, 2);

    if (const auto name = object.GetName(); !name.empty())
    {
      RenderName(name, position, size);
    }
  }
}

}  // namespace Tactile
