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

#include "crash.hpp"

#include <fstream>  // ofstream
#include <ios>      // ios

namespace tactile {

void dump_crash_info(const boost::stacktrace::stacktrace& trace)
{
  std::ofstream stream{"crash.txt", std::ios::out | std::ios::trunc};
  stream << trace;
}

}  // namespace tactile