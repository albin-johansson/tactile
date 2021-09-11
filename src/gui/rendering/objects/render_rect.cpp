#include "render_rect.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include <cassert>  // assert

#include "aliases/cstr.hpp"
#include "gui/rendering/common_rendering.hpp"
#include "core/components/object.hpp"
#include "core/components/property_context.hpp"

namespace Tactile {
namespace {

void RenderName(const CStr name, const ImVec2& position, const ImVec2& rectSize)
{
  const auto textSize = ImGui::CalcTextSize(name);
  if (textSize.x <= rectSize.x)
  {
    const auto textX = (rectSize.x - textSize.x) / 2.0f;
    ImGui::GetWindowDrawList()->AddText(position + ImVec2{textX, rectSize.y + 4.0f},
                                        IM_COL32_WHITE,
                                        name);
  }
}

}  // namespace

void RenderRect(const entt::registry& registry,
                const entt::entity entity,
                const ImVec2& position,
                const cen::frect& bounds,
                const uint32 color,
                const ImVec2& ratio)
{
  const auto& object = registry.get<Object>(entity);
  assert(object.type == ObjectType::Rectangle);

  const auto size = ImVec2{object.width, object.height} * ratio;
  const auto rect = cen::frect{position.x, position.y, size.x, size.y};
  if (cen::intersects(bounds, rect))
  {
    RenderShadowedRect(position, size, color, 2);

    const auto& context = registry.get<PropertyContext>(entity);
    if (!context.name.empty())
    {
      RenderName(context.name.c_str(), position, size);
    }
  }
}

}  // namespace Tactile
