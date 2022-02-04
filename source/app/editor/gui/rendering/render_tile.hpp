#pragma once

#include <entt/entt.hpp>

#include "tactile_def.hpp"

struct ImVec2;

namespace tactile {

class graphics_ctx;

/**
 * \brief Renders a single tile.
 *
 * \ingroup rendering
 *
 * \param graphics the graphics context that will be used.
 * \param registry the active registry.
 * \param tile the tile that will be rendered.
 * \param row the row coordinate of the tile.
 * \param column the column coordinate of the tile.
 */
void RenderTile(graphics_ctx& graphics,
                const entt::registry& registry,
                tile_id tile,
                int32 row,
                int32 column);

}  // namespace tactile
