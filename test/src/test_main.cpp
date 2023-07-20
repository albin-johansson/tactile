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

#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>

#include "common/type/ptr.hpp"
#include "engine/app_delegate.hpp"
#include "engine/engine.hpp"
#include "model/services/service_locator.hpp"
#include "model/services/settings_service.hpp"

using namespace tactile;

class NullApp final : public AppDelegate {
 public:
  NullApp()
  {
    mSettingsService = std::make_unique<SettingsService>();
    ServiceLocator<SettingsService>::reset(mSettingsService.get());
  }

  [[nodiscard]] auto should_stop() const -> bool override { return false; }

 private:
  Unique<SettingsService> mSettingsService;
};

auto main(int argc, char* argv[]) -> int
{
  Engine engine {BackendAPI::Null};
  engine.set_app_delegate(std::make_unique<NullApp>());

  doctest::Context context {argc, argv};
  const auto res = context.run();

  if (context.shouldExit()) {
    return res;
  }

  return 0;
}