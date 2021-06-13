#include "stamp_sequence.hpp"

#include <utility>  // move

#include "map_document.hpp"
#include "tactile_error.hpp"
#include "tile_layer.hpp"

namespace tactile::cmd {

StampSequence::StampSequence(not_null<core::MapDocument*> document,
                             vector_map<core::MapPosition, tile_id>&& oldState,
                             vector_map<core::MapPosition, tile_id>&& sequence)
    : QUndoCommand{QStringLiteral(u"Stamp Sequence")}
    , mDocument{document}
    , mOldState{std::move(oldState)}
    , mSequence{std::move(sequence)}
{
  if (!mDocument)
  {
    throw TactileError{"Cannot create stamp sequence command from null map!"};
  }
}

void StampSequence::undo()
{
  QUndoCommand::undo();

  auto& map = mDocument->Raw();
  const auto layer = map.ActiveLayerId().value();

  map.SelectLayer(mLayer);

  auto* tileLayer = map.GetTileLayer(mLayer);
  Q_ASSERT(tileLayer);

  for (const auto& [position, tile] : mOldState)
  {
    tileLayer->SetTile(position, tile);
  }

  map.SelectLayer(layer);

  emit mDocument->S_Redraw();
}

void StampSequence::redo()
{
  /* The stamp tools works directly when applied, so we don't do anything when
     the command is executed when first inserted into the command stack. */
  if (mFirst)
  {
    mLayer = mDocument->CurrentLayerId().value();
    mFirst = false;
    return;
  }

  QUndoCommand::redo();

  auto& map = mDocument->Raw();
  const auto layer = map.ActiveLayerId().value();

  map.SelectLayer(mLayer);

  auto* tileLayer = map.GetTileLayer(mLayer);
  Q_ASSERT(tileLayer);

  for (const auto& [position, tile] : mSequence)
  {
    tileLayer->SetTile(position, tile);
  }

  map.SelectLayer(layer);

  emit mDocument->S_Redraw();
}

}  // namespace tactile::cmd
