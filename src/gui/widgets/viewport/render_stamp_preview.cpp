#include "render_stamp_preview.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include "aliases/ints.hpp"
#include "core/map/map.hpp"
#include "core/tileset/tileset.hpp"
#include "gui/get_texture_id.hpp"
#include "rendering.hpp"

namespace Tactile {
namespace {

inline constexpr uint32 preview_opacity = 100;  // [0, 255]

void RenderPreviewTile(ImTextureID texture,
                       const MapPosition& tilePos,
                       const ImVec2& mapPos,
                       const ImVec2& tilesetTilePos,
                       const ImVec2& uvTileSize,
                       const ImVec2& gridSize)
{
  const auto x = static_cast<float>(tilePos.GetColumn()) * gridSize.x;
  const auto y = static_cast<float>(tilePos.GetRow()) * gridSize.y;
  const auto realPos = mapPos + ImVec2{x, y};

  const auto uvMin = tilesetTilePos * uvTileSize;
  const auto uvMax = uvMin + uvTileSize;

  auto* drawList = ImGui::GetWindowDrawList();
  drawList->AddImage(texture,
                     realPos,
                     realPos + gridSize,
                     uvMin,
                     uvMax,
                     IM_COL32(0xFF, 0xFF, 0xFF, preview_opacity));
}

}  // namespace

void RenderStampPreview(const ImVec2& mapPos,
                        const ImVec2& gridSize,
                        const Map& map,
                        const Tileset& tileset,
                        const MapPosition& mousePos)
{
  const auto [topLeft, bottomRight] = tileset.GetSelection().value();
  const auto size = bottomRight - topLeft;
  const MapPosition offset = {size.GetRow() / 2_row, size.GetColumn() / 2_col};

  const auto texture = GetTextureID(tileset.GetTexture());
  const auto uvTileSize = GetTileSizeUV(tileset);

  const auto endRow = size.GetRow();
  const auto endCol = size.GetColumn();
  for (row_t row{0}; row < endRow; ++row)
  {
    for (col_t col{0}; col < endCol; ++col)
    {
      const auto position = MapPosition{row, col};
      const auto tilePos = mousePos + position - offset;
      if (map.InBounds(tilePos))
      {
        const auto tsTile = topLeft + position;
        const auto tsRow = static_cast<float>(tsTile.GetRow());
        const auto tsCol = static_cast<float>(tsTile.GetColumn());

        RenderPreviewTile(texture,
                          tilePos,
                          mapPos,
                          {tsCol, tsRow},
                          uvTileSize,
                          gridSize);
      }
    }
  }
}

}  // namespace Tactile
