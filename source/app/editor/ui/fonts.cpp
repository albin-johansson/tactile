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

#include "fonts.hpp"

#include "io/proto/preferences.hpp"

namespace tactile::ui {

auto can_increase_font_size() -> bool
{
  const auto& prefs = io::get_preferences();
  return !prefs.use_default_font && prefs.font_size < max_font_size;
}

auto can_decrease_font_size() -> bool
{
  const auto& prefs = io::get_preferences();
  return !prefs.use_default_font && prefs.font_size > min_font_size;
}

}  // namespace tactile::ui