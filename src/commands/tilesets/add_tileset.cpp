#include "add_tileset.hpp"

#include <utility>  // move

#include "tactile_error.hpp"

namespace tactile::cmd {

AddTileset::AddTileset(not_null<core::MapDocument*> document,
                       Shared<core::Tileset> tileset,
                       const tileset_id id)
    : QUndoCommand{QStringLiteral(u"Add Tileset")}
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

void AddTileset::undo()
{
  QUndoCommand::undo();

  auto* tilesets = mDocument->GetTilesets();
  tilesets->Remove(mId);

  emit mDocument->S_RemovedTileset(mId);
  emit mDocument->S_Redraw();
}

void AddTileset::redo()
{
  QUndoCommand::redo();

  auto* tilesets = mDocument->GetTilesets();
  tilesets->Add(mId, mTileset);

  emit mDocument->S_AddedTileset(mId);
}

}  // namespace tactile::cmd
