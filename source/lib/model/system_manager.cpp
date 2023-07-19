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

#include "system_manager.hpp"

#include "model/documents/command_system.hpp"
#include "model/i18n/language_system.hpp"
#include "model/locator.hpp"
#include "model/persistence/settings_system.hpp"
#include "model/textures/texture_system.hpp"
#include "model/textures/texture_system_factory.hpp"
#include "model/tools/tool_system.hpp"

namespace tactile {

SystemManager::SystemManager(const BackendAPI api)
{
  mSettingsSystem = std::make_unique<SettingsSystem>();
  mLanguageSystem = std::make_unique<LanguageSystem>();
  mCommandSystem = std::make_unique<CommandSystem>();
  mToolSystem = std::make_unique<ToolSystem>();
  mTextureSystem = make_texture_system(api);

  mSystems.reserve(5);
  mSystems.push_back(mSettingsSystem.get());
  mSystems.push_back(mLanguageSystem.get());
  mSystems.push_back(mCommandSystem.get());
  mSystems.push_back(mToolSystem.get());
  mSystems.push_back(mTextureSystem.get());

  Locator<SettingsSystem>::reset(mSettingsSystem.get());
  Locator<LanguageSystem>::reset(mLanguageSystem.get());
  Locator<CommandSystem>::reset(mCommandSystem.get());
  Locator<ToolSystem>::reset(mToolSystem.get());
  Locator<TextureSystem>::reset(mTextureSystem.get());
}

SystemManager::~SystemManager() noexcept = default;

void SystemManager::reset()
{
  for (auto& system: mSystems) {
    system->reset();
  }
}

void SystemManager::update(Registry& registry, Dispatcher& dispatcher)
{
  for (auto& system: mSystems) {
    system->update(registry, dispatcher);
  }
}

auto SystemManager::get_settings_system() -> SettingsSystem&
{
  return *mSettingsSystem;
}

auto SystemManager::get_settings_system() const -> const SettingsSystem&
{
  return *mSettingsSystem;
}

auto SystemManager::get_language_system() -> LanguageSystem&
{
  return *mLanguageSystem;
}

auto SystemManager::get_language_system() const -> const LanguageSystem&
{
  return *mLanguageSystem;
}

auto SystemManager::get_command_system() -> CommandSystem&
{
  return *mCommandSystem;
}

auto SystemManager::get_command_system() const -> const CommandSystem&
{
  return *mCommandSystem;
}

auto SystemManager::get_tool_system() -> ToolSystem&
{
  return *mToolSystem;
}

auto SystemManager::get_tool_system() const -> const ToolSystem&
{
  return *mToolSystem;
}

auto SystemManager::get_texture_system() -> TextureSystem&
{
  return *mTextureSystem;
}

auto SystemManager::get_texture_system() const -> const TextureSystem&
{
  return *mTextureSystem;
}

}  // namespace tactile
