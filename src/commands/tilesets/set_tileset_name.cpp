#include "set_tileset_name.hpp"

#include <QTranslator>  // tr
#include <utility>      // move

#include "map_document.hpp"
#include "tactile_error.hpp"

namespace tactile::cmd {

SetTilesetName::SetTilesetName(not_null<core::MapDocument*> document,
                               const tileset_id id,
                               QString name)
    : QUndoCommand{QTranslator::tr("Set Tileset Name")}
    , mDocument{document}
    , mId{id}
    , mName{std::move(name)}
{
  if (!mDocument)
  {
    throw TactileError{"Cannot create command from null document!"};
  }
}

void SetTilesetName::undo()
{
  QUndoCommand::undo();

  const auto name = mPrevious.value();

  auto* tilesets = mDocument->GetTilesets();
  tilesets->rename(mId, name);

  emit mDocument->S_RenamedTileset(mId, name);
  mPrevious.reset();
}

void SetTilesetName::redo()
{
  QUndoCommand::redo();

  auto* tilesets = mDocument->GetTilesets();

  mPrevious = tilesets->at(mId).name();
  tilesets->rename(mId, mName);

  emit mDocument->S_RenamedTileset(mId, mName);
}

}  // namespace tactile::cmd
