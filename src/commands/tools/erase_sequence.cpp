#include "erase_sequence.hpp"

#include <utility>  // move

#include "map_document.hpp"
#include "tactile_error.hpp"
#include "tactile_qstring.hpp"
#include "tile_layer.hpp"

namespace tactile::cmd {

EraseSequence::EraseSequence(not_null<core::map_document*> document,
                             vector_map<core::position, tile_id>&& oldState)
    : QUndoCommand{TACTILE_QSTRING(u"Erase Tiles")}
    , mDocument{document}
    , mOldState{std::move(oldState)}
{
  if (!mDocument)
  {
    throw tactile_error{"Cannot create erase_sequence command from null map!"};
  }
}

void EraseSequence::undo()
{
  QUndoCommand::undo();

  auto& map = mDocument->raw();
  const auto layer = map.active_layer_id().value();

  map.select_layer(mLayer);

  auto* tileLayer = map.get_tile_layer(mLayer);
  Q_ASSERT(tileLayer);

  for (const auto& [position, tile] : mOldState)
  {
    tileLayer->SetTile(position, tile);
  }

  map.select_layer(layer);

  emit mDocument->redraw();
}

void EraseSequence::redo()
{
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

  for (const auto& [position, _] : mOldState)
  {
    tileLayer->SetTile(position, empty);
  }

  map.select_layer(layer);

  emit mDocument->redraw();
}

}  // namespace tactile::cmd
