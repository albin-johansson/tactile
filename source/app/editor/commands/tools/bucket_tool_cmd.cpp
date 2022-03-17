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

#include "bucket_tool_cmd.hpp"

#include "core/algorithms/flood_fill.hpp"
#include "core/components/layers.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "core/systems/layers/tile_layer_system.hpp"
#include "misc/assert.hpp"

namespace tactile {

BucketToolCmd::BucketToolCmd(RegistryRef registry,
                             const TilePos origin,
                             const tile_id replacement)
    : ACommand{"Bucket Fill"}
    , mRegistry{registry}
    , mLayer{sys::get_active_layer_id(registry).value()}
    , mOrigin{origin}
    , mReplacement{replacement}
{}

void BucketToolCmd::undo()
{
  auto& registry = mRegistry.get();

  const auto entity = sys::find_layer(registry, mLayer);
  TACTILE_ASSERT(entity != entt::null);
  TACTILE_ASSERT(registry.all_of<comp::TileLayer>(entity));

  const auto target = mTarget.value();
  auto& layer = registry.get<comp::TileLayer>(entity);
  for (const auto& position : mPositions) {
    layer.matrix.at(position.row_index()).at(position.col_index()) = target;
  }

  mPositions.clear();
  mTarget.reset();
}

void BucketToolCmd::redo()
{
  auto& registry = mRegistry.get();

  const auto entity = sys::get_tile_layer_entity(registry, mLayer);
  const auto& layer = registry.get<comp::TileLayer>(entity);

  mTarget = sys::get_tile(layer, mOrigin);
  flood(registry, entity, mOrigin, mReplacement, mPositions);
}

}  // namespace tactile
