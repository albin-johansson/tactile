#include "remove_tileset.hpp"

#include <utility>  // move

#include "tactile_error.hpp"

namespace tactile::cmd {

RemoveTileset::RemoveTileset(not_null<core::MapDocument*> document,
                             Shared<core::Tileset> tileset,
                             const tileset_id id)
    : QUndoCommand{QStringLiteral(u"Remove Tileset")}
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
  tilesets->Add(mId, mTileset);

  emit mDocument->S_AddedTileset(mId);
}

void RemoveTileset::redo()
{
  QUndoCommand::redo();

  auto& map = mDocument->Raw();
  auto* tilesets = mDocument->GetTilesets();

  const auto [first, last] = tilesets->RangeOf(mId);
  map.RemoveOccurrences(first, last);

  tilesets->Remove(mId);

  emit mDocument->S_RemovedTileset(mId);
  emit mDocument->S_Redraw();
}

}  // namespace tactile::cmd
