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

#include <cstdlib>    // abort
#include <exception>  // exception

#include <fmt/ostream.h>
#include <spdlog/spdlog.h>

#include "application.hpp"
#include "cfg/configuration.hpp"
#include "io/directories.hpp"
#include "misc/logging.hpp"
#include "misc/panic.hpp"

auto main(int, char**) -> int
{
  tactile::init_logger();

  try {
    spdlog::info("Using persistent file directory {}",
                 tactile::io::persistent_file_dir());

    tactile::AppConfiguration configuration;
    tactile::Application app{&configuration};
    app.start();

    return 0;
  }
  catch (const tactile::TactileError& e) {
    spdlog::critical("Unhandled exception message: '{}'\n{}", e.what(), e.trace());
    std::abort();
  }
  catch (const std::exception& e) {
    spdlog::critical("Unhandled exception message: '{}'", e.what());
    std::abort();
  }
}