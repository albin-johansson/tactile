#pragma once

#include <centurion.hpp>  // frect

#include "aliases/ints.hpp"

struct ImVec2;

namespace Tactile {

class Object;

/**
 * \brief Renders a rectangle object.
 *
 * \pre The supplied object must be a rectangle.
 *
 * \ingroup rendering
 *
 * \param object the rectangle object that will be rendered.
 * \param position the translated position of the object.
 * \param bounds the current render bounds.
 * \param color the foreground color of the rectangle.
 * \param ratio the ratio between the viewport tile size and the logical tile size.
 */
void RenderRect(const Object& object,
                const ImVec2& position,
                const cen::frect& bounds,
                uint32 color,
                const ImVec2& ratio);

}  // namespace Tactile
