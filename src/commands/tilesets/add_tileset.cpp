#include "add_tileset.hpp"

#include <utility>  // move

#include "tactile_error.hpp"
#include "tactile_qstring.hpp"

namespace tactile::cmd {

AddTileset::AddTileset(not_null<core::MapDocument*> document,
                       shared<core::Tileset> tileset,
                       const tileset_id id)
    : QUndoCommand{TACTILE_QSTRING(u"Add Tileset")}
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
  tilesets->remove(mId);

  emit mDocument->S_RemovedTileset(mId);
  emit mDocument->S_Redraw();
}

void AddTileset::redo()
{
  QUndoCommand::redo();

  auto* tilesets = mDocument->GetTilesets();
  tilesets->add(mId, mTileset);

  emit mDocument->S_AddedTileset(mId);
}

}  // namespace tactile::cmd
