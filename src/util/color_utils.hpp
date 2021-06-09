#pragma once

#include <QColor>  // QColor

namespace tactile {

/**
 * \brief The intensity threshold used to determine whether or not black text is
 * required when using the color as a backdrop.
 *
 * \see `intensity_of(const QColor&)`
 */
inline constexpr double color_intensity_threshold = 186;

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
 * \brief Indicates whether or not a color is bright enough to warrant black
 * foreground text.
 *
 * \param color the color that will be checked.
 *
 * \return `true` if the intensity of the color is greater than
 * `color_intensity_threshold`.
 */
[[nodiscard]] auto is_bright(const QColor& color) -> bool;

}  // namespace tactile
