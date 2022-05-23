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

#include <ostream>  // basic_ostream

#include <boost/stacktrace/stacktrace_fwd.hpp>

namespace boost::stacktrace {

template <class CharT, class TraitsT, class Allocator>
auto stream_trace(std::basic_ostream<CharT, TraitsT>& stream,
                  const basic_stacktrace<Allocator>& trace)
    -> std::basic_ostream<CharT, TraitsT>&;

template <class CharT, class TraitsT>
auto operator<<(std::basic_ostream<CharT, TraitsT>& stream, const stacktrace& trace)
    -> std::basic_ostream<CharT, TraitsT>&
{
  return stream_trace(stream, trace);
}

}  // namespace boost::stacktrace