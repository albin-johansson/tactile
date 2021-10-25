#include "render_tile_layer.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include "core/components/layer.hpp"
#include "core/components/tile_layer.hpp"
#include "core/systems/layers/tile_layer_system.hpp"
#include "render_info.hpp"
#include "render_tile.hpp"

namespace Tactile {

void RenderTileLayer(const entt::registry& registry,
                     const entt::entity layerEntity,
                     const RenderInfo& info,
                     const float parentOpacity)
{
  const auto& layer = registry.get<Layer>(layerEntity);
  const auto opacity = parentOpacity * layer.opacity;

  const auto endRow = info.bounds.end.GetRow();
  const auto endCol = info.bounds.end.GetColumn();
  for (auto row = info.bounds.begin.GetRow(); row < endRow; ++row) {
    for (auto col = info.bounds.begin.GetColumn(); col < endCol; ++col) {
      const auto tile = Sys::GetTileFromLayer(registry, layerEntity, {row, col});
      if (tile != empty_tile) {
        const ImVec2 offset =
            info.grid_size * ImVec2{static_cast<float>(col), static_cast<float>(row)};
        const ImVec2 position = info.map_position + offset;
        RenderTile(tile, registry, position, info.grid_size, opacity);
      }
    }
  }
}

}  // namespace Tactile
