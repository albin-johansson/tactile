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

#include <string_view>  // string_view

#include <fmt/core.h>
#include <fmt/ostream.h>

#include "core/math/vector.hpp"

template <>
struct fmt::formatter<tactile::Float2> : fmt::formatter<std::string_view> {
  auto format(const tactile::Float2& vec, auto& ctx) const
  {
    return fmt::format_to(ctx.out(), "{{{}, {}}}", vec.x, vec.y);
  }
};

template <>
struct fmt::formatter<tactile::Int2> : fmt::formatter<std::string_view> {
  auto format(const tactile::Int2& vec, auto& ctx) const
  {
    return fmt::format_to(ctx.out(), "{{{}, {}}}", vec.x, vec.y);
  }
};

template <>
struct fmt::formatter<tactile::Float3> : fmt::formatter<std::string_view> {
  auto format(const tactile::Float3& vec, auto& ctx) const
  {
    return fmt::format_to(ctx.out(), "{{{}, {}, {}}}", vec.x, vec.y, vec.z);
  }
};

template <>
struct fmt::formatter<tactile::Int3> : fmt::formatter<std::string_view> {
  auto format(const tactile::Int3& vec, auto& ctx) const
  {
    return fmt::format_to(ctx.out(), "{{{}, {}, {}}}", vec.x, vec.y, vec.z);
  }
};

template <>
struct fmt::formatter<tactile::Float4> : fmt::formatter<std::string_view> {
  auto format(const tactile::Float4& vec, auto& ctx) const
  {
    return fmt::format_to(ctx.out(), "{{{}, {}, {}, {}}}", vec.x, vec.y, vec.z, vec.w);
  }
};

template <>
struct fmt::formatter<tactile::Int4> : fmt::formatter<std::string_view> {
  auto format(const tactile::Int4& vec, auto& ctx) const
  {
    return fmt::format_to(ctx.out(), "{{{}, {}, {}, {}}}", vec.x, vec.y, vec.z, vec.w);
  }
};

static_assert(fmt::is_formattable<tactile::Int2>::value);
static_assert(fmt::is_formattable<tactile::Int3>::value);
static_assert(fmt::is_formattable<tactile::Int4>::value);
static_assert(fmt::is_formattable<tactile::Float2>::value);
static_assert(fmt::is_formattable<tactile::Float3>::value);
static_assert(fmt::is_formattable<tactile::Float4>::value);
