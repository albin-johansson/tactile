#include "set_layer_name.hpp"

#include <QTranslator>  // tr
#include <utility>      // move

#include "map_document.hpp"
#include "tactile_error.hpp"

namespace tactile::cmd {

SetLayerName::SetLayerName(not_null<core::MapDocument*> document,
                           const layer_id id,
                           QString name)
    : QUndoCommand{QTranslator::tr("Set Layer Name")}
    , mDocument{document}
    , mId{id}
    , mName{std::move(name)}
{
  if (!mDocument)
  {
    throw TactileError{"Cannot create command from null document!"};
  }
}

void SetLayerName::undo()
{
  QUndoCommand::undo();

  auto& map = mDocument->Raw();
  map.SetName(mId, mPrevious.value());

  emit mDocument->S_ChangedLayerName(mId, mPrevious.value());
  emit mDocument->S_Redraw();

  mPrevious.reset();
}

void SetLayerName::redo()
{
  QUndoCommand::redo();

  auto& map = mDocument->Raw();

  mPrevious = map.Name(mId);
  map.SetName(mId, mName);

  emit mDocument->S_ChangedLayerName(mId, mName);
  emit mDocument->S_Redraw();
}

}  // namespace tactile::cmd
