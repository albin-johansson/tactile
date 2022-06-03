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

#include "core/common/ints.hpp"

namespace tactile::ui {

/// \name Font functions
/// \{

/**
 * Reloads the main and icon fonts.
 *
 * This is meant to be called every time the application has reason to believe
 * that the fonts need to be reloaded, e.g. when the reported display framebuffer size
 * changes. This information may change during the first couple of frames of the
 * application, so this needs to be dynamically handled.
 */
void reload_fonts();

/// Indicates whether it's possible to increase the font size.
[[nodiscard]] auto can_increase_font_size() -> bool;

/// Indicates whether it's possible to decrease the font size.
[[nodiscard]] auto can_decrease_font_size() -> bool;

/// Returns the default font size.
[[nodiscard]] auto get_default_font_size() -> int32;

/// Returns the smallest acceptable font size.
[[nodiscard]] auto get_min_font_size() -> int32;

/// Returns the largest acceptable font size.
[[nodiscard]] auto get_max_font_size() -> int32;

/// \} End of font functions

}  // namespace tactile::ui