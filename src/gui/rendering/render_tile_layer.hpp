#pragma once

namespace Tactile {

class TileLayer;
class TilesetManager;
struct RenderInfo;

/**
 * \brief Renders a tile layer.
 *
 * \ingroup rendering
 *
 * \param layer the tile layer that will be rendered.
 * \param tilesets the available tilesets.
 * \param info the rendering context information that will be used.
 * \param parentOpacity the opacity of the parent layer.
 */
void RenderTileLayer(const TileLayer& layer,
                     const TilesetManager& tilesets,
                     const RenderInfo& info,
                     float parentOpacity);

}  // namespace Tactile
