#pragma once

#include "aliases/tile_id.hpp"

struct ImVec2;

namespace Tactile {

class TilesetManager;

/**
 * \brief Renders a single tile.
 *
 * \ingroup rendering
 *
 * \param tile the ID of the tile that will be rendered.
 * \param tilesets the available tilesets.
 * \param screenPos the position of the rendered tile on the screen.
 * \param gridSize the size of tiles in the viewport.
 * \param opacity the opacity of the tile, in the range [0, 1].
 */
void RenderTile(tile_id tile,
                const TilesetManager& tilesets,
                const ImVec2& screenPos,
                const ImVec2& gridSize,
                float opacity = 1.0);

}  // namespace Tactile
