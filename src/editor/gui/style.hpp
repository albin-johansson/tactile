#pragma once

struct ImGuiStyle;

namespace Tactile {

/**
 * \brief Applies the styling used by Tactile to the specified style instance.
 *
 * \note This function doesn't affect the colors of the style.
 *
 * \param style the style that will be updated.
 */
void ApplyStyle(ImGuiStyle& style);

}  // namespace Tactile
