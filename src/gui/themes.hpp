#pragma once

#include <string>  // string

#include "common/maybe.hpp"

struct ImGuiStyle;

namespace Tactile {

/// \addtogroup gui
/// \{

/// \name Theme API
/// \{

/// A string literal with the available theme options for use with combo boxes.
constexpr auto theme_options = "Dear Dark\0Dear Light\0Ash\0\0";

/**
 * \brief Applies a theme to the specified style.
 *
 * \details This function has no effect if there is no theme associated with the
 * specified name.
 *
 * \param style the style that will be affected.
 * \param name the name of the theme that will be applied to the style.
 */
void ApplyTheme(ImGuiStyle& style, const std::string& name);

/**
 * \brief Returns the name of the theme associated with the specified index.
 *
 * \param index the theme index.
 *
 * \return the name of the associated theme; `nothing` if the index is invalid.
 */
[[nodiscard]] auto GetThemeFromIndex(int index) -> Maybe<std::string>;

/**
 * \brief Returns the index (underlying value) of the theme with the specified name.
 *
 * \param name the name of the theme to get the index of.
 *
 * \return the index associated with the theme; `nothing` if the name is invalid.
 */
[[nodiscard]] auto GetThemeIndex(const std::string& name) -> Maybe<int>;

/// \} End of theme API

/// \} End of group gui

}  // namespace Tactile
