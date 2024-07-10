// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <algorithm>    // min
#include <string_view>  // string_view

#include <boost/uuid/uuid_io.hpp>
#include <fmt/format.h>
#include <fmt/ostream.h>

#include "core/tile/tile_pos.hpp"
#include "tactile/base/container/array.hpp"
#include "tactile/base/container/string.hpp"
#include "tactile/base/int.hpp"
#include "tactile/base/meta/attribute.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/debug/stacktrace.hpp"
#include "tactile/core/numeric/vec.hpp"
#include "tactile/core/util/uuid.hpp"

namespace fmt {

template <>
struct formatter<tactile::UUID> : formatter<std::string_view> {
  auto format(const tactile::UUID& uuid, auto& ctx) const
  {
    return fmt::format_to(ctx.out(), "{}", fmt::streamed(uuid));
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
struct formatter<tactile::Float2> : formatter<std::string_view> {
  auto format(const tactile::Float2& vec, auto& ctx) const
  {
    return fmt::format_to(ctx.out(), "{{{}, {}}}", vec.x(), vec.y());
  }
};

template <>
struct formatter<tactile::Int2> : formatter<std::string_view> {
  auto format(const tactile::Int2& vec, auto& ctx) const
  {
    return fmt::format_to(ctx.out(), "{{{}, {}}}", vec.x(), vec.y());
  }
};

template <>
struct formatter<tactile::Float3> : formatter<std::string_view> {
  auto format(const tactile::Float3& vec, auto& ctx) const
  {
    return fmt::format_to(ctx.out(), "{{{}, {}, {}}}", vec.x(), vec.y(), vec.z());
  }
};

template <>
struct formatter<tactile::Int3> : formatter<std::string_view> {
  auto format(const tactile::Int3& vec, auto& ctx) const
  {
    return fmt::format_to(ctx.out(), "{{{}, {}, {}}}", vec.x(), vec.y(), vec.z());
  }
};

template <>
struct formatter<tactile::Float4> : formatter<std::string_view> {
  auto format(const tactile::Float4& vec, auto& ctx) const
  {
    return fmt::format_to(ctx.out(),
                          "{{{}, {}, {}, {}}}",
                          vec.x(),
                          vec.y(),
                          vec.z(),
                          vec.w());
  }
};

template <>
struct formatter<tactile::Int4> : formatter<std::string_view> {
  auto format(const tactile::Int4& vec, auto& ctx) const
  {
    return fmt::format_to(ctx.out(),
                          "{{{}, {}, {}, {}}}",
                          vec.x(),
                          vec.y(),
                          vec.z(),
                          vec.w());
  }
};

template <>
struct formatter<tactile::AttributeType> : formatter<std::string_view> {
  auto format(const tactile::AttributeType& type, auto& ctx) const
  {
    return fmt::format_to(ctx.out(), "{}", fmt::streamed(type));
  }
};

template <>
struct formatter<tactile::Attribute> : formatter<std::string_view> {
  auto format(const tactile::Attribute& attr, auto& ctx) const
  {
    return fmt::format_to(ctx.out(), "{}", fmt::streamed(attr));
  }
};

static_assert(is_formattable<tactile::Int2, char>::value);
static_assert(is_formattable<tactile::Int3, char>::value);
static_assert(is_formattable<tactile::Int4, char>::value);
static_assert(is_formattable<tactile::Float2, char>::value);
static_assert(is_formattable<tactile::Float3, char>::value);
static_assert(is_formattable<tactile::Float4, char>::value);
static_assert(is_formattable<tactile::AttributeType, char>::value);
static_assert(is_formattable<tactile::Attribute, char>::value);
static_assert(is_formattable<tactile::TilePos, char>::value);

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
