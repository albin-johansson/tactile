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

#include "tactile/core/platform/environment.hpp"

#include <cstdlib>  // getenv, _dupenv_s, free

#include "tactile/core/common/prelude.hpp"

namespace tactile {

auto read_env_var(const char* var) -> Maybe<String>
{
  if (var) {
#if TACTILE_OS_WINDOWS
    char* temp {};
    _dupenv_s(&temp, nullptr, var);

    if (temp) {
      String result {temp};
      std::free(temp);
      return result;
    }
#else
    if (const auto* value = std::getenv(var)) {
      return String {value};
    }
#endif  // TACTILE_OS_WINDOWS
  }

  return kNone;
}

}  // namespace tactile