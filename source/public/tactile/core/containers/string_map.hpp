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

#include <functional>     // hash, equal_to
#include <string>         // string
#include <string_view>    // string_view
#include <unordered_map>  // unordered_map

#include "tactile/core/common/prelude.hpp"
#include "tactile/core/core.hpp"

namespace tactile {

struct TACTILE_CORE_API StringHash final {
  using is_transparent [[maybe_unused]] = void;

  [[nodiscard]] auto operator()(const char* str) const -> usize
  {
    return std::hash<const char*> {}(str);
  }

  [[nodiscard]] auto operator()(std::string_view str) const -> usize
  {
    return std::hash<std::string_view> {}(str);
  }

  [[nodiscard]] auto operator()(const std::string& str) const -> usize
  {
    return std::hash<std::string> {}(str);
  }
};

/** Hash map optimized for string keys, supporting heterogeneous lookups. */
template <typename T>
using StringMap = std::unordered_map<std::string, T, StringHash, std::equal_to<>>;

}  // namespace tactile
