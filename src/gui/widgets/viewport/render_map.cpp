#include "render_map.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include "core/map/layers/layer_utils.hpp"
#include "core/map_document.hpp"
#include "io/preferences.hpp"
#include "rendering.hpp"

namespace Tactile {
namespace {

inline constexpr auto out_border_color = IM_COL32(0xFF, 0xFF, 0xFF, 0xFF);
inline constexpr auto in_border_color = IM_COL32(0xFF, 0xFF, 0xFF, 50);

void RenderOutline(ImDrawList* drawList,
                   const int nRows,
                   const int nCols,
                   const ImVec2 offset,
                   const ImVec2 tileSize)
{
  const auto width = static_cast<float>(nCols) * tileSize.x;
  const auto height = static_cast<float>(nRows) * tileSize.y;
  drawList->AddRect(offset, offset + ImVec2{width, height}, out_border_color);
}

void RenderTileLayer(const TileLayer& layer,
                     const TilesetManager& tilesets,
                     ImDrawList* drawList,
                     const ImVec2& mapPos,
                     const ImVec2& gridSize)
{
  const auto rowCount = layer.GetRowCount();
  const auto colCount = layer.GetColumnCount();
  for (auto row = 0_row; row < rowCount; ++row)
  {
    for (auto col = 0_col; col < colCount; ++col)
    {
      const auto tile = layer.GetTile({row, col});
      if (tile && tile != empty_tile)
      {
        const ImVec2 pos = {mapPos.x + (gridSize.x * static_cast<float>(col)),
                            mapPos.y + (gridSize.y * static_cast<float>(row))};
        RenderTile(*tile, tilesets, pos, gridSize);
      }
    }
  }
}

}  // namespace

void RenderMap(const MapDocument& document,
               ImDrawList* drawList,
               const ImVec2 mapPos,
               const ImVec2 tileSize)
{
  const auto showGrid = Prefs::GetShowGrid();
  const auto& map = document.GetMap();

  for (const auto& [id, layer] : map)
  {
    if (const auto* tileLayer = AsTileLayer(layer))
    {
      RenderTileLayer(*tileLayer,
                      document.GetTilesets(),
                      drawList,
                      mapPos,
                      tileSize);
    }

    // TODO RenderObjectLayer()
  }

  const auto nRows = document.GetRowCount();
  const auto nCols = document.GetColumnCount();
  for (auto row = 0; row < nRows; ++row)
  {
    for (auto col = 0; col < nCols; ++col)
    {
      const ImVec2 pos = {mapPos.x + (tileSize.x * static_cast<float>(col)),
                          mapPos.y + (tileSize.y * static_cast<float>(row))};
      if (showGrid)
      {
        drawList->AddRect(pos, pos + tileSize, in_border_color);
      }
    }
  }

  RenderOutline(drawList, nRows, nCols, mapPos, tileSize);
}

}  // namespace Tactile
