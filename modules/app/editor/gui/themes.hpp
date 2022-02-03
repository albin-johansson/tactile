#pragma once

#include <array>  // array

struct ImGuiStyle;

namespace tactile {

/// \addtogroup gui
/// \{

/// \name Theme API
/// \{

/**
 * \brief Represents the different available themes.
 *
 * \warning Do not change the enumerator values!
 *
 * \see proto::theme
 */
enum class Theme {
  dear_dark = 0,   ///< The standard Dear ImGui dark theme.
  dear_light = 1,  ///< The standard Dear ImGui light theme.
  ruby = 2,
  sapphire = 3,
  emerald = 4,
  amethyst = 5,
  amber = 6,
  nocturnal = 7,
  ash = 8
};

constexpr std::array themes = {
    Theme::dear_dark,
    Theme::dear_light,
    Theme::ruby,
    Theme::sapphire,
    Theme::emerald,
    Theme::amethyst,
    Theme::amber,
    Theme::nocturnal,
    Theme::ash,
};

[[nodiscard]] auto human_readable_name(Theme theme) -> std::string_view;

/**
 * \brief Applies a theme to the specified style.
 *
 * \param style the style that will be affected.
 * \param theme the theme that will be applied to the style.
 */
void ApplyTheme(ImGuiStyle& style, Theme theme);

/// \} End of theme API

/// \} End of group gui

}  // namespace tactile
