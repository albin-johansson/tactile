#pragma once

#include <QColor>  // QColor

namespace tactile {

/**
 * \brief Returns the "intensity" of the specified color.
 *
 * \details This function is particularly useful to determine appropriate color
 * of foreground text.
 *
 * \param color the color to check.
 *
 * \return the intensity of the color.
 */
[[nodiscard]] auto intensity_of(const QColor& color) -> double;

/**
 * \brief The intensity threshold used to determine whether or not black text is
 * required when using the color as a backdrop.
 *
 * \see `intensity_of(const QColor&)`
 */
inline constexpr double color_intensity_threshold = 186;

}  // namespace tactile
