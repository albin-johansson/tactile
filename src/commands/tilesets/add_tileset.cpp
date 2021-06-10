#include "add_tileset.hpp"

#include <utility>  // move

#include "tactile_error.hpp"
#include "tactile_qstring.hpp"

namespace tactile::cmd {

AddTileset::AddTileset(not_null<core::map_document*> document,
                       shared<core::tileset> tileset,
                       const tileset_id id)
    : QUndoCommand{TACTILE_QSTRING(u"Add Tileset")}
    , mDocument{document}
    , mTileset{std::move(tileset)}
    , mId{id}
{
  if (!mDocument)
  {
    throw tactile_error{"Null map document!"};
  }

  if (!mTileset)
  {
    throw tactile_error{"Null tileset!"};
  }
}

void AddTileset::undo()
{
  QUndoCommand::undo();

  auto* tilesets = mDocument->tilesets();
  tilesets->remove(mId);

  emit mDocument->removed_tileset(mId);
  emit mDocument->redraw();
}

void AddTileset::redo()
{
  QUndoCommand::redo();

  auto* tilesets = mDocument->tilesets();
  tilesets->add(mId, mTileset);

  emit mDocument->added_tileset(mId);
}

}  // namespace tactile::cmd
