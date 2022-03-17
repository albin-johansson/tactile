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

#include "assert.hpp"

#include <cstdlib>  // abort

#include "crash.hpp"
#include "logging.hpp"
#include "stacktrace.hpp"

namespace boost {

void assertion_failed_msg(const char* expr,
                          const char* msg,
                          const char* /* function */,
                          const char* file,
                          const long line)

{
  const stacktrace::stacktrace trace{};

  tactile::print(fmt::color::orange_red,
                 "{}:{} expression '{}' evaluated to false: {}\n{}",
                 file,
                 line,
                 expr,
                 msg ? msg : "N/A",
                 trace);
  tactile::dump_crash_info(trace);

  std::abort();
}

}  // namespace boost