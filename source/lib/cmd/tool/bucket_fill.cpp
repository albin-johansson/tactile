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

#include "bucket_fill.hpp"

#include "common/debug/assert.hpp"
#include "model/components/layer.hpp"
#include "model/layers/tile_layers.hpp"
#include "model/systems/language_system.hpp"
#include "model/systems/validation_system.hpp"

namespace tactile::cmd {

BucketFill::BucketFill(Model* model,
                       const Entity tile_layer_entity,
                       const TilePos origin,
                       const TileID replacement)
    : mModel {model},
      mTileLayerEntity {tile_layer_entity},
      mOrigin {origin},
      mReplacement {replacement}
{
  TACTILE_ASSERT(sys::is_tile_layer_entity(*mModel, mTileLayerEntity));
}

void BucketFill::undo()
{
  auto& model = *mModel;
  auto& tile_layer = model.get<TileLayer>(mTileLayerEntity);

  const auto target_tile_id = mTargetTileID.value();
  for (const auto& affected_position: mAffectedPositions) {
    tile_layer.set_tile(affected_position, target_tile_id);
  }

  mAffectedPositions.clear();
  mTargetTileID.reset();
}

void BucketFill::redo()
{
  auto& model = *mModel;
  auto& tile_layer = model.get<TileLayer>(mTileLayerEntity);

  mTargetTileID = tile_layer.tile_at(mOrigin);
  sys::flood_tiles(tile_layer, mOrigin, mReplacement, &mAffectedPositions);
}

auto BucketFill::get_name() const -> String
{
  const auto& strings = sys::get_current_language_strings(*mModel);
  return strings.cmd.bucket_tool;
}

}  // namespace tactile::cmd
