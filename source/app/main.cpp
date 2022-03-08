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

#include "application.hpp"
#include "cfg/configuration.hpp"
#include "misc/crash.hpp"
#include "misc/logging.hpp"
#include "misc/throw.hpp"

auto main(int, char**) -> int
{
  try {
    tactile::app_configuration configuration;
    tactile::application app{&configuration};
    return app.run();
  }
  catch (const std::exception& e) {
    tactile::print(fmt::color::hot_pink, "Unhandled exception message: '{}'\n", e.what());

    if (const auto* stacktrace = boost::get_error_info<tactile::trace_info>(e)) {
      tactile::print(fmt::color::hot_pink, "{}\n", *stacktrace);
      tactile::dump_crash_info(*stacktrace);
    }

    std::abort();
  }
}