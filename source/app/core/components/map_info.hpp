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

#include "core/common/identifiers.hpp"
#include "core/common/ints.hpp"
#include "core/common/math.hpp"

namespace tactile::comp {

/**
 * Provides information about a map context.
 *
 * \ingroup components
 */
struct MapInfo final
{
  usize row_count{};             ///< Total amount of rows.
  usize column_count{};          ///< Total amount of columns.
  Vector2i tile_size{};          ///< Logical size of all tiles.
  LayerID next_layer_id{1};      ///< The next available layer identifier.
  ObjectID next_object_id{1};    ///< The next available object identifier.
  int32 tile_layer_suffix{1};    ///< Incrementing tile layer suffix.
  int32 object_layer_suffix{1};  ///< Incrementing object layer suffix.
  int32 group_layer_suffix{1};   ///< Incrementing group layer suffix.
};

}  // namespace tactile::comp
