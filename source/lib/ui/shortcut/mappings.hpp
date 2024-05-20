// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <centurion/keyboard.hpp>

#include "tactile/base/prelude.hpp"

namespace tactile {

#if TACTILE_OS_APPLE
  #define TACTILE_PRIMARY_MOD "Cmd"
  #define TACTILE_SECONDARY_MOD "Option"
#else
  #define TACTILE_PRIMARY_MOD "Ctrl"
  #define TACTILE_SECONDARY_MOD "Alt"
#endif  // TACTILE_OS_APPLE

inline constexpr cen::key_mod kPrimaryModifier = kOnMacos ? cen::key_mod::lgui  //
                                                          : cen::key_mod::lctrl;
inline constexpr cen::key_mod kPrimaryModifierAndShift =
    kPrimaryModifier | cen::key_mod::lshift;

inline constexpr cen::key_mod kSecondaryModifier = cen::key_mod::lalt;
inline constexpr cen::key_mod kSecondaryModifierAndShift =
    kSecondaryModifier | cen::key_mod::lshift;

}  // namespace tactile
