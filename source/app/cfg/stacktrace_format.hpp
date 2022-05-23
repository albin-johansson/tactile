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

#pragma once

#include <cstddef>  // size_t

#include <boost/stacktrace.hpp>

namespace boost::stacktrace {

template <class CharT, class TraitsT, class Allocator>
auto stream_trace(std::basic_ostream<CharT, TraitsT>& stream,
                  const basic_stacktrace<Allocator>& trace)
    -> std::basic_ostream<CharT, TraitsT>&
{
  const auto width = stream.width();
  const auto count = trace.size();

  std::size_t index = 0;
  for (const auto& frame : trace) {
    stream.width(2);
    stream << index;

    stream.width(width);
    stream << "# " << frame.name();

    if (index < count - 1) {
      stream << '\n';
    }

    ++index;
  }

  return stream;
}

}  // namespace boost::stacktrace
