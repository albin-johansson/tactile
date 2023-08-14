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

#include "tactile/core/containers/array.hpp"
#include "tactile/core/containers/string.hpp"
#include "core/prelude.hpp"

namespace tactile {

class StringBuffer final {
 public:
  TACTILE_DEFAULT_COPY(StringBuffer);
  TACTILE_DEFAULT_MOVE(StringBuffer);

  StringBuffer() = default;

  auto operator=(StringView str) -> StringBuffer&;

  void clear();

  [[nodiscard]] auto data() -> char* { return mBuffer.data(); }

  [[nodiscard]] auto size() const noexcept -> usize { return mBuffer.size(); }

  [[nodiscard]] auto size_bytes() const noexcept -> usize
  {
    return mBuffer.size() * sizeof(char);
  }

  [[nodiscard]] auto as_string() const -> String;
  [[nodiscard]] auto as_string_view() const -> StringView;

 private:
  Array<char, 256> mBuffer;
};

[[nodiscard]] auto operator==(const StringBuffer& buffer, StringView str) -> bool;
[[nodiscard]] auto operator==(StringView str, const StringBuffer& buffer) -> bool;

}  // namespace tactile
