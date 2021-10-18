#pragma once

#include <entt.hpp>  // registry

#include <tactile-base/tactile_std.hpp>

struct ImVec2;

namespace Tactile {

/**
 * \brief Renders a single tile.
 *
 * \ingroup rendering
 *
 * \param tile the ID of the tile that will be rendered.
 * \param registry the active registry.
 * \param screenPos the position of the rendered tile on the screen.
 * \param gridSize the size of tiles in the viewport.
 * \param opacity the opacity of the tile, in the range [0, 1].
 */
void RenderTile(TileID tile,
                const entt::registry& registry,
                const ImVec2& screenPos,
                const ImVec2& gridSize,
                float opacity = 1.0);

}  // namespace Tactile
