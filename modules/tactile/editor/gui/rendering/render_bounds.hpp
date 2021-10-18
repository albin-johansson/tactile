#pragma once

#include <centurion.hpp>  // frect

#include "tactile/core/region.hpp"

namespace Tactile {

struct CanvasInfo;
struct RenderInfo;

/// \addtogroup rendering
/// \{

/**
 * \brief Returns the render bounds for the specified rendering context.
 *
 * \param canvas information about the target canvas.
 * \param info the rendering context information.
 *
 * \return the corresponding render bounds.
 */
[[nodiscard]] auto GetRenderBounds(const CanvasInfo& canvas, const RenderInfo& info)
    -> Region;

/// \} End of group rendering

}  // namespace Tactile
