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
#include "common/tile_extent.hpp"
#include "common/type/ecs.hpp"
#include "common/type/func.hpp"
#include "model/components/map.hpp"
#include "model/layers/layer_components.hpp"
#include "model/model.hpp"

namespace tactile::sys {

[[nodiscard]] auto get_parent_layer(const Model& model,
                                    const Map& map,
                                    Entity layer_entity) -> Entity;

}  // namespace tactile::sys
