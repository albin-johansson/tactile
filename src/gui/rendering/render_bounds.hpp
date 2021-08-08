#pragma once

#include <centurion.hpp>  // frect

#include "core/region.hpp"

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

/**
 * \brief Converts the render bounds associated with a rendering context to an
 * ordinary rectangle.
 *
 * \param info the rendering context information that provides the render bounds.
 *
 * \return the render bounds as a rectangle.
 */
[[nodiscard]] auto ConvertBoundsToRect(const RenderInfo& info) -> cen::frect;

/// \} End of group rendering

}  // namespace Tactile
