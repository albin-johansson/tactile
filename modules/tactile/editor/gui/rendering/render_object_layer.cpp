#include "render_object_layer.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include "common_rendering.hpp"
#include "core/components/layer.hpp"
#include "objects/render_object.hpp"

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
    RenderObject(registry, objectEntity, info, color);
  }
}

}  // namespace Tactile
