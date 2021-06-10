#include "set_tileset_name.hpp"

#include <QTranslator>  // tr
#include <utility>      // move

#include "map_document.hpp"
#include "tactile_error.hpp"

namespace tactile::cmd {

SetTilesetName::SetTilesetName(not_null<core::map_document*> document,
                               const tileset_id id,
                               QString name)
    : QUndoCommand{QTranslator::tr("Set Tileset Name")}
    , mDocument{document}
    , mId{id}
    , mName{std::move(name)}
{
  if (!mDocument)
  {
    throw tactile_error{"Cannot create command from null document!"};
  }
}

void SetTilesetName::undo()
{
  QUndoCommand::undo();

  const auto name = mPrevious.value();

  auto* tilesets = mDocument->tilesets();
  tilesets->rename(mId, name);

  emit mDocument->renamed_tileset(mId, name);
  mPrevious.reset();
}

void SetTilesetName::redo()
{
  QUndoCommand::redo();

  auto* tilesets = mDocument->tilesets();

  mPrevious = tilesets->at(mId).name();
  tilesets->rename(mId, mName);

  emit mDocument->renamed_tileset(mId, mName);
}

}  // namespace tactile::cmd
