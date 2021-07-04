#include "uv_tile_size.hpp"

#include <imgui_internal.h>

#include "core/tileset/tileset.hpp"

namespace Tactile {

[[nodiscard]] auto GetTileSizeUV(const Tileset& tileset) -> ImVec2
{
  const ImVec2 textureSize = {static_cast<float>(tileset.GetWidth()),
                              static_cast<float>(tileset.GetHeight())};
  const ImVec2 tileSize = {static_cast<float>(tileset.GetTileWidth()),
                           static_cast<float>(tileset.GetTileHeight())};
  return tileSize / textureSize;
}

}  // namespace Tactile
