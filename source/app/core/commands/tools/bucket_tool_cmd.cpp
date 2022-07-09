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

#include <utility>  // move

#include "core/layers/tile_layer.hpp"
#include "misc/panic.hpp"

namespace tactile {

BucketToolCmd::BucketToolCmd(Shared<Map>    map,
                             const UUID&    layerId,
                             const TilePos& origin,
                             const TileID   replacement)
    : mMap{std::move(map)}
    , mLayerId{layerId}
    , mOrigin{origin}
    , mReplacement{replacement}
{
  if (!mMap) {
    throw TactileError{"Invalid null map!"};
  }
}

void BucketToolCmd::undo()
{
  auto& layer = mMap->view_tile_layer(mLayerId);

  const auto target = mTarget.value();
  for (const auto& position : mPositions) {
    layer.set_tile(position, target);
  }

  mPositions.clear();
  mTarget.reset();
}

void BucketToolCmd::redo()
{
  auto& layer = mMap->view_tile_layer(mLayerId);

  mTarget = layer.tile_at(mOrigin);
  layer.flood(mOrigin, mReplacement, &mPositions);
}

auto BucketToolCmd::get_name() const -> const char*
{
  return "Bucket Fill";
}

}  // namespace tactile
