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

#include "common/type/math.hpp"
#include "common/type/maybe.hpp"

namespace tactile {

struct ViewportLimits final {
  Float2 min_offset {};
  Float2 max_offset {};
};

struct Viewport final {
  Float2 offset {};
  Float2 tile_size {32, 32};
  Maybe<ViewportLimits> limits;

  [[nodiscard]] auto scaling_ratio(const Float2& logical_tile_size) const -> Float2;
};

}  // namespace tactile
