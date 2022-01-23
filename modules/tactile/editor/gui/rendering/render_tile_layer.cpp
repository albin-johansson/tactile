#include "render_tile_layer.hpp"

#include "core/components/layer.hpp"
#include "core/region.hpp"
#include "core/systems/layers/tile_layer_system.hpp"
#include "graphics.hpp"
#include "render_tile.hpp"

namespace tactile {

void RenderTileLayer(Graphics& graphics,
                     const entt::registry& registry,
                     const entt::entity layerEntity,
                     const float parentOpacity)
{
  const auto& layer = registry.get<Layer>(layerEntity);
  graphics.SetOpacity(parentOpacity * layer.opacity);

  const auto bounds = graphics.GetBounds();
  const auto endRow = bounds.end.GetRow();
  const auto endCol = bounds.end.GetColumn();

  for (auto row = bounds.begin.GetRow(); row < endRow; ++row) {
    for (auto col = bounds.begin.GetColumn(); col < endCol; ++col) {
      const auto tile = sys::GetTileFromLayer(registry, layerEntity, {row, col});
      if (tile != empty_tile) {
        RenderTile(graphics, registry, tile, row, col);
      }
    }
  }
}

}  // namespace tactile
