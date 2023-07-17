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
#include <utility>      // to_underlying

#include <fmt/core.h>

#include "common/type/ecs.hpp"

template <>
struct fmt::formatter<tactile::Entity> final : fmt::formatter<std::string_view> {
  auto format(const tactile::Entity entity, auto& ctx) const
  {
    if (entity == tactile::kNullEntity) {
      return fmt::format_to(ctx.out(), "Entity(NULL)");
    }
    else {
      return fmt::format_to(ctx.out(), "Entity({})", std::to_underlying(entity));
    }
  }
};

static_assert(fmt::is_formattable<tactile::Entity>::value);
