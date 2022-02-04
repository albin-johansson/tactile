#include "render_map.hpp"

#include "core/components/layer.hpp"
#include "core/components/object.hpp"
#include "core/components/parent.hpp"
#include "graphics.hpp"
#include "io/persistence/preferences.hpp"
#include "render_object_layer.hpp"
#include "render_tile_layer.hpp"

namespace tactile {
namespace {

void RenderLayer(graphics_ctx& graphics,
                 const entt::registry& registry,
                 const entt::entity layerEntity,
                 const comp::layer& layer,
                 const float parentOpacity)
{
  if (layer.type == layer_type::tile_layer) {
    RenderTileLayer(graphics, registry, layerEntity, parentOpacity);
  }
  else if (layer.type == layer_type::object_layer) {
    RenderObjectLayer(graphics, registry, layerEntity, parentOpacity);
  }
}

}  // namespace

void RenderMap(graphics_ctx& graphics, const entt::registry& registry)
{
  for (auto&& [entity, node] : registry.view<comp::layer_tree_node>().each()) {
    const auto& layer = registry.get<comp::layer>(entity);
    const auto& parent = registry.get<comp::parent>(entity);

    const auto* parentLayer = (parent.entity != entt::null)
                                  ? registry.try_get<comp::layer>(parent.entity)
                                  : nullptr;
    const auto parentOpacity = parentLayer ? parentLayer->opacity : 1.0f;

    if (layer.visible) {
      if (!parentLayer || parentLayer->visible) {
        RenderLayer(graphics, registry, entity, layer, layer.opacity * parentOpacity);
      }
    }
  }

  if (const auto& activeObject = registry.ctx<comp::active_object>();
      activeObject.entity != entt::null) {
    RenderObject(graphics, registry, activeObject.entity, cen::colors::yellow);
  }

  graphics.set_line_thickness(1.0f);

  if (get_preferences().is_grid_visible()) {
    graphics.set_draw_color(cen::colors::white.with_alpha(20));
    graphics.render_translated_grid();
  }
}

}  // namespace tactile
