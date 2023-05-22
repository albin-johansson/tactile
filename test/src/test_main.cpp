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

#include "engine/engine.hpp"

using namespace tactile;

class DummyApp final : public AppDelegate {
 public:
  [[nodiscard]] auto should_stop() const -> bool override { return false; }
};

auto main(int argc, char* argv[]) -> int
{
  Engine engine {BackendAPI::OpenGL};
  engine.set_app_delegate(std::make_unique<DummyApp>());
  engine.get_window().show();

  doctest::Context context {argc, argv};
  const auto res = context.run();

  if (context.shouldExit()) {
    return res;
  }

  return 0;
}