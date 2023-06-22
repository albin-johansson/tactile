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

#include "map_factory.hpp"

#include "model/contexts/context_components.hpp"
#include "model/layers/layer_factory.hpp"
#include "model/maps/map_components.hpp"
#include "model/tiles/tile_components.hpp"

namespace tactile::sys {

auto create_map(Model& model, const TileExtent& extent, const Int2& tile_size) -> Entity
{
  const auto map_entity = model.create_entity();

  auto& map_context = model.add<Context>(map_entity);
  map_context.name = "Map";

  auto& map = model.add<Map>(map_entity);
  map.extent = extent;
  map.tile_size = tile_size;
  map.root_layer = create_group_layer(model, -1);
  map.active_tileset = kNullEntity;
  map.active_layer = kNullEntity;

  auto& identifiers = model.add<MapIdentifiers>(map_entity);
  identifiers.next_tile_id = 1;
  identifiers.next_layer_id = 1;
  identifiers.next_object_id = 1;

  auto& layer_suffixes = model.add<MapLayerSuffixes>(map_entity);
  layer_suffixes.tile_layer_suffix = 1;
  layer_suffixes.object_layer_suffix = 1;
  layer_suffixes.group_layer_suffix = 1;

  model.add<TileFormat>(map_entity);

  return map_entity;
}

}  // namespace tactile::sys
