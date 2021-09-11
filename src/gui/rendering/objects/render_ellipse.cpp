#include "render_ellipse.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include <cassert>  // assert

#include "aliases/cstr.hpp"
#include "core/components/object.hpp"
#include "core/components/property_context.hpp"
#include "gui/rendering/common_rendering.hpp"

namespace Tactile {
namespace {

void RenderName(const CStr name, const ImVec2& center, const ImVec2& radius)
{
  const auto textSize = ImGui::CalcTextSize(name);
  if (textSize.x <= radius.x)
  {
    const auto textX = center.x - (textSize.x / 2.0f);
    const auto textY = center.y + (textSize.y / 2.0f) + (radius.y);
    ImGui::GetWindowDrawList()->AddText({textX, textY}, IM_COL32_WHITE, name);
  }
}

}  // namespace

void RenderEllipse(const entt::registry& registry,
                   const entt::entity entity,
                   const RenderInfo& info,
                   const ImVec2& position,
                   const uint32 color)
{
  const auto& object = registry.get<Object>(entity);
  assert(object.type == ObjectType::Ellipse);

  const auto xRadius = 0.5f * object.width * info.ratio.x;
  const auto yRadius = 0.5f * object.height * info.ratio.y;

  const auto center = position + ImVec2{xRadius, yRadius};

  constexpr auto nSegments = 50;
  RenderShadowedEllipse(center, xRadius, yRadius, color, 2, nSegments);

  const auto& context = registry.get<PropertyContext>(entity);
  if (!context.name.empty())
  {
    RenderName(context.name.c_str(), center, {xRadius, yRadius});
  }
}

}  // namespace Tactile
