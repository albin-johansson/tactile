#pragma once

#include <array>    // array
#include <utility>  // make_pair

#include "io/preferences.hpp"

struct ImGuiStyle;

namespace tactile {

/// \addtogroup gui
/// \{

/// \name Theme API
/// \{

constexpr std::array themes = {
    std::make_pair("Dear Dark", Theme::dear_dark),
    std::make_pair("Dear Light", Theme::dear_light),
    std::make_pair("Ruby", Theme::ruby),
    std::make_pair("Sapphire", Theme::sapphire),
    std::make_pair("Emerald", Theme::emerald),
    std::make_pair("Amethyst", Theme::amethyst),
    std::make_pair("Amber", Theme::amber),
    std::make_pair("Nocturnal", Theme::nocturnal),
    std::make_pair("Ash", Theme::ash),
};

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
