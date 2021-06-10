#include "bucket_fill.hpp"

#include "map_document.hpp"
#include "tactile_error.hpp"
#include "tactile_qstring.hpp"
#include "tile_layer.hpp"

namespace tactile::cmd {

BucketFill::BucketFill(not_null<core::MapDocument*> document,
                       const core::Position& position,
                       const tile_id replacement)
    : QUndoCommand{TACTILE_QSTRING(u"Bucket Fill")}
    , mDocument{document}
    , mOrigin{position}
    , mReplacement{replacement}
{
  if (!mDocument)
  {
    throw TactileError{"Cannot create command from null document!"};
  }
  mPositions.reserve(64);
}

void BucketFill::undo()
{
  QUndoCommand::undo();

  auto& map = mDocument->Raw();
  const auto layer = map.ActiveLayerId().value();

  map.SelectLayer(mLayer);

  auto* tileLayer = map.GetTileLayer(mLayer);
  Q_ASSERT(tileLayer);

  for (const auto& position : mPositions)
  {
    tileLayer->SetTile(position, mTarget);
  }

  map.SelectLayer(layer);
  emit mDocument->S_Redraw();
}

void BucketFill::redo()
{
  QUndoCommand::redo();

  mPositions.clear();

  auto& map = mDocument->Raw();
  mLayer = map.ActiveLayerId().value();

  auto* tileLayer = map.GetTileLayer(mLayer);
  Q_ASSERT(tileLayer);

  mTarget = tileLayer->TileAt(mOrigin).value();
  tileLayer->Flood(mOrigin, mReplacement, mPositions);

  emit mDocument->S_Redraw();
}

}  // namespace tactile::cmd
