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

#include "font_system.hpp"

#include "model/context.hpp"
#include "ui/constants.hpp"

namespace tactile::sys {

auto can_increase_font_size(const Model&) -> bool
{
  const auto& settings = get_global_settings();
  return !settings.test_flag(SETTINGS_USE_DEFAULT_FONT_BIT) &&
         settings.get_font_size() < kMaxFontSize;
}

auto can_decrease_font_size(const Model&) -> bool
{
  const auto& settings = get_global_settings();
  return !settings.test_flag(SETTINGS_USE_DEFAULT_FONT_BIT) &&
         settings.get_font_size() > kMaxFontSize;
}

auto can_reset_font_size(const Model&) -> bool
{
  const auto& settings = get_global_settings();
  return !settings.test_flag(SETTINGS_USE_DEFAULT_FONT_BIT);
}

}  // namespace tactile::sys
