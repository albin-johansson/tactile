/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <centurion/keyboard.hpp>

#include "tactile/core/prelude.hpp"

namespace tactile {

#if TACTILE_OS_MACOS
#define TACTILE_PRIMARY_MOD "Cmd"
#define TACTILE_SECONDARY_MOD "Option"
#else
#define TACTILE_PRIMARY_MOD "Ctrl"
#define TACTILE_SECONDARY_MOD "Alt"
#endif  // TACTILE_OS_MACOS

inline constexpr cen::key_mod kPrimaryModifier = kIsApple ? cen::key_mod::lgui  //
                                                          : cen::key_mod::lctrl;
inline constexpr cen::key_mod kPrimaryModifierAndShift =
    kPrimaryModifier | cen::key_mod::lshift;

inline constexpr cen::key_mod kSecondaryModifier = cen::key_mod::lalt;
inline constexpr cen::key_mod kSecondaryModifierAndShift =
    kSecondaryModifier | cen::key_mod::lshift;

}  // namespace tactile
