#include "remove_tileset.hpp"

#include <utility>  // move

#include "tactile_error.hpp"
#include "tactile_qstring.hpp"

namespace tactile::cmd {

RemoveTileset::RemoveTileset(not_null<core::map_document*> document,
                             shared<core::tileset> tileset,
                             const tileset_id id)
    : QUndoCommand{TACTILE_QSTRING(u"Remove Tileset")}
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

void RemoveTileset::undo()
{
  QUndoCommand::undo();

  auto* tilesets = mDocument->tilesets();
  tilesets->add(mId, mTileset);

  emit mDocument->added_tileset(mId);
}

void RemoveTileset::redo()
{
  QUndoCommand::redo();

  auto& map = mDocument->raw();
  auto* tilesets = mDocument->tilesets();

  const auto [first, last] = tilesets->range_of(mId);
  map.remove_occurrences(first, last);

  tilesets->remove(mId);

  emit mDocument->removed_tileset(mId);
  emit mDocument->redraw();
}

}  // namespace tactile::cmd
