#pragma once

#include "aliases/tile_id.hpp"

struct ImVec2;

namespace Tactile {

class TilesetManager;

void RenderTile(tile_id tile,
                const TilesetManager& tilesets,
                const ImVec2& screenPos,
                const ImVec2& gridSize,
                float opacity = 1.0);

}  // namespace Tactile
