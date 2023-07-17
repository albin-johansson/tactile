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

#include "common/primitives.hpp"
#include "common/type/ecs.hpp"
#include "common/type/maybe.hpp"
#include "model/layers/layer_components.hpp"
#include "model/maps/map_components.hpp"
#include "model/registry.hpp"

namespace tactile::sys {

void move_layer_up(Registry& registry, const Map& map, Entity layer_entity);

void move_layer_down(Registry& registry, const Map& map, Entity layer_entity);

void set_layer_local_index(Registry& registry,
                           const Map& map,
                           Entity layer_entity,
                           usize new_index);

[[nodiscard]] auto can_move_layer_up(const Registry& registry,
                                     const GroupLayer& root,
                                     Entity layer_entity) -> bool;

[[nodiscard]] auto can_move_layer_down(const Registry& registry,
                                       const GroupLayer& root,
                                       Entity layer_entity) -> bool;

[[nodiscard]] auto get_local_layer_index(const Registry& registry,
                                         const GroupLayer& root,
                                         Entity layer_entity) -> Maybe<usize>;

[[nodiscard]] auto get_parent_layer(const Registry& registry,
                                    const Map& map,
                                    Entity layer_entity) -> Entity;

}  // namespace tactile::sys
