#include "stamp_sequence.hpp"

#include <utility>  // move

#include "map_document.hpp"
#include "tactile_error.hpp"
#include "tactile_qstring.hpp"
#include "tile_layer.hpp"

namespace tactile::cmd {

StampSequence::StampSequence(not_null<core::map_document*> document,
                             vector_map<core::position, tile_id>&& oldState,
                             vector_map<core::position, tile_id>&& sequence)
    : QUndoCommand{TACTILE_QSTRING(u"Stamp Sequence")}
    , mDocument{document}
    , mOldState{std::move(oldState)}
    , mSequence{std::move(sequence)}
{
  if (!mDocument)
  {
    throw tactile_error{"Cannot create stamp sequence command from null map!"};
  }
}

void StampSequence::undo()
{
  QUndoCommand::undo();

  auto& map = mDocument->raw();
  const auto layer = map.active_layer_id().value();

  map.select_layer(mLayer);

  auto* tileLayer = map.get_tile_layer(mLayer);
  Q_ASSERT(tileLayer);

  for (const auto& [position, tile] : mOldState)
  {
    tileLayer->set_tile(position, tile);
  }

  map.select_layer(layer);

  emit mDocument->redraw();
}

void StampSequence::redo()
{
  /* The stamp tools works directly when applied, so we don't do anything when
     the command is executed when first inserted into the command stack. */
  if (mFirst)
  {
    mLayer = mDocument->current_layer_id().value();
    mFirst = false;
    return;
  }

  QUndoCommand::redo();

  auto& map = mDocument->raw();
  const auto layer = map.active_layer_id().value();

  map.select_layer(mLayer);

  auto* tileLayer = map.get_tile_layer(mLayer);
  Q_ASSERT(tileLayer);

  for (const auto& [position, tile] : mSequence)
  {
    tileLayer->set_tile(position, tile);
  }

  map.select_layer(layer);

  emit mDocument->redraw();
}

}  // namespace tactile::cmd
