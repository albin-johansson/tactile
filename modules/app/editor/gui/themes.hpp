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
enum class editor_theme {
  dear_dark = 0,   ///< The standard Dear ImGui dark theme.
  dear_light = 1,  ///< The standard Dear ImGui light theme.
  ruby = 2,
  sapphire = 3,
  emerald = 4,
  amethyst = 5,
  amber = 6,
  nocturnal = 7,
  ash = 8,
  diamond = 9,
  joker = 10,
  raspberry = 11
};

constexpr std::array themes = {editor_theme::dear_dark,
                               editor_theme::dear_light,
                               editor_theme::ruby,
                               editor_theme::sapphire,
                               editor_theme::emerald,
                               editor_theme::amethyst,
                               editor_theme::amber,
                               editor_theme::nocturnal,
                               editor_theme::ash,
                               editor_theme::diamond,
                               editor_theme::joker,
                               editor_theme::raspberry};

[[nodiscard]] auto human_readable_name(editor_theme theme) -> std::string_view;

/**
 * \brief Applies a theme to a style.
 *
 * \param style the style that will be affected.
 * \param theme the theme that will be applied to the style.
 */
void apply_theme(ImGuiStyle& style, editor_theme theme);

/// \} End of theme API

/// \} End of group gui

}  // namespace tactile
