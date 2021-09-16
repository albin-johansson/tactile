#include "render_object_layer.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include "common_rendering.hpp"
#include "core/components/layer.hpp"
#include "core/components/object.hpp"
#include "core/components/object_layer.hpp"
#include "objects/render_ellipse.hpp"
#include "objects/render_point.hpp"
#include "objects/render_rect.hpp"

namespace Tactile {

void RenderObjectLayer(const entt::registry& registry,
                       const entt::entity layerEntity,
                       const RenderInfo& info,
                       const float parentOpacity)
{
  const auto& layer = registry.get<Layer>(layerEntity);
  const auto& objectLayer = registry.get<ObjectLayer>(layerEntity);

  const auto opacity = 255.0f * (parentOpacity * layer.opacity);
  const auto color = IM_COL32(0xFF, 0, 0, opacity);

  for (const auto objectEntity : objectLayer.objects) {
    const auto& object = registry.get<Object>(objectEntity);

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
}

}  // namespace Tactile
