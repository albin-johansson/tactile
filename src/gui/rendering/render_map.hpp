#pragma once

namespace Tactile {

class MapDocument;
struct RenderInfo;

/**
 * \brief Renders a map.
 *
 * \ingroup rendering
 *
 * \param document the map document that will have its map rendered.
 * \param info the rendering context information that will be used.
 */
void RenderMap(const MapDocument& document, const RenderInfo& info);

}  // namespace Tactile
