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

#include "common/numeric.hpp"
#include "common/type/ecs.hpp"
#include "core/layer.hpp"
#include "model/model.hpp"

namespace tactile::sys {

void move_layer_up(Model& model, Entity root_layer_entity, Entity layer_entity);

void move_layer_down(Model& model, Entity root_layer_entity, Entity layer_entity);

void set_layer_local_index(Model& model,
                           Entity root_layer_entity,
                           Entity layer_entity,
                           usize new_index);

[[nodiscard]] auto can_move_layer_up(const Model& model,
                                     const GroupLayer& root,
                                     Entity layer_entity) -> bool;

[[nodiscard]] auto can_move_layer_down(const Model& model,
                                       const GroupLayer& root,
                                       Entity layer_entity) -> bool;

[[nodiscard]] auto get_local_layer_index(const Model& model,
                                         const GroupLayer& root,
                                         Entity layer_entity) -> Maybe<usize>;

}  // namespace tactile::sys
