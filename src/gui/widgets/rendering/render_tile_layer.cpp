#include "render_tile_layer.hpp"

#include <imgui.h>

#include "core/map/layers/tile_layer.hpp"
#include "gui/widgets/rendering/render_bounds.hpp"
#include "render_tile.hpp"

namespace Tactile {

void RenderTileLayer(const TileLayer& layer,
                     const TilesetManager& tilesets,
                     const ImVec2& mapPos,
                     const ImVec2& gridSize,
                     const RenderBounds& bounds)
{
  const auto opacity = layer.GetOpacity();

  const auto endRow = bounds.end.GetRow();
  const auto endCol = bounds.end.GetColumn();
  for (auto row = bounds.begin.GetRow(); row < endRow; ++row)
  {
    for (auto col = bounds.begin.GetColumn(); col < endCol; ++col)
    {
      const auto tile = layer.GetTile({row, col});
      if (tile && tile != empty_tile)
      {
        const ImVec2 pos = {mapPos.x + (gridSize.x * static_cast<float>(col)),
                            mapPos.y + (gridSize.y * static_cast<float>(row))};
        RenderTile(*tile, tilesets, pos, gridSize, opacity);
      }
    }
  }
}

}  // namespace Tactile
