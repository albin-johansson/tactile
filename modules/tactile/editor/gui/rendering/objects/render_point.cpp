#include "render_point.hpp"

#include <imgui.h>
#include <imgui_internal.h>
#include <tactile_def.hpp>

#include "assert.hpp"
#include "core/components/object.hpp"
#include "core/components/property_context.hpp"
#include "editor/gui/rendering/common_rendering.hpp"

namespace Tactile {
namespace {

constexpr float radius = 6.0f;
constexpr float thickness = 2.0f;

void RenderName(const CStr name,
                const ImVec2& position,
                const float gridWidth,
                const uint32 opacity)
{
  const auto textSize = ImGui::CalcTextSize(name);
  if (textSize.x <= gridWidth) {
    const auto textX = position.x - (textSize.x / 2.0f);
    const auto textY = position.y + radius + 4.0f;
    ImGui::GetWindowDrawList()->AddText({textX, textY},
                                        IM_COL32(0xFF, 0xFF, 0xFF, opacity),
                                        name);
  }
}

}  // namespace

void RenderPoint(const entt::registry& registry,
                 const entt::entity entity,
                 const RenderInfo& info,
                 const ImVec2& position,
                 const uint32 color)
{
  const auto& object = registry.get<Object>(entity);
  TACTILE_ASSERT(object.type == ObjectType::Point);

  if (info.bounds_rect.Contains(cen::FPoint{position.x, position.y})) {
    RenderShadowedCircle(position, radius, color, thickness);

    const auto& context = registry.get<PropertyContext>(entity);
    if (!context.name.empty()) {
      RenderName(context.name.c_str(),
                 position,
                 info.grid_size.x,
                 color >> IM_COL32_A_SHIFT);
    }
  }
}

}  // namespace Tactile
