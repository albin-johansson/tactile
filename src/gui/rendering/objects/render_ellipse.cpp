#include "render_ellipse.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include <cassert>  // assert

#include "gui/rendering/common_rendering.hpp"

namespace Tactile {

void RenderEllipse(const Object& object,
                   const ImVec2& position,
                   const ImVec2& ratio,
                   const uint32 color)
{
  assert(object.type == ObjectType::Ellipse);

  constexpr auto nSegments = 50;

  const auto xRadius = 0.5f * object.width * ratio.x;
  const auto yRadius = 0.5f * object.height * ratio.y;

  const auto center = position + ImVec2{xRadius, yRadius};
  RenderShadowedEllipse(center, xRadius, yRadius, color, 2, nSegments);
}

}  // namespace Tactile
