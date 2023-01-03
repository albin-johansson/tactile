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

#include <exception>  // exception

#include "common/predef.hpp"
#include "common/type/string.hpp"
#include "core/vocabulary.hpp"

namespace tactile {

/// The exception type used for all exceptions thrown in the codebase.
class TactileError : public std::exception {
 public:
  TACTILE_NOINLINE explicit TactileError(String what);

  [[nodiscard]] auto what() const noexcept -> const char* override
  {
    return mWhat.c_str();
  }

  [[nodiscard]] auto get_trace() const -> const String& { return mTrace; }

 private:
  String mWhat {"N/A"};
  String mTrace;
};

}  // namespace tactile
