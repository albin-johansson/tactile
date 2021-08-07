#pragma once

struct ImVec2;

#include "aliases/ints.hpp"

namespace Tactile {

class Object;

/**
 * \brief Renders an ellipse object.
 *
 * \pre The supplied object must be an ellipse.
 *
 * \ingroup rendering
 *
 * \param object the ellipse object that will be rendered.
 * \param position the translated position of the ellipse.
 * \param ratio the ratio between the viewport tile size and the logical tile size.
 * \param color the foreground color of the ellipse.
 */
void RenderEllipse(const Object& object,
                   const ImVec2& position,
                   const ImVec2& ratio,
                   uint32 color);

}  // namespace Tactile
