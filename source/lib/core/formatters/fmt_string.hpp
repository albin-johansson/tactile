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

#include <algorithm>  // min

#include <fmt/core.h>
#include <fmt/format.h>

#include "core/containers/array.hpp"
#include "core/containers/string.hpp"
#include "core/prelude.hpp"

namespace tactile {

template <usize Capacity = 128>
class FmtString final {
 public:
  template <typename... Args>
  explicit FmtString(StringView fmt, const Args&... args)
  {
    const auto result =
        fmt::format_to_n(mBuffer.begin(), Capacity, fmt::runtime(fmt), args...);
    *result.out = '\0';  // Ensure null-terminator
    mSize = std::min(result.size, Capacity);
  }

  [[nodiscard]] auto data() const noexcept -> const char* { return mBuffer.data(); }

  [[nodiscard]] auto view() const noexcept -> StringView
  {
    return StringView {mBuffer.data(), mSize};
  }

  [[nodiscard]] auto size() const noexcept -> usize { return view().size(); }

  [[nodiscard]] constexpr auto capacity() const noexcept -> usize { return Capacity; }

 private:
  Array<char, Capacity + 1> mBuffer;  // NOLINT
  usize mSize {};
};

}  // namespace tactile