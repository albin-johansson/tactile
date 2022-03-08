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

#include "boost.hpp"
#include "meta/build.hpp"

#if TACTILE_DEBUG

#define TACTILE_ASSERT(Expr) BOOST_ASSERT(Expr)
#define TACTILE_ASSERT_MSG(Expr, Msg) BOOST_ASSERT_MSG(Expr, Msg)

#else

#define TACTILE_ASSERT(Expr)
#define TACTILE_ASSERT_MSG(Expr, Msg)

#endif  // TACTILE_DEBUG

namespace boost {

void assertion_failed_msg(const char* expr,
                          const char* msg,
                          const char* function,
                          const char* file,
                          long line);

inline void assertion_failed(const char* expr,
                             const char* function,
                             const char* file,
                             const long line)
{
  assertion_failed_msg(expr, nullptr, function, file, line);
}

}  // namespace boost
