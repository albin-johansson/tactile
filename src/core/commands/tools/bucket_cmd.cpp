#include "bucket_cmd.hpp"

#include <cassert>  // assert

#include "core/map_document.hpp"
#include "core/tactile_error.hpp"

namespace Tactile {

BucketCmd::BucketCmd(NotNull<MapDocument*> document,
                     const MapPosition origin,
                     const tile_id replacement)
    : ACommand{"Bucket Fill"}
    , mDocument{document}
    , mOrigin{origin}
    , mReplacement{replacement}
{
  if (!mDocument)
  {
    throw TactileError{"Cannot create command from null document!"};
  }
}

void BucketCmd::Undo()
{
  auto& map = mDocument->GetMap();

  const auto layer = map.GetActiveLayerId().value();
  auto* tileLayer = map.GetTileLayer(layer);
  assert(tileLayer);

  for (const auto& position : mPositions)
  {
    tileLayer->SetTile(position, mTarget.value());
  }

  mTarget.reset();
}

void BucketCmd::Redo()
{
  mPositions.clear();

  auto& map = mDocument->GetMap();

  const auto layer = map.GetActiveLayerId().value();
  auto* tileLayer = map.GetTileLayer(layer);
  assert(tileLayer);

  mTarget = tileLayer->GetTile(mOrigin).value();
  tileLayer->Flood(mOrigin, mReplacement, mPositions);
}

}  // namespace Tactile
