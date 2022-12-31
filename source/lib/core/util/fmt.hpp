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

#include <algorithm>    // min
#include <sstream>      // stringstream
#include <string_view>  // string_view

#include <boost/uuid/uuid_io.hpp>
#include <fmt/format.h>
#include <fmt/ostream.h>

#include "core/debug/stacktrace.hpp"
#include "core/predef.hpp"
#include "core/tile/tile_pos.hpp"
#include "core/type/array.hpp"
#include "core/type/math.hpp"
#include "core/type/string.hpp"
#include "core/type/uuid.hpp"
#include "core/vocabulary.hpp"

namespace fmt {

template <>
struct formatter<boost::stacktrace::stacktrace> : formatter<std::string_view> {
  auto format(const boost::stacktrace::stacktrace& trace, auto& ctx) const
  {
    std::stringstream stream;
    stream << trace;
    return formatter<std::string_view>::format(stream.str(), ctx);
  }
};

template <>
struct formatter<tactile::UUID> : formatter<std::string_view> {
  auto format(const tactile::UUID& uuid, auto& ctx) const
  {
    std::stringstream stream;
    stream << uuid;
    return formatter<std::string_view>::format(stream.str(), ctx);
  }
};

template <>
struct formatter<tactile::TilePos> : formatter<std::string_view> {
  auto format(const tactile::TilePos& pos, auto& ctx) const
  {
    return fmt::format_to(ctx.out(), "{{{}, {}}}", pos.row(), pos.col());
  }
};

template <>
struct formatter<tactile::Vec2> : formatter<std::string_view> {
  auto format(const tactile::Vec2& vec, auto& ctx) const
  {
    return fmt::format_to(ctx.out(), "{{{}, {}}}", vec.x, vec.y);
  }
};

template <>
struct formatter<tactile::Vec2i> : formatter<std::string_view> {
  auto format(const tactile::Vec2i& vec, auto& ctx) const
  {
    return fmt::format_to(ctx.out(), "{{{}, {}}}", vec.x, vec.y);
  }
};

static_assert(is_formattable<tactile::Int2, char>::value);
static_assert(is_formattable<tactile::Float2, char>::value);
static_assert(is_formattable<tactile::TilePos, char>::value);
static_assert(is_formattable<boost::stacktrace::stacktrace, char>::value);

}  // namespace fmt

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