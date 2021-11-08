#include "render_rect.hpp"

#include <cassert>  // assert

#include <tactile_def.hpp>

#include <imgui.h>
#include <imgui_internal.h>

#include "core/components/object.hpp"
#include "core/components/property_context.hpp"
#include "editor/gui/rendering/common_rendering.hpp"

namespace Tactile {
namespace {

void RenderName(const CStr name,
                const ImVec2& position,
                const ImVec2& rectSize,
                const uint32 opacity)
{
  const auto textSize = ImGui::CalcTextSize(name);
  if (textSize.x <= rectSize.x) {
    const auto textX = (rectSize.x - textSize.x) / 2.0f;
    ImGui::GetWindowDrawList()->AddText(position + ImVec2{textX, rectSize.y + 4.0f},
                                        IM_COL32(0xFF, 0xFF, 0xFF, opacity),
                                        name);
  }
}

}  // namespace

void RenderRect(const entt::registry& registry,
                const entt::entity entity,
                const RenderInfo& info,
                const ImVec2& position,
                const uint32 color)
{
  const auto& object = registry.get<Object>(entity);
  assert(object.type == ObjectType::Rectangle);

  const auto size = ImVec2{object.width, object.height} * info.ratio;
  const auto rect = cen::frect{position.x, position.y, size.x, size.y};
  if (cen::intersects(info.bounds_rect, rect)) {
    RenderShadowedRect(position, size, color, 2);

    const auto& context = registry.get<PropertyContext>(entity);
    if (!context.name.empty()) {
      RenderName(context.name.c_str(), position, size, color >> IM_COL32_A_SHIFT);
    }
  }
}

}  // namespace Tactile
