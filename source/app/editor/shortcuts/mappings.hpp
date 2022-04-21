/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
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

#include "meta/build.hpp"
#include "tactile.hpp"

namespace tactile {

#if TACTILE_PLATFORM_OSX

#define TACTILE_PRIMARY_MOD "Cmd"
#define TACTILE_SECONDARY_MOD "Option"

inline constexpr cen::key_mod primary_modifier = cen::key_mod::lgui;

#else

#define TACTILE_PRIMARY_MOD "Ctrl"
#define TACTILE_SECONDARY_MOD "Alt"

inline constexpr cen::key_mod primary_modifier = cen::key_mod::lctrl;

#endif  // TACTILE_PLATFORM_OSX

inline constexpr cen::key_mod secondary_modifier = cen::key_mod::lalt;

}  // namespace tactile
