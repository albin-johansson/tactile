#include "render_map.hpp"

#include "core/components/layer.hpp"
#include "core/components/object.hpp"
#include "core/components/parent.hpp"
#include "core/utils/color_utils.hpp"
#include "graphics.hpp"
#include "io/preferences.hpp"
#include "render_info.hpp"
#include "render_object_layer.hpp"
#include "render_tile_layer.hpp"

namespace Tactile {
namespace {

void RenderLayer(Graphics& graphics,
                 const Region& bounds,
                 const entt::registry& registry,
                 const entt::entity layerEntity,
                 const Layer& layer,
                 const float parentOpacity)
{
  if (layer.type == LayerType::TileLayer) {
    RenderTileLayer(graphics, registry, layerEntity, bounds, parentOpacity);
  }
  else if (layer.type == LayerType::ObjectLayer) {
    RenderObjectLayer(graphics, registry, layerEntity, parentOpacity);
  }
}

}  // namespace

void RenderMap(const entt::registry& registry, const RenderInfo& info)
{
  Graphics graphics{info};

  for (auto&& [entity, node] : registry.view<LayerTreeNode>().each()) {
    const auto& layer = registry.get<Layer>(entity);
    const auto& parent = registry.get<Parent>(entity);

    const auto* parentLayer =
        (parent.entity != entt::null) ? registry.try_get<Layer>(parent.entity) : nullptr;
    const auto parentOpacity = parentLayer ? parentLayer->opacity : 1.0f;

    if (layer.visible) {
      if (!parentLayer || parentLayer->visible) {
        RenderLayer(graphics,
                    info.bounds,
                    registry,
                    entity,
                    layer,
                    layer.opacity * parentOpacity);
      }
    }
  }

  if (const auto& activeObject = registry.ctx<ActiveObject>();
      activeObject.entity != entt::null) {
    RenderObject(graphics, registry, activeObject.entity, cen::colors::yellow);
  }

  graphics.SetLineThickness(1.0f);

  if (Prefs::GetShowGrid()) {
    const auto gridColor = Darker(Prefs::GetViewportBackground());
    graphics.SetDrawColor(gridColor.WithAlpha(50));
    graphics.RenderGrid(info.map_position, info.bounds);
  }
}

}  // namespace Tactile
