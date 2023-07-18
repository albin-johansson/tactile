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

#include <algorithm>  // min, max

#include <spdlog/spdlog.h>

#include "io/settings_io.hpp"
#include "ui/constants.hpp"

namespace tactile {

void SettingsSystem::load_from_disk()
{
  mSettings = load_settings_from_disk();
  mSettings.print();
}

void SettingsSystem::reset_font_size()
{
  spdlog::trace("[SettingsSystem] Resetting the font size");

  mSettings.set_font_size(kDefFontSize);
}

void SettingsSystem::increase_font_size()
{
  spdlog::trace("[SettingsSystem] Increasing the font size");

  const auto new_size = std::min(mSettings.get_font_size() + 2, kMaxFontSize);
  mSettings.set_font_size(new_size);
}

void SettingsSystem::decrease_font_size()
{
  spdlog::trace("[SettingsSystem] Decreasing the font size");

  const auto new_size = std::max(mSettings.get_font_size() - 2, kMinFontSize);
  mSettings.set_font_size(new_size);
}

auto SettingsSystem::can_reset_font_size() const -> bool
{
  return !mSettings.test_flag(SETTINGS_USE_DEFAULT_FONT_BIT);
}

auto SettingsSystem::can_increase_font_size() const -> bool
{
  return !mSettings.test_flag(SETTINGS_USE_DEFAULT_FONT_BIT) &&
         mSettings.get_font_size() < kMaxFontSize;
}

auto SettingsSystem::can_decrease_font_size() const -> bool
{
  return !mSettings.test_flag(SETTINGS_USE_DEFAULT_FONT_BIT) &&
         mSettings.get_font_size() > kMinFontSize;
}

auto SettingsSystem::copy_current() const -> Settings
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
