#include "render_object.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include "core/components/object.hpp"
#include "render_ellipse.hpp"
#include "render_point.hpp"
#include "render_rect.hpp"

namespace Tactile {

void RenderObject(const entt::registry& registry,
                  const entt::entity objectEntity,
                  const RenderInfo& info,
                  const uint32 color)
{
  const auto& object = registry.get<Object>(objectEntity);

  if (!object.visible) {
    return;
  }

  const auto localPos = ImVec2{object.x, object.y};
  const auto absolutePos = info.map_position + (localPos * info.ratio);

  switch (object.type) {
    case ObjectType::Point:
      RenderPoint(registry, objectEntity, info, absolutePos, color);
      break;

    case ObjectType::Ellipse:
      RenderEllipse(registry, objectEntity, info, absolutePos, color);
      break;

    case ObjectType::Rectangle:
      RenderRect(registry, objectEntity, info, absolutePos, color);
      break;
  }
}

}  // namespace Tactile