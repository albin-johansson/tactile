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

#include "group_layer_ops.hpp"

#include <algorithm>  // find, iter_swap

#include "common/util/algorithms.hpp"
#include "common/util/functional.hpp"
#include "common/util/vectors.hpp"
#include "core/debug/assert.hpp"
#include "core/functional/maybe.hpp"
#include "model/entity_validation.hpp"
#include "model/layers/layer_tree_system.hpp"
#include "model/maps/map_components.hpp"

namespace tactile::sys {

auto attach_layer_to(GroupLayer& group_layer, const Entity layer_entity) -> Result
{
  if (!contained_in(group_layer.children, layer_entity)) {
    group_layer.children.push_back(layer_entity);
    return success;
  }

  return failure;
}

}  // namespace tactile::sys
