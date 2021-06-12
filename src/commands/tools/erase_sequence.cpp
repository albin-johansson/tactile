#include "erase_sequence.hpp"

#include <utility>  // move

#include "map_document.hpp"
#include "tactile_error.hpp"
#include "tactile_qstring.hpp"
#include "tile_layer.hpp"

namespace tactile::cmd {

EraseSequence::EraseSequence(not_null<core::MapDocument*> document,
                             vector_map<core::MapPosition, tile_id>&& oldState)
    : QUndoCommand{TACTILE_QSTRING(u"Erase Tiles")}
    , mDocument{document}
    , mOldState{std::move(oldState)}
{
  if (!mDocument)
  {
    throw TactileError{"Cannot create erase sequence command from null map!"};
  }
}

void EraseSequence::undo()
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

void EraseSequence::redo()
{
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

  for (const auto& [position, _] : mOldState)
  {
    tileLayer->SetTile(position, empty);
  }

  map.SelectLayer(layer);

  emit mDocument->S_Redraw();
}

}  // namespace tactile::cmd
