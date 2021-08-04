#include "render_ellipse.hpp"

#include <imgui_internal.h>

#include "core/map/layers/object.hpp"
#include "gui/rendering/common.hpp"

namespace Tactile {

void RenderEllipse(const Object& object,
                   const ImVec2& position,
                   const ImVec2& ratio,
                   const ImU32 color)
{
  constexpr auto nSegments = 50;

  const auto xRadius = object.GetWidth() * ratio.x;
  const auto yRadius = object.GetHeight() * ratio.y;

  // Offset the position to mimic Tiled behaviour
  const auto offsetPos = position + ImVec2{xRadius / 2.0f, yRadius / 2.0f};

  RenderShadowedEllipse(offsetPos, color, xRadius, yRadius, 2, nSegments);
}

}  // namespace Tactile
