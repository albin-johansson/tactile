#include "remove_tileset.hpp"

#include <utility>  // move

#include "tactile_error.hpp"
#include "tactile_qstring.hpp"

namespace tactile::cmd {

RemoveTileset::RemoveTileset(not_null<core::MapDocument*> document,
                             shared<core::tileset> tileset,
                             const tileset_id id)
    : QUndoCommand{TACTILE_QSTRING(u"Remove Tileset")}
    , mDocument{document}
    , mTileset{std::move(tileset)}
    , mId{id}
{
  if (!mDocument)
  {
    throw TactileError{"Null map document!"};
  }

  if (!mTileset)
  {
    throw TactileError{"Null tileset!"};
  }
}

void RemoveTileset::undo()
{
  QUndoCommand::undo();

  auto* tilesets = mDocument->GetTilesets();
  tilesets->add(mId, mTileset);

  emit mDocument->S_AddedTileset(mId);
}

void RemoveTileset::redo()
{
  QUndoCommand::redo();

  auto& map = mDocument->Raw();
  auto* tilesets = mDocument->GetTilesets();

  const auto [first, last] = tilesets->range_of(mId);
  map.RemoveOccurrences(first, last);

  tilesets->remove(mId);

  emit mDocument->S_RemovedTileset(mId);
  emit mDocument->S_Redraw();
}

}  // namespace tactile::cmd
