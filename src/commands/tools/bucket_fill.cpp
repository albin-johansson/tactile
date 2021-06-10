#include "bucket_fill.hpp"

#include "map_document.hpp"
#include "tactile_error.hpp"
#include "tactile_qstring.hpp"
#include "tile_layer.hpp"

namespace tactile::cmd {

BucketFill::BucketFill(not_null<core::map_document*> document,
                       const core::position& position,
                       const tile_id replacement)
    : QUndoCommand{TACTILE_QSTRING(u"Bucket Fill")}
    , mDocument{document}
    , mOrigin{position}
    , mReplacement{replacement}
{
  if (!mDocument)
  {
    throw tactile_error{"Cannot create command from null document!"};
  }
  mPositions.reserve(64);
}

void BucketFill::undo()
{
  QUndoCommand::undo();

  auto& map = mDocument->raw();
  const auto layer = map.active_layer_id().value();

  map.select_layer(mLayer);

  auto* tileLayer = map.get_tile_layer(mLayer);
  Q_ASSERT(tileLayer);

  for (const auto& position : mPositions)
  {
    tileLayer->set_tile(position, mTarget);
  }

  map.select_layer(layer);
  emit mDocument->redraw();
}

void BucketFill::redo()
{
  QUndoCommand::redo();

  mPositions.clear();

  auto& map = mDocument->raw();
  mLayer = map.active_layer_id().value();

  auto* tileLayer = map.get_tile_layer(mLayer);
  Q_ASSERT(tileLayer);

  mTarget = tileLayer->tile_at(mOrigin).value();
  tileLayer->flood(mOrigin, mReplacement, mPositions);

  emit mDocument->redraw();
}

}  // namespace tactile::cmd
