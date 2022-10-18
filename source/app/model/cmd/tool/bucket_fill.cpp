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

#include "bucket_fill.hpp"

#include <utility>  // move

#include "core/layer/tile_layer.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "misc/panic.hpp"

namespace tactile::cmd {

BucketFill::BucketFill(Shared<Map> map,
                       const UUID& layer_id,
                       const TilePos& origin,
                       const TileID replacement)
    : mMap {std::move(map)},
      mLayerId {layer_id},
      mOrigin {origin},
      mReplacement {replacement}
{
  if (!mMap) {
    throw TactileError {"Invalid null map!"};
  }
}

void BucketFill::undo()
{
  auto& layer = mMap->invisible_root().tile_layer(mLayerId);

  const auto target = mTarget.value();
  for (const auto& position: mPositions) {
    layer.set_tile(position, target);
  }

  mPositions.clear();
  mTarget.reset();
}

void BucketFill::redo()
{
  auto& layer = mMap->invisible_root().tile_layer(mLayerId);

  mTarget = layer.tile_at(mOrigin);
  layer.flood(mOrigin, mReplacement, &mPositions);
}

auto BucketFill::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.bucket_tool;
}

}  // namespace tactile::cmd
