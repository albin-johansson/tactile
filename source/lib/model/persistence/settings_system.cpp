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

#include "settings_system.hpp"

#include "io/settings_io.hpp"

namespace tactile {

void SettingsSystem::load_from_disk()
{
  mSettings = load_settings_from_disk();
  mSettings.print();
}

auto SettingsSystem::copy_current() -> Settings
{
  return mSettings.copy();
}

auto SettingsSystem::current_settings() -> Settings&
{
  return mSettings;
}

auto SettingsSystem::current_settings() const -> const Settings&
{
  return mSettings;
}

}  // namespace tactile
