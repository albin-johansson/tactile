#pragma once

#include <array>    // array
#include <utility>  // make_pair

#include "io/preferences.hpp"

struct ImGuiStyle;

namespace Tactile {

/// \addtogroup gui
/// \{

/// \name Theme API
/// \{

constexpr std::array themes = {
    std::make_pair("Dear Dark", Theme::DearDark),
    std::make_pair("Dear Light", Theme::DearLight),
    std::make_pair("Ruby", Theme::Ruby),
    std::make_pair("Sapphire", Theme::Sapphire),
    std::make_pair("Emerald", Theme::Emerald),
    std::make_pair("Amethyst", Theme::Amethyst),
    std::make_pair("Amber", Theme::Amber),
    std::make_pair("Nocturnal", Theme::Nocturnal),
    std::make_pair("Ash", Theme::Ash),
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

}  // namespace Tactile
