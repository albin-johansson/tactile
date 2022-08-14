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

#include "env.hpp"

#include <cstdlib>  // getenv, _dupenv_s, free

#include "meta/build.hpp"

namespace tactile {

auto env_var(const char* var) -> Maybe<std::string>
{
  if (var) {
#if TACTILE_PLATFORM_WINDOWS
    char* temp {};
    _dupenv_s(&temp, nullptr, var);

    if (temp) {
      std::string result {temp};
      std::free(temp);
      return result;
    }
#else
    if (const auto* value = std::getenv(var)) {
      return std::string {value};
    }
#endif  // TACTILE_PLATFORM_WINDOWS
  }

  return nothing;
}

}  // namespace tactile