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

#pragma once

#include "tactile/core/common/prelude.hpp"
#include "tactile/core/core.hpp"

#ifndef TACTILE_ENABLE_ASSERTIONS
  #ifdef NDEBUG
    #define TACTILE_ENABLE_ASSERTIONS 0
  #else
    #define TACTILE_ENABLE_ASSERTIONS 1
  #endif  // NDEBUG
#endif    // TACTILE_ENABLE_ASSERTIONS

#if TACTILE_ENABLE_ASSERTIONS
  #define TACTILE_ASSERT_MSG(Expr, Msg) \
    (!!(Expr) ? ((void) 0)              \
              : tactile::on_assertion_failed(#Expr, (Msg), __FILE__, __LINE__))
  #define TACTILE_ASSERT(Expr) TACTILE_ASSERT_MSG(Expr, "?")
#else
  #define TACTILE_ASSERT(Expr)
  #define TACTILE_ASSERT_MSG(Expr, Msg)
#endif  // TACTILE_ENABLE_ASSERTIONS

namespace tactile {

[[noreturn]] TACTILE_CORE_API void on_assertion_failed(const char* expr,
                                                       const char* msg,
                                                       const char* file,
                                                       int64 line);

}  // namespace tactile
