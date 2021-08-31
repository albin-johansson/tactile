#pragma once

#include <centurion.hpp>  // frect

#include "aliases/ints.hpp"
#include "core/components/object.hpp"

struct ImVec2;

namespace Tactile {

/**
 * \brief Renders a point object.
 *
 * \pre The supplied object must be a point.
 *
 * \ingroup rendering
 *
 * \param object the point object that will be rendered.
 * \param position the translated position of the point.
 * \param bounds the current render bounds.
 * \param color the foreground color of the rectangle.
 * \param gridWidth the width of tiles in the viewport.
 */
void RenderPoint(const Object& object,
                 const ImVec2& position,
                 const cen::frect& bounds,
                 uint32 color,
                 float gridWidth);

}  // namespace Tactile
