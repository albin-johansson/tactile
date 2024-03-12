// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "bucket_fill.hpp"

#include <utility>  // move

#include "core/layer/group_layer.hpp"
#include "core/layer/tile_layer.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "tactile/core/debug/exception.hpp"

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
    throw Exception {"Invalid null map!"};
  }
}

void BucketFill::undo()
{
  auto& layer = mMap->get_invisible_root().get_tile_layer(mLayerId);

  const auto target = mTarget.value();
  for (const auto& position: mPositions) {
    layer.set_tile(position, target);
  }

  mPositions.clear();
  mTarget.reset();
}

void BucketFill::redo()
{
  auto& layer = mMap->get_invisible_root().get_tile_layer(mLayerId);

  mTarget = layer.tile_at(mOrigin);
  layer.flood(mOrigin, mReplacement, &mPositions);
}

auto BucketFill::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.bucket_tool;
}

}  // namespace tactile::cmd
