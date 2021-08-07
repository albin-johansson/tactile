#pragma once

namespace Tactile {

class ObjectLayer;
struct RenderInfo;

/**
 * \brief Renders an object layer.
 *
 * \ingroup rendering
 *
 * \param layer the object layer that will be rendered.
 * \param info the rendering context information that will be used.
 * \param parentOpacity the opacity of the parent layer.
 */
void RenderObjectLayer(const ObjectLayer& layer,
                       const RenderInfo& info,
                       float parentOpacity);

}  // namespace Tactile
