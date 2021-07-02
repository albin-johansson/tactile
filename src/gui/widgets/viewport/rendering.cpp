#include "rendering.hpp"

#include <imgui_internal.h>

#include <cassert>  //assert

#include "core/tileset/tileset.hpp"
#include "core/tileset/tileset_manager.hpp"
#include "gui/get_texture_id.hpp"

namespace Tactile {

[[nodiscard]] auto GetTileSizeUV(const Tileset& tileset) -> ImVec2
{
  const ImVec2 textureSize = {static_cast<float>(tileset.GetWidth()),
                              static_cast<float>(tileset.GetHeight())};
  const ImVec2 tileSize = {static_cast<float>(tileset.GetTileWidth()),
                           static_cast<float>(tileset.GetTileHeight())};
  return tileSize / textureSize;
}

void RenderTile(const tile_id tile,
                const TilesetManager& tilesets,
                const ImVec2& screenPos,
                const ImVec2& gridSize)
{
  const auto& tileset = tilesets.GetTileset(tile);
  const auto texture = GetTextureID(tileset.GetTexture());

  const auto source = tileset.GetImageSource(tile);
  assert(source.has_value());

  const auto row =
      static_cast<float>(source->y()) / static_cast<float>(source->height());
  const auto col =
      static_cast<float>(source->x()) / static_cast<float>(source->width());

  const auto uvTileSize = GetTileSizeUV(tileset);
  const auto uvMin = ImVec2{col, row} * uvTileSize;
  const auto uvMax = uvMin + uvTileSize;

  ImGui::GetWindowDrawList()->AddImage(texture,
                                       screenPos,
                                       screenPos + gridSize,
                                       uvMin,
                                       uvMax);
}

}  // namespace Tactile
