#include "render_tile.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include <cassert>  // assert

#include "core/tileset/tileset_manager.hpp"
#include "gui/texture_utils.hpp"
#include "uv_tile_size.hpp"

namespace Tactile {

void RenderTile(const tile_id tile,
                const TilesetManager& tilesets,
                const ImVec2& screenPos,
                const ImVec2& gridSize,
                const float opacity)
{
  if (const auto* tileset = tilesets.TryGetTileset(tile))
  {
    const auto renderedTile =
        tileset->IsAnimated(tile) ? tileset->GetAnimatedTile(tile) : tile;

    const auto texture = ToTextureID(tileset->GetTexture());

    const auto source = tileset->GetImageSource(renderedTile);
    assert(source.has_value());

    const auto row =
        static_cast<float>(source->y()) / static_cast<float>(source->height());
    const auto col =
        static_cast<float>(source->x()) / static_cast<float>(source->width());

    const auto uvTileSize = GetTileSizeUV(*tileset);
    const auto uvMin = ImVec2{col, row} * uvTileSize;
    const auto uvMax = uvMin + uvTileSize;

    ImGui::GetWindowDrawList()->AddImage(texture,
                                         screenPos,
                                         screenPos + gridSize,
                                         uvMin,
                                         uvMax,
                                         IM_COL32(255, 255, 255, 255 * opacity));
  }
}

}  // namespace Tactile
