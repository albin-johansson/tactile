#include "render_stamp_preview.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include "aliases/ints.hpp"
#include "core/components/texture.hpp"
#include "core/components/tileset.hpp"
#include "core/components/uv_tile_size.hpp"
#include "core/map.hpp"
#include "gui/texture_utils.hpp"

namespace Tactile {
namespace {

constexpr uint32 preview_opacity = 100;  // [0, 255]

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

void RenderStampPreview(const entt::registry& registry,
                        const entt::entity tilesetEntity,
                        const ImVec2& mapPos,
                        const ImVec2& gridSize,
                        const MapPosition& mousePos)
{
  const auto& tileset = registry.get<Tileset>(tilesetEntity);
  const auto& texture = registry.get<Texture>(tilesetEntity);
  const auto& selection = registry.get<TilesetSelection>(tilesetEntity);
  const auto& uv = registry.get<UvTileSize>(tilesetEntity);

  const auto [topLeft, bottomRight] = selection.region.value();
  const auto size = bottomRight - topLeft;
  const MapPosition offset = {size.GetRow() / 2_row, size.GetColumn() / 2_col};

  const auto textureId = ToTextureID(texture.id);
  const ImVec2 uvTileSize = {uv.width, uv.height};

  const auto& map = registry.ctx<Map>();
  const auto endRow = size.GetRow();
  const auto endCol = size.GetColumn();
  for (row_t row{0}; row < endRow; ++row)
  {
    for (col_t col{0}; col < endCol; ++col)
    {
      const auto position = MapPosition{row, col};
      const auto tilePos = mousePos + position - offset;
      if (tilePos.GetRow() > 0_row && tilePos.GetColumn() > 0_col &&
          tilePos.GetRow() < map.row_count && tilePos.GetColumn() < map.column_count)
      {
        const auto tsTile = topLeft + position;
        const auto tsRow = static_cast<float>(tsTile.GetRow());
        const auto tsCol = static_cast<float>(tsTile.GetColumn());

        RenderPreviewTile(textureId,
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
