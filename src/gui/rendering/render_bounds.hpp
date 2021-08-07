#pragma once

#include <centurion.hpp>  // frect

struct ImVec2;

#include "aliases/col.hpp"
#include "aliases/row.hpp"
#include "core/map/map_position.hpp"

namespace Tactile {

struct CanvasInfo;
struct RenderInfo;

/// \addtogroup rendering
/// \{

/**
 * \brief Represents the area of a map that should be rendered.
 *
 * \see `GetRenderBounds()`
 * \see `ConvertBoundsToRect()`
 */
struct RenderBounds final
{
  MapPosition begin;  ///< The top-left position of the bounds.
  MapPosition end;    ///< The bottom-right position of the bounds.
};

/**
 * \brief Returns the render bounds for the specified rendering context.
 *
 * \param canvas information about the target canvas.
 * \param info the rendering context information.
 *
 * \return the corresponding render bounds.
 */
[[nodiscard]] auto GetRenderBounds(const CanvasInfo& canvas, const RenderInfo& info)
    -> RenderBounds;

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
