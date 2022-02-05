#include "render_tile_layer.hpp"

#include "core/components/layer.hpp"
#include "core/region.hpp"
#include "core/systems/layers/tile_layer_system.hpp"
#include "graphics.hpp"
#include "render_tile.hpp"

namespace tactile {

void RenderTileLayer(graphics_ctx& graphics,
                     const entt::registry& registry,
                     const entt::entity layerEntity,
                     const float parentOpacity)
{
  const auto& layer = registry.get<comp::layer>(layerEntity);
  const auto& tileLayer = registry.get<comp::tile_layer>(layerEntity);

  graphics.set_opacity(parentOpacity * layer.opacity);

  const auto bounds = graphics.bounds();
  const auto endRow = bounds.end.row();
  const auto endCol = bounds.end.col();

  for (auto row = bounds.begin.row(); row < endRow; ++row) {
    for (auto col = bounds.begin.col(); col < endCol; ++col) {
      const auto tile = sys::get_tile(tileLayer, {row, col});
      if (tile != empty_tile) {
        RenderTile(graphics, registry, tile, row, col);
      }
    }
  }
}

}  // namespace tactile