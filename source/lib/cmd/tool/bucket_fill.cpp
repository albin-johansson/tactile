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
#include "model/entity_validation.hpp"
#include "model/layers/layer_components.hpp"
#include "model/layers/tile_layer_ops.hpp"
#include "model/services/language_service.hpp"
#include "model/services/service_locator.hpp"

namespace tactile::cmd {

BucketFill::BucketFill(Registry* registry,
                       const Entity tile_layer_entity,
                       const TilePos origin,
                       const TileID replacement)
    : mRegistry {registry},
      mTileLayerEntity {tile_layer_entity},
      mOrigin {origin},
      mReplacement {replacement}
{
  TACTILE_ASSERT(sys::is_tile_layer_entity(*mRegistry, mTileLayerEntity));
}

void BucketFill::undo()
{
  auto& registry = *mRegistry;
  auto& tile_layer = registry.get<TileLayer>(mTileLayerEntity);

  const auto target_tile_id = mTargetTileID.value();
  for (const auto& affected_position: mAffectedPositions) {
    sys::set_tile(tile_layer, affected_position, target_tile_id);
  }

  mAffectedPositions.clear();
  mTargetTileID.reset();
}

void BucketFill::redo()
{
  auto& registry = *mRegistry;
  auto& tile_layer = registry.get<TileLayer>(mTileLayerEntity);

  mTargetTileID = sys::tile_at(tile_layer, mOrigin);
  sys::flood_tiles(tile_layer, mOrigin, mReplacement, &mAffectedPositions);
}

auto BucketFill::get_name() const -> String
{
  const auto& strings =
      ServiceLocator<LanguageService>::get().get_current_language_strings();
  return strings.cmd.bucket_tool;
}

}  // namespace tactile::cmd
