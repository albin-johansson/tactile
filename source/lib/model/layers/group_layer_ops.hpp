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

#include "core/ecs/entity.hpp"
#include "core/functional/maybe.hpp"
#include "core/functional/result.hpp"
#include "core/prelude.hpp"
#include "model/layers/layer_components.hpp"
#include "model/maps/map_components.hpp"
#include "model/registry.hpp"

namespace tactile::sys {

/// Adds a layer to the group unless the layer has been added before.
auto attach_layer_to(GroupLayer& group_layer, Entity layer_entity) -> Result;

}  // namespace tactile::sys
