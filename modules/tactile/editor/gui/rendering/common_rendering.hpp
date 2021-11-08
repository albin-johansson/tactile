#pragma once

#include <tactile_def.hpp>

struct ImVec2;

namespace Tactile {

/// \addtogroup rendering
/// \{

/**
 * \brief Renders a circle with a corresponding shadow.
 *
 * \param center the center of the rendered circle.
 * \param radius the radius of the circle.
 * \param color the foreground color of the circle.
 * \param thickness the thickness of the circle.
 *
 * \see `RenderShadowedEllipse()`
 * \see `RenderShadowedRect()`
 */
void RenderShadowedCircle(const ImVec2& center,
                          float radius,
                          uint32 color,
                          float thickness);

/**
 * \brief Renders an ellipse with a corresponding shadow
 *
 * \param center the center of the rendered ellipse.
 * \param xRadius the x-axis radius of the ellipse.
 * \param yRadius the y-axis radius of the ellipse.
 * \param color the foreground color of the ellipse.
 * \param thickness the thickness of the ellipse.
 * \param nSegments the number of line segments used to draw the ellipse.
 *
 * \see `RenderShadowedCircle()`
 * \see `RenderShadowedRect()`
 */
void RenderShadowedEllipse(const ImVec2& center,
                           float xRadius,
                           float yRadius,
                           uint32 color,
                           float thickness,
                           int nSegments);

/**
 * \brief Renders a rectangle with a corresponding shadow.
 *
 * \param position the position of the rendered rectangle.
 * \param size the width and height of the rectangle.
 * \param color the color of the rendered rectangle.
 * \param thickness the thickness of the rectangle.
 *
 * \see `RenderShadowedCircle()`
 * \see `RenderShadowedEllipse()`
 */
void RenderShadowedRect(const ImVec2& position,
                        const ImVec2& size,
                        uint32 color,
                        float thickness);

/// \} End of group rendering

}  // namespace Tactile
