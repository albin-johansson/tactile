#pragma once

#include <string_view>  // string_view

#include "common/maybe.hpp"
#include "io/preferences.hpp"

struct ImGuiStyle;

namespace Tactile {

/// \addtogroup gui
/// \{

/// \name Theme API
/// \{

/**
 * \brief Applies a theme to the specified style.
 *
 * \param style the style that will be affected.
 * \param theme the theme that will be applied to the style.
 */
void ApplyTheme(ImGuiStyle& style, Theme theme);

[[nodiscard]] auto ToString(Theme theme) -> std::string_view;

/// \} End of theme API

/// \} End of group gui

}  // namespace Tactile
