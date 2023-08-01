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

#include "core/ecs/entity_callback.hpp"
#include "model/layers/layer_components.hpp"
#include "model/maps/map_components.hpp"
#include "model/registry.hpp"

namespace tactile::sys {

void visit_layers(const Registry& registry,
                  const GroupLayer& root_layer,
                  const EntityCallback& callback);

void visit_layers(const Registry& registry,
                  const Map& map,
                  const EntityCallback& callback);

void visit_tile_layers(const Registry& registry,
                       const GroupLayer& root_layer,
                       const EntityCallback& callback);

void visit_tile_layers(const Registry& registry,
                       const Map& map,
                       const EntityCallback& callback);

}  // namespace tactile::sys
